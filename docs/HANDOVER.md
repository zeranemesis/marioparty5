# MP5 decomp â€” HANDOVER

Read this first in a new session. Everything below is verified state, not plans.
Written 2026-10-04. Repo `G:\decomp\MP5`, branch `decomp/m524Dll`, HEAD `1cc89b2`. Local only, nothing pushed.

## 1. Objective and acceptance criterion

Decompile Mario Party 5 (GameCube, PowerPC, MWCC 1.3.2 / CodeWarrior) to a maximal verified match.

Criterion: `matched_code` 100 % + `matched_data` 100 % + 6635/6635 functions + 408/408 units + valid
SHA-1, except units proven inaccessible, which go in `docs/capped.md`.

## 2. Current verified metrics

From `build/GP5E01_00/report.json`, 881 units. Last full build ended `135 files OK`, and
`build\tools\dtk.exe shasum -q -c config\GP5E01_00\build.sha1` reported `135 files OK`.

| scope | code matched | data matched | linked |
|---|---|---|---|
| All | 2 679 184 / 9 481 492 (28.26 %) | 623 715 / 2 644 182 (23.59 %) | 346 / 881 |
| DOL | 1 478 600 / 1 730 512 (85.44 %) | 515 474 / 926 941 (55.61 %) | 232 / 307 |
| Modules | 1 200 584 / 7 750 980 (15.49 %) | 108 241 / 1 717 241 (6.30 %) | 114 / 574 |

Functions: 6447 / 21157. Session start was 27.99 % / 15.16 % / 6351 functions, so this session gained
roughly **+25 000 code bytes and +96 functions**. The mass of the remaining work is Modules: ~6.5 MB.

## 3. Environment constraints (will waste time if rediscovered)

- Shell is Windows PowerShell 5.1. **Forbidden in `bash` calls**: pipes `|`, semicolons `;`,
  redirections `>`, `&`, backticks, multi-line commands. Put anything non-trivial in a `.ps1` file and
  run it with `& "path\script.ps1" args...`.
- `git *` is denied except reads (`log`, `status`, `diff`, `branch --list`, `show`). **No** `merge`,
  `commit`, `add`, `checkout`, `reset`. All work is uncommitted and therefore perishable â€” integrate into
  the main checkout often rather than sitting on worktrees.
- `python ...` may require approval through `ask`.
- DOL units exist only in the main checkout, so DOL work is **serial**. REL can parallelise.
- `bash` calls need an explicit `workdir`; without it they run in `C:\Users\valen`.

## 4. Regenerating the build graph

Pass the tool **binaries**, never a directory â€” a directory is treated as a source tree and configure.py
fails looking for `Cargo.toml`:

```
python configure.py --dtk G:\decomp\MP5\build\tools\dtk.exe --objdiff G:\decomp\MP5\build\tools\objdiff-cli.exe --sjiswrap G:\decomp\MP5\build\tools\sjiswrap.exe --binutils G:\decomp\MP5\build\binutils --compilers G:\decomp\MP5\build\compilers
```

**Mandatory after adding a source file for a unit that had none.** `tools/project.py` gates both the ninja
compile edge and objdiff's `base_path` on `obj.src_path.exists()`, so without this the unit keeps
reporting `[no compiled source]` forever. `files.cmake` is only a progress report â€” not the build graph.

## 5. Integration protocol â€” never skip the verification

1. `tools\install_unit.ps1 <worktree> <module> <file>` copies a unit's source into the main checkout,
   creating the directory if needed and forcing `LastWriteTime`. Back up the previous version first if the
   unit already existed.
2. Re-run `configure.py` (above) if the unit is new.
3. `ninja -j 4` must end `135 files OK` with no `FAILED:`.
4. `python tools\unit_status.py <module>/REL/<module>/<file>` **in the main checkout** â€” compare
   per-function against main's current state, not just totals.
5. `build\tools\dtk.exe shasum -q -c config\GP5E01_00\build.sha1` must report `135 files OK`.

Only integrate when the main-checkout measurement is strictly better. If it is neutral (new bodies but no
bytes credited) it is still usually worth keeping, since real bodies are required by the final criterion â€”
but say so honestly rather than claiming a byte gain.

## 6. Dispatch protocol â€” scope narrowly or agents die

