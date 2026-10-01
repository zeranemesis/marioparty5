#!/usr/bin/env python3

"""
Strict per-function diff between a target DOL disassembly and a candidate .c.

The target is a doldecomp-style .s (absolute addresses, raw relocations). The
candidate is compiled with the project's real MWCC flags by tools/mwccprobe.py.
Both sides are reduced to a canonical instruction stream:

  * numbers are normalised (the target writes 0x1b0, objdump writes 432),
  * relocations become plain symbol names (so `bl OSGetTime` compares equal),
  * local branch targets become `<t>` - their numeric/symbolic identity differs
    between the two dumps, only the fact that a branch is there is comparable,
  * `b`/`bl`/`beq`... must still be the *same kind* of branch, and a call must
    still name the same callee.

Usage:
  python tools/asmdiff.py src/dolphin/os/OSStopwatch.c
  python tools/asmdiff.py src/dolphin/os/OSStopwatch.c OSStopStopwatch
  python tools/asmdiff.py -v src/dolphin/os/OSStopwatch.c OSDumpStopwatch

This is a scratch/authoritative tool: it does not go through ninja or objdiff,
so it works on any candidate file, including one that is not in the build yet.
Use tools/fndiff.py for scoring.
"""

import os
import difflib
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent

FUNC_RE = re.compile(r"^([0-9a-fA-F]{8}) <([^>]+)>:$")
INSN_RE = re.compile(r"^\s*([0-9a-f]+):\s+((?:[0-9a-fA-F]{2} )+)[ \t]*(\S+)[ \t]*(.*)$")
RELOC_RE = re.compile(r"^\s*[0-9a-f]+:\s*R_PPC_\S+\s+(\S+)\s*$")
TARGET_FN_RE = re.compile(r"^\.fn\s+([^,]+),")
# the target's comment carries the vaddr, the file offset and the raw bytes:
#   /* 8013C3D8 001391F8  54 A6 F6 FE */\textrwi r6, r5, 5, 25
TARGET_INSN_RE = re.compile(r"^/\*[ \t]*[0-9a-fA-F]+[ \t]+[0-9a-fA-F]+[ \t]+((?:[0-9a-fA-F]{2} )*[0-9a-fA-F]{2})[ \t]*\*/[ \t]*(\S+)[ \t]*(.*?)\s*$")
# `bl`/`bla` must be in here: the branch arm of canon_insn is what consumes the
# resolved relocation, so a call whose mnemonic falls through would lose its
# callee name and be compared as raw text.
BRANCHES = ("b", "bc", "bcl", "bclr", "bctrl", "bcctr", "bcctrl", "bl", "bla")

MEM_RE = re.compile(r"^([+-]?(?:0x[0-9a-fA-F]+|\d+))\((.+)\)$")

# The two disassemblers spell the same 32-bit encoding differently: doldecomp
# prints the architectural alias (`extrwi r6,r5,5,25`) where objdump prints the
# raw rotate (`rlwinm r6,r5,30,27,31`), and likewise `dcbt` vs `dcbtct`.  For
# these mnemonics the operand text carries no meaning, so we compare the raw
# encoding instead - which is strictly stronger than comparing the mnemonic.
AMBIGUOUS = {"rlwinm", "rotlwi", "slwi", "srwi", "clrlwi", "clrlslwi", "extrwi",
             "dcbt", "dcbtst", "dcbtct"}

# objdump and doldecomp spell the same SPR differently: objdump prints the
# decimal SPR number, doldecomp prints its name.  The keys are *strings*
# because the operands reach this table already canonicalised as text.
SPR_NAMES = {"1008": "HID0", "920": "HID2", "1017": "L2CR", "922": "DMA_U", "923": "DMA_L",
             "1015": "MASR", "1014": "EAR", "1010": "TBL", "269": "TBU", "268": "TBL",
             "19": "XER", "8": "LR", "9": "CTR"}


def num(text: str) -> str:
    return str(int(text, 16) if text.lower().startswith(("0x", "-0x", "+0x")) else int(text))