Eight agents have now died on **context exhaustion**, never on the decomp itself: several
"Compaction did not run: the model returned an empty summary", one "Tool calls cutoff by max_tokens",
one "Compaction exhausted after 3 attempts". Every one was a whole-unit dispatch on a large unit.

**Rule: name 3-5 specific functions, ascending by size. One file per agent** so two agents never produce
conflicting copies of the same source and integration stays a single clean copy. Prefer functions of
roughly 20-800 bytes.

A single 8332-byte function (`m518dll::fn_1_4B80`, 2083 instructions) killed a dedicated agent on its own.

`tools\fndiff.py` has `--max-diff N` (cap differing-row anchors) and `--rows START:END` (print a raw row
slice) precisely so large functions can be paged instead of flooding context.

**A dead agent is often not a total loss.** Two dead agents left integrable work; one was worth +1120 bytes
and +6 functions despite dying mid-task. Always run
`git diff --stat -- src include` in its worktree before discarding it, and measure before keeping it.

## 7. Tools available (`G:\decomp\MP5\tools\`)

- `unit_status.py <unit>` â€” the authoritative measurement. Reads `report.json`.
- `fndiff.py <unit> <symbol> [--max-diff N] [--rows A:B] [--context N] [--all]` â€” per-function disassembly diff.
- `next_targets.py --sort volume|cheap|finish`, `--unregistered`, `--unit NAME` â€” target picking, honours `POOL.md`.
- `install_unit.ps1 <worktree> <module> <file>` â€” install an agent's source into main.
- `prep_wt.ps1 <names...>` â€” make worktrees buildable (tools, build.ninja, orig hardlinks).
- `sync_tools.ps1` â€” **force**-overwrite `tools/*.py` into worktrees (see pitfall below).
- `cfg_wt.ps1 <names...>` â€” pre-run the correct configure.py in worktrees.
- `install_headers.ps1` â€” copy missing REL headers from `orig/foo/include/REL/`.

**Pitfall already hit twice:** `Copy-Item -Force` **preserves `LastWriteTime`**, so a copied source can look
older than the built `.o`; ninja then reports `no work to do` and you measure a stale object. Always force
the timestamp. Relatedly, `prep_wt.ps1` originally copied `tools/*.py` only when absent, leaving worktrees
with git-tracked **stale** copies where `fndiff.py` failed with "Unrecognized argument: -o". Fixed, and
`sync_tools.ps1` now overwrites.

`fndiff.py` must use objdiff **2.7.1** because it passes `-o`, which 3.x removed (a 3.x build answers
"Unrecognized argument: -o" and prints no diff at all). Do not overwrite `objdiff-cli.exe` â€” configure.py
needs 3.x to generate `report.json`.

## 8. Banked codegen facts (verified; reuse before inventing explanations)

- **fndiff's percentage is not objdiff's.** fndiff loses points on relocation-heavy rows. Real cases:
  `m515::fn_1_2EB4` printed 47.06 % with all 34 rows identical and 0 differing, while report.json says
  100.0. Also `fn_1_1FC` 92.86 %, `fn_1_C0D0` 90.62 %, `mdbeach::fn_1_28C8` 64.71 %. **If fndiff says 0
  differing and 0 label-only, the function is done.** `report.json` â†’ unit node â†’
  `functions[].fuzzy_match_percent` is the truth.
- **Locals map first-declared â†’ r31, then r30, r29, r28.** Declaring `ent, b94, b80, obj` in that order
  reproduces those registers exactly. (This is the same fact as "reverse declaration order" seen from the
  other side â€” use whichever phrasing, but verify empirically.)
- **Return-value hoisting.** A constant returned after a call must live in a local: `s32 ret = 1; f(); return ret;`
  emits `li r31,1 â€¦ mr r3,r31`. A bare `return 1;` emits `li r3,1` after the call and loses the
  callee-saved pair. Tell: a missing `stw`/`lwz` spill pair.
- **`s16` local with `s32` return type** emits a trailing `extsh r3,r31`; declare the function `s16`.
- **`void f(s32 a){ g(a); }`** reproduces `stwu -0x10 / mflr / stw r0,0x14 / stw r3,0x8 / lwz r3,0x8 / bl`
  exactly. That frame is `-O4` giving the parameter a stack home, not an `-O0` artifact.
- **MWCC inlines any callee whose body it has already compiled, emitting it first.** So a caller must be
  defined **above** its helpers or the helper is silently inlined in, with no compile error. Callers above
  helpers is why `mdparty.c` defines `MdInit`/`MdClose` after `MdMain`.
- **Spill pressure can run the wrong way.** In `m518::fn_1_6E08`/`fn_1_7908` the original *spills* values
  our build keeps in registers (target frame `-0xb0` vs ours `-0xa0`). The fix is to **increase** pressure â€”
  keep values live across calls, avoid narrowing types â€” not to reduce register use.
- **`switch` case set matters.** A `cmpwi 1/beq/bge/cmpwi 0/b/cmpwi 3/b` dispatch comes from the case set
  **{0,1,2}**; {0,1,3} makes MWCC emit a 3-level dense split over [0,4]. An explicit `case 0:` can be
  needed before MWCC roots a binary search at 2 instead of 3.
- **A `for` counter declared `s16`** forces an `extsh` pair and a wider `cmpwi`; the original uses `s32`.
- **Dead conjuncts**: MWCC evaluates the boolean at runtime via `subf`/`cntlzw`/`srwi`; deleting the dead
  side of `motNo == 5 && MOT == MOT_WIN` matched.
- **`s16`/`u16` field types**: a field loaded with `lha` must be `s16` (`s32` gives `lwz` and a different
  compare width); `sth` likewise.
- **Struct padding**: if a 16-bit field is consistently +2 versus the target, padding is the likely cause
  and explicit `u8 padN[...]` pinning is the fix â€” but **verify per struct**. `-align powerpc` is NOT
  universally "align s16 to 4": mgmatchDll showed 4-byte alignment, mdcardDll showed plain align 2 with six
  functions matching without fixups. Do not add padding to structs that already match.
- **`_savegpr_27` vs inline `stw r31..r28`** flips purely on whether r27 is preserved; one extra named
  `s32 ret;` local does it.
- **`const f32`/`f64` globals** get anonymous `@308`-style copies instead of a named relocation, producing
  label-only rows; dropping `const` fixes it.
- **`if (++sys->counter > 36000)`** keeps one `sys` load; split statements reload the global pointer and
  cost 4 instructions.
- **An empty function body is sometimes the correct body** â€” when the target is a single `blr`. Precedents:
  `REL/m504dll/light.c::fn_1_D108`, `m515::fn_1_1D20`, `mdbeach::fn_1_16DE8`. Verify in the diff first. A
  frame-only target (`stwu`/save `f31,r31,r30`/restore/`blr`, no LR save) is reproduced by self-assignment
  locals (`s32 x; x = x;`): MWCC allocates the registers then deletes the copy.
- **objdump prints single-bit `rlwinm` bit positions INVERTED** â€” `& 0x40` prints `25, 25`. Map through 31-n.
- **Recovering the original's unnamed constant tables**: grep `^\.obj lbl_1_rodata_` in the target `.s`. The
  target `.o` has zeroed section file-offsets so ELF reads fail. objdiff resolves static-data relocations
  **by address**, so `.bss` must be reconstructed in declaration order.
- A target using `addi r30,r3,0x16` + `0x0(r30)` wants a real named local pointer, not a folded `0x16(r3)`.
- `frandmod` must stay **undeclared**; typing it `u32` suppresses the `xoris r0,r0,0x8000` pair the implicit
  intâ†’float conversion needs.
- Explicit `(double)` casts break float matching: with them MWCC emits `fmul`/`fsub`+`frsp`, without them the
  correct single-precision `fmuls`/`fsubs`.

## REFUTED THIS SESSION - two "codegen walls" that were not walls

### 1. The magic-base int->float idiom IS reachable (see section 10 entry above)
### 2. `void *` + byte offsets cause spills; a named struct type does not

Writing `void fn_xxx(void *arg0)` and then `*(s16 *)((u8 *)arg0 + 0x184)` makes MWCC **spill
the pointer to the stack**. Writing a real type instead:

```c
typedef struct { u8 pad[0x184]; s16 modelId; } M508OBJ;
void fn_xxx(M508OBJ *arg0, s32 arg1) { Hu3DMotionTimeSet(arg0->modelId, ...); }
```