def canon_operand(operand: str) -> str:
    operand = operand.strip()
    if not operand:
        return ""
    if "@" in operand:  # lis r3, lbl_801CEB58@ha
        operand = operand.split("@")[0].strip()
    parts = [p.strip() for p in operand.split(",")]
    out = []
    for part in parts:
        m = MEM_RE.match(part)
        if m:
            out.append(num(m.group(1)) + "(" + m.group(2) + ")")
        elif re.fullmatch(r"[+-]?(?:0x[0-9a-fA-F]+|\d+)", part):
            out.append(num(part))
        else:
            out.append(part)
    
    # objdump writes `4*cr1+eq`, doldecomp writes `cr1eq`
    out = [re.sub(r"\d+\*(cr\d+)\+(\w+)", r"\1\2", p) for p in out]
    # objdump prints the branch register as `0`, doldecomp as `r0`.  The
    # lookbehind must be case-insensitive, otherwise the trailing `0` of a
    # SPR name (`HID0`, `L2CR`...) would be rewritten to `HIDr0`.
    out = [re.sub(r"(?<![A-Za-z0-9_])0(?=,|$)", "r0", p) for p in out]
    # the target's string pool / data labels have build-specific addresses
    out = [re.sub(r"lbl[._][\w.]*", "SYM", p) for p in out]
    return ",".join(out)


def canon_insn(mnemonic: str, operand: str | None, reloc: str | None = None,
               word: int | None = None) -> str:
    operand = (operand or "").strip()
    if mnemonic in AMBIGUOUS:
        return f"encoding {word:08x}" if word is not None else f"{mnemonic} {operand}"
    if mnemonic in BRANCHES or mnemonic.startswith("b") and mnemonic not in (
            "bclr", "bctrl") and re.match(r"^b(c|cl|ne|eq|lt|gt|le|ge|so|ns|dnz)?$", mnemonic):
        # branch / call: a reloc gives us the real callee, otherwise local target
        target = reloc
        if target is None:
            first = operand.split(",")[-1].strip()
            target = first if re.fullmatch(r"[A-Za-z_][\w]*", first) else "<t>"
        if mnemonic == "bl" and target.startswith("."):
            target = "<t>"
        return f"{mnemonic} {target}"
    # doldecomp prints `subi rX,rY,0x217d` where objdump prints the very same
    # encoding as `addi rX,rY,-8573`; `lis rX,32768` is `-32768` in objdump.
    if mnemonic == "hwsync":  # objdump's name for `sync`
        return "sync"
    # `mftbu r3` (objdump) is `mftb r3,0x10d` (doldecomp), `mftb r3` == 0x10c
    if mnemonic in ("mftb", "mftbu", "mftbl"):
        parts = canon_operand(operand).split(",")
        if len(parts) > 1:  # doldecomp spells out the TBR selector
            return f"mftb {','.join(parts)}"
        return f"mftb {parts[0]},{269 if mnemonic == 'mftbu' else 268}"
    if mnemonic in ("subi", "subis"):
        parts = [p.strip() for p in operand.split(",")]
        last = parts[-1]
        if re.fullmatch(r"[+-]?(?:0x[0-9a-fA-F]+|\d+)", last):
            parts[-1] = str(-int(last, 0))
        return f"{'addi' if mnemonic == 'subi' else 'addis'} {','.join(parts)}"
    # a data reloc (lis/addi on a string pool) - the symbol is what matters
    if reloc is not None:
        parts = canon_operand(operand).split(",")
        if parts and re.fullmatch(r"[+-]?\d+", parts[-1]):
            parts[-1] = "SYM"
        operand = ",".join(parts)
    # objdump prints the SPR as its decimal number, doldecomp as its name.
    # `mfspr rA,SPR` but `mtspr SPR,rA`: the SPR is not in the same slot.
    if mnemonic in ("mfspr", "mtspr"):
        parts = canon_operand(operand).split(",")
        i = 0 if mnemonic == "mtspr" else 1
        if len(parts) == 2:
            parts[i] = SPR_NAMES.get(parts[i], parts[i])
        return f"{mnemonic} {','.join(parts)}"
    if mnemonic == "lis":
        parts = canon_operand(operand).split(",")
        parts = [p for p in parts if p]
        if re.fullmatch(r"[+-]?(?:0x[0-9a-fA-F]+|\d+)", parts[-1]):
            parts[-1] = str(int(parts[-1], 0) & 0xFFFF)
        return f"lis {','.join(parts)}"
    return f"{mnemonic} {canon_operand(operand)}".strip()