makes MWCC keep the pointer in a **callee-saved register** (`mr r31, r3`), which removes the
frame change and the `_savegpr_N` mismatch that had been logged as "register allocation
differs". Verified on m508Dll: `fn_1_8290` 33% -> 100%, `fn_1_7FE0` 52% -> 0 differing (!).

Apply this to every function hitting the "`_savegpr_26` vs `_savegpr_27`" or "obj in r28 vs r30"
wall BEFORE concluding it is an allocation problem.

## 9. Refuted â€” do not retry

- **Renaming static data symbols to objdiff's `lbl_1_data_*` changes nothing.** objdiff resolves symbols by
  address; the `~` label-only rows are cosmetic and cost no matched bytes. Tested: `m570Dll/player` stayed
  at exactly 4036/7592, 17/24, fuzzy 98.88 % across the rename. Reverted. Keep readable names.
- **`Equivalent` with a low `matched_code` is not fraud.** Units declared `Equivalent` can show 1.9-5.7 %
  byte-identity (e.g. `w04Dll/world04` 5.7 %, `w02Dll/world02` 1.9 %) because their sources are faithful
  transcriptions marked `NONMATCHING: complete and faithful` â€” semantically correct, not byte-identical.
  Low byte-identity is the *definition* of `Equivalent`. Check source size before accusing.
- `next_targets.py`'s `stat` column comes from `configure.py`, not objdiff (`configure_status()` at
  tools/next_targets.py:47-53), and its `pct` is only meaningful for `Matching`/`NonMatching` units.
- `promote_scan.py --modules` reporting `LINK IS BROKEN NOW: 27 modules` is a `sqrtf` false positive;
  `CHECK build.sha1` passes.

## 10. Open issues

- **objdiff `matched_code` only credits functions at/near 100 %.** A function at 90 % contributes 0 bytes,
  so partial work looks like nothing in the totals. Report partial progress as fuzzy %, not bytes.
- **`bankCoin` in `include/game/gamework.h` is 2 bytes late** for the padding reason, costing
  `mgmatch::ObjectSetup` its last row (99.95 %, 1 row). Deliberately NOT fixed: that header is shared
  widely, so 4 bytes in one function is not worth risking units that already match. Anyone taking it on
  must re-verify every unit including it.
- **object â†’ REL propagation.** After changing a source, ninja recompiles the object and objdiff re-measures,
  but no relink happens and `CHECK` never re-runs ("no work to do"). `m540Dll.preplf` does list the object
  and `m540Dll.rel` depends on the `.plf` plus `main.elf`, so `obj/REL/<mod>/<unit>.o` appears to be an
  objdiff-side object that is not what feeds the linked `.rel`. Forcing a fresh timestamp does not change it.
  So `CHECK` can pass against a stale `.rel`. Run the explicit shasum; treat objdiff metrics, not the link
  chain, as the working measure.
- **15 REL headers were missing** from `include/REL/` and have been added from `orig/foo/include/REL/`
  (m509Dll, m515Dll, m520Dll, m531Dll, m541Dll, m549Dll, m551Dll, m555Dll, mdbeachDll, mdminiDll, mdstoryDll,
  mgmatchDll, mgtourDll, sd00dll_stage, systemDll). Agents had been duplicating types inline because of
  this. Existing `.c` files were **not** refactored to use them, so nothing breaks â€” but that cleanup is
  available and would help future work.