def parse_candidate(text: str) -> dict[str, list[str]]:
    funcs: dict[str, list[str]] = {}
    current = None
    pending = None
    for line in text.splitlines():
        m = FUNC_RE.match(line)
        if m:
            current = m.group(2)
            funcs[current] = []
            pending = None
            continue
        m = RELOC_RE.match(line)
        if m and pending is not None and current is not None:
            funcs[current][-1] = canon_insn(*pending, reloc=m.group(1))
            pending = None
            continue
        m = INSN_RE.match(line)
        if m and current is not None:
            # group(2) is the raw big-endian byte string of the instruction
            word = int.from_bytes(bytes(int(b, 16) for b in m.group(2).split()), "big")
            funcs[current].append(canon_insn(m.group(3), m.group(4), word=word))
            pending = (m.group(3), m.group(4))
    return funcs


def parse_target(text: str) -> dict[str, list[str]]:
    funcs: dict[str, list[str]] = {}
    current = None
    for raw in text.splitlines():
        m = TARGET_FN_RE.match(raw)
        if m:
            current = m.group(1).strip()
            funcs.setdefault(current, [])
            continue
        if raw.startswith(".endfn"):
            current = None
            continue
        if current is None:
            continue
        m = TARGET_INSN_RE.match(raw)
        if m:
            word = int.from_bytes(bytes(int(b, 16) for b in m.group(1).split()), "big")
            funcs[current].append(canon_insn(m.group(2), m.group(3), word=word))
    return funcs


def is_branch(insn: str) -> bool:
    return insn.split(" ", 1)[0].startswith("b")


def diff_function(name: str, got: list[str], want: list[str], verbose: bool) -> tuple[int, int]:
    """Align the two streams and report matching / total.

    Both sides are compared with a branch-insensitive equality so that a
    register permutation inside a branch target is not counted as a mismatch;
    difflib keeps the alignment stable when a few instructions are merely
    rescheduled, which is what usually happens at the start of a function.
    """
    def eq(a: str, b: str) -> bool:
        if a == b:
            return True
        return is_branch(a) and is_branch(b) and a.split(" ", 1)[0] == b.split(" ", 1)[0]

    sm = difflib.SequenceMatcher(None, got, want, autojunk=False)
    same = 0
    problems: list[str] = []
    for tag, i1, i2, j1, j2 in sm.get_opcodes():
        matched = sum(1 for k in range(i2 - i1)
                      if i1 + k < len(got) and j1 + k < len(want) and eq(got[i1 + k], want[j1 + k]))
        same += matched
        if tag != "equal" or matched != i2 - i1:
            for k in range(i1, i2):
                problems.append(f"  + {got[k]}")
            for k in range(j1, j2):
                problems.append(f"  - {want[k]}")
    if problems and verbose:
        print(f"\n--- {name} ---")
        for p in problems[:120]:
            print(p)
    return same, max(len(got), len(want))


def main() -> int:
    args = [a for a in sys.argv[1:] if a not in ("-v", "--verbose")]
    verbose = len(args) != len(sys.argv) - 1
    if not args:
        print(__doc__, file=sys.stderr)
        return 2
    src, wanted = args[0], args[1:]
    hits = sorted((ROOT / "build/GP5E01_00/asm").rglob(Path(src).stem + ".s"))
    if not hits:
        print(f"no target asm for {src}", file=sys.stderr)
        return 2
    want = parse_target(hits[0].read_text(errors="replace"))
    out = subprocess.run([sys.executable, str(ROOT / "tools/mwccprobe.py"), src],
                         cwd=ROOT, capture_output=True, text=True)
    if out.returncode != 0:
        print(out.stdout + out.stderr, file=sys.stderr)
        return 1
    got = parse_candidate(out.stdout)
    if not got:
        print("candidate produced no code", file=sys.stderr)
        return 1

    total_same = total_all = 0
    for name in want:
        if wanted and name not in wanted:
            continue
        if name not in got:
            print(f"MISS  {name}")
            continue
        # a branch back to the function's own label is a loop back-edge; objdump
        # renders it as an address, doldecomp as the function name.
        want[name] = [f"{i.split(' ', 1)[0]} <t>" if i.split(' ', 1)[-1] == name else i
                      for i in want[name]]
        same, total = diff_function(name, got[name], want[name], verbose)
        pct = 100.0 * same / total if total else 100.0
        print(f"{'OK  ' if same == total else 'DIFF'} {name:<22} "
              f"{same:3d}/{total:<3d} {pct:6.2f}%  ({len(got[name])} insns)")
        total_same += same
        total_all += total
    if total_all:
        print(f"\nTOTAL {total_same}/{total_all} = {100.0 * total_same / total_all:.2f}%")
    return 0


if __name__ == "__main__":
    sys.exit(main())