- **Stray files that break worktrees**: `src/REL/m539Dll/drum.cpp` (parked at
  `C:\Users\valen\AppData\Local\Temp\kilo\stray_drum.cpp`; `extern FILE __files[];` is invalid under
  `-lang=c++`, no `drum.o` ever existed) and `src/REL/m523Dll/*` (parked under `...\kilo\m523Dll-blockers\`;
  includes `game/hu3dshadow.h` / `game/hu3dmodel.h` which exist nowhere in the repo). Neither is in main.
- `include/game/hsfex.h:23` declares `Hu3D3Dto2D`'s 2nd arg `s16`; the original uses `u16`.
- `Hu3DMotionMaxTimeGet` is called as `f32` in `m570::fn_1_58EC` and as `s32` in `fn_1_5E38`/`fn_1_5A78`, and
  MWCC rejects both declarations ("redeclared, was float now long"). `fn_1_58EC` still carries an intâ†’float
  widen plus a swapped `fcmpu`.
- **The A268 result mask**: target `clrlwi rX,r3,24` + `rlwinm r0,rX,0,25,25`; MWCC always narrows to 16. No
  source form found. Affects `m518::fn_1_83FC`, `fn_1_CEA0`.
- MWCC 1.3.2 **does not CSE** a field load across two compares, and declaring any local in such a function
  forces a frame the target does not have (`m551::fn_1_117B0`).
- `m518::fn_1_7768`/`fn_1_77F0`: address form and register allocation are mutually exclusive depending on the
  parameter type (`s16*`/`char*`/`u8*`/`void*` invert; only pointer-to-array does not). Parked at 99.12 %.
- `m515::M515_SUB` cannot be expressed as one flat struct â€” the target's 20-byte node array at +0x34c
  overlaps live fields at +0x3f8, +0x470, +0x47e..+0x486, +0x680, +0x684, +0x688, +0xcac. `fn_1_5C40` needs
  raw pointer arithmetic.
- ~~The **magic-base float idiom** is unsolved~~ â€” **REFUTED (verified empirically).** Compiling
  `(f64)v`, `(f32)(f64)v`, `double d = v;` and `return v;` with the project's own
  `mwcceppc 1.3.2` all emit `xoris rX, 0x8000` + `lis 0x4330` + `lfd` + `fsubs`, i.e. the exact
  idiom. **MWCC produces this naturally for any signed int -> double conversion**, so it is
  reachable from plain C and is *not* a codegen wall. Do not park functions on it.
  Method to check any idiom yourself, no objdiff needed:
    build\tools\sjiswrap.exe build\compilers\GC\1.3.2\mwcceppc.exe <flags from build.ninja> \
        -c scratch.c -o scratch.o
    build\tools\dtk.exe elf disasm scratch.o scratch.disasm.o
  (put sjiswrap args in a PowerShell array; commas in `-O4,p` break the command line).
  Still genuinely unsolved: the int *index* idiom on `m518::fn_1_7908`.

## 11. Units opened/integrated this session

Verified in main, all `135 files OK`. Most of these units previously had **no source at all** and reported
`[no compiled source]`; creating the file is what made them measurable, and each then matched its three
smallest functions on the first or second attempt.

| unit | result |
|---|---|
| `m570Dll/player` | 7/24 â†’ 17/24 fns, 1356 â†’ 4036 B (53.16 %) |
| `mdminiDll/mdmini` | none â†’ 16/58 fns, 2756 B |
| `m540Dll/m540` | 37.2 % â†’ 41.63 %, 183/226 fns |
| `m518dll/m518` | `fn_1_6E08` 74.10 %, `fn_1_7908` 62.58 % (neutral in bytes, fuzzy 76.6 â†’ 83.0) |
| `m515Dll/m515` | 3/84 â†’ 21/84 fns, 12 â†’ 1636 B |
| `m555Dll/game` | 3/43 â†’ 12/43 fns, 220 â†’ 3628 B |
| `mdcardDll/mdcard` | none â†’ 6/39 fns, 932 B |
| `mdbeachDll/mdbeach` | none â†’ 3/54 fns, 124 B |
| `m549Dll/game` | none â†’ 3/42 fns, 164 B |
| `m551Dll/game` | none â†’ 2/40 fns, 104 B |
| `sdroomdll/garage` | none â†’ 3/31 fns, 308 B |
| `sd00dll/stage` | none â†’ 3/71 fns, 100 B |
| `mgmatchDll/mgmatch` | none â†’ 3/93 fns, 60 B **plus +3640 data bytes** |
| `partyresultdll/result` | none â†’ 1/23 fns, 120 B |
| `m541Dll/game` | none â†’ 2/37 fns, 68 B |

**Strategic lesson, the single most useful thing here:** opening a closed unit beats refining an open one.
A unit with no source reports neither code nor data; three small leaf functions make it measurable and
they usually match almost immediately. Opening `mgmatchDll` also lifted **data** by 3640 bytes, and data was
the weakest criterion. Keep dispatching at unopened units â€” `next_targets.py --sort volume` lists them and
already excludes the pooled modules in `POOL.md`.

## 12. Best remaining targets

Unopened, from `next_targets.py --sort volume` (re-run it, the list shifts as units get sources):
`mdstoryDll/mdstory` 44 076, `mdoptionDll/mdoption` 43 628, `rescardDll/rescard` 43 232,
`m508Dll/m508` 41 364, `m520Dll/m520` 40 628, `m567Dll/m567` 39 852, `mgfreeDll/decide` 37 860,
`storymodedll/opening` 37 808, `carddll/cardmain` 36 552, `m527Dll/m527` 53 928 (16 fns only â€” attractive),
`m528Dll/m528` 52 628.

Huge single functions needing dedicated multi-session work with `--rows` paging: `m518::fn_1_4B80` 8332 B
(2083 instructions, untouched), `m540::fn_1_FFC` 7204, `m540::fn_1_2C74` 4260, `m527::fn_1_5B4` 18 916,
`m527::fn_1_4F98` 11 972.

## 13. Worktree state

62 worktrees under `G:\decomp\mp5-wt`. Ten were prepared and confirmed buildable, and their `tools/` are
force-synced: `storyresultdll`, `w04sDll`, `m571dll`, `m529dll`, `m504dll`, `m518dll`, `m523Dll`, `m531Dll`,
`m532Dll`, `m539Dll` (plus `m563dll`).

**Never copy a worktree's `configure.py`.** `w04sDll`'s carries a rejected `Object(Equivalent, ...)` on a
41.87 %-matched unit. Copy only the unit's source file.

`src/REL/m571dll/m571.c`, `include/REL/m504dll.h`, `src/REL/m504dll/com.c`, `src/REL/m538Dll/m538.c` and
similar are pre-existing dirty state in various worktrees â€” already verified byte-identical to main or
deliberately left alone. The 62-worktree scan found **no** unintegrated gold: `m529`, `m563`, `m571` were
already merged.

## 14. Rejected work â€” do not redo

- `w04sDll/world04`: wrote 5 empty stubs covering 24 804 bytes then declared the unit `Object(Equivalent)`.
  Zero matching progress. Its only salvageable nugget is a constant fix in `MB4_CoinDispUpdate`
  (`0x8C2D` â†’ `0x8C002D`), still on the shelf.
- `storyresultdll/rank`: empty report, no `src/` change at all.
- `m518dll/m518` as first submitted: its work was already in main and slightly **better** (`fn_1_83FC` at
  99.96 % vs its 99.36 %). Integrating it would have regressed the unit.
- `systemDll/nel` as first submitted: reported 3 function bodies but **no `nel.c` existed** in the worktree â€”
  a phantom report. Nothing to integrate.

## 15. Standing rules for agents

Never write an empty function body or a `// NONMATCHING:` stub in place of real logic. Never set
`Object(Equivalent, ...)` unless objdiff reports the unit fully matched. Never weaken code that already
matches. No commits, no mutating git commands. Touch only the assigned unit's source file; report a needed
header change instead of making it. An honest zero is useful; a fabricated number is not.

## 16. NEW verified rule: `li` vs `lis` for constants (project-wide ceiling)

Measured directly, compiling a throwaway file with the project's own flags
(`build\compilers\GC\1.3.2\mwcceppc.exe`, `-O4,p -inline auto ...`) and disassembling with
`build\tools\dtk.exe elf disasm`. Call `HuDataSelHeapReadNum(0x3C85, 0x1000, 2)`:

| constant | MWCC emits | retail binary emits |
| --- | --- | --- |
| `0x3C85` | `li r3, 0x3c85` | `lis r3, 0x3c` + `addi r3, 3, 0x85` |
| `0x1000` | `li r4, 0x1000` | `lis r4, 0x1000` |
| `0x2` | `li r5, 0x2` | `li r5, 0x2` |
| `0x3C8585` | `lis r3, 0x3d` + `subi r3, 3, 0x7a7b` | (same family) |
| `0x8000` | `lis r3, 0x1` + `addi r3, 3, -0x8000` | (same family) |

**The rule is exceptionless: MWCC emits `li` whenever the value fits in a signed 16-bit immediate, and
only reaches for `lis` when it does not.** The retail build instead uses `lis` as soon as the high 16
bits are non-zero.

Consequence: **any function passing a literal in `[0x1000, 0x7FFF]` cannot be matched from C**, because
MWCC has no spelling of that constant that avoids `li`. This is not a decompilation mistake and must not
be parked on as a "compiler wall" to be retried later. `m502dll/bat/fn_1_CB10` (4 calls x 2 such
constants) and `m539Dll/fn_1_2C94` (3 diffs out of 19, all of them this) are the confirmed cases.

Corollary that *is* actionable: it also explains why the 20+ already-matched functions contain **zero**
`lis` instructions. When hunting for a `lis`/`li` diff, first check the constant's magnitude.

To reproduce the experiment: the file must be under the repo root and passed as a **relative** path --
`dtk.exe` mangles absolute Windows paths into `/Users/...`. Build the flag array in PowerShell with
`@('-c','-nodefaults',...)`; passing the flags inline makes PowerShell parse `-inline auto` as a
parameter and fail.

## 17. NEW verified rule: `fcmpo` + `cror eq,gt,eq` means `>=`, not `==`

`m502dll/bat/fn_1_DE94` (68 bytes, now matched) pins this down. The retail sequence for the second
bound is:

```
fcmpo cr0, f1, f0
cror eq, gt, eq
bne  <return 1>
```

`cror eq, gt, eq` rewrites the `eq` bit as "gt OR eq", i.e. "not less-than", so the branch is testing
`f1 >= f0`. Written as `==` or `<=` in C, MWCC emits `fcmpu` and drops the `cror`, and the function
lands at 52-58 %. Written as `>=`, it is byte-exact.

So: **when you see `fcmpo` followed by `cror eq,gt,eq`, the C operator is `>=` (or the negation of
`<=`).** The symmetric `cror eq,lt,eq` is the `<=` shape.

The other half of that function: the two bounds are **rodata symbols**, not literals (`lbl_1_rodata_4B0`,
`lbl_1_rodata_580`). Literals cost ~40 points. And the two tests must share one `return 0` block, which
means a single `if (a || b)` rather than two early-return `if`s -- separate `if`s produce a `bge` over the
shared block instead of the retail `blt` into it.

## 18. `stfd` on f31 does not imply a `double` parameter

`m539Dll/fn_1_2FCC` saves f31 with `stfd` and moves it with `fmr f1, f31`, which reads like a
double-typed scale. It is not: the parameter is `f32`, and `Hu3DModelScaleSet` takes `f32`. Writing
`double scale` with `(f32)` casts costs 45 % (frsp after every conversion); writing `f32` is exact.
MWCC parks a float in the double-width half of f31 when it must survive a GPR op, and saves all 64 bits.

## 19. NEW verified rule: `srwi` vs `srawi`, `cmplwi` vs `cmpwi` means cast to `u32`

`m539Dll/fn_1_36CC` (116 bytes, now matched) sat at 93.10 % with exactly **two** instructions
differing, and both were signedness:

```
target: srwi   r0, r0, 30          mine: srawi  r0, r0, 30
target: cmplwi r0, 0x270           mine: cmpwi  r0, 0x270
```

MWCC picks the *arithmetic* form by default for `>>` on a signed type and for a signed bound test.
Casting the operand to `u32` switches both to the logical form and takes the function to 100 %:

```c
while ((u32)r->count < 0x270) {
    r->v[r->count] = r->count +
        (((u32)r->v[r->count - 1] ^ ((u32)r->v[r->count - 1] >> 30)) * 0x6C078965);
    r->count++;
}
```

So: **when a lone diff is `srawi`/`srwi` or `cmpwi`/`cmplwi`, do not go looking for a code-generation
wall â€” try the unsigned cast first.** It is a one-token change and it was worth a full function.

## 20. Reading a field: scalars vs pointers changes the instruction, not just the type

`m539Dll/fn_1_1270` is byte-for-byte the same six-instruction shuffle as `fn_1_6324`, and I got
77.78 % with both words typed `s32`. The target does:

```
lwz r0, 0x4(r31)      r0 = self->f04
lwz r3, 0x0(r31)      r3 = self->f00
stw r0, 0x4(r3)       <-- stores THROUGH the value of f00
```

The store destination is `r3 + 4`, not `r31 + 4`. That is only coherent if `f00` holds a **pointer to
another node**, so the type is `struct node *f00, *f04;` and the source is `self->f00->f04 = self->f04`.
Re-typing the pair as node pointers took the function to 88.89 % with no other change.

Generalisation: **when the target's store offset is computed from a register you have just loaded, the
field you read is a pointer, not a scalar.** Check the store base register before deciding a field's type.
