"""Find (and with --apply, rewrite) statements of the game code that draw the RNG more than once with no sequence
point between the draws: argument and operand evaluation order is unspecified in C, and MSVC (right to left for
call arguments) and clang (left to right) differ, so the same draws land in different places on a PC and a phone
(a netplay desync). The rewrite draws left to right into temporaries, under TARGET_PC (the GameCube build keeps the
original).

"RNG" covers the game's generators (frand, frandmod, rand8, the RandF macros) and, per module, every non-void function
that calls one of them or takes a *Rand state (mini-games with their own generator), to a fixpoint.

usage: python tools/netplay_rngorder.py [-v] [--apply]
"""
import glob
import re
import sys

BASE_RNG = {
    "MBCapsuleEffRandF": "float", "MBEffRandF": "float", "MBEffSRandF": "float",
    "randomMantissa__7CRandomFv": "float", "frand": "u32", "frandmod": "u32", "rand8": "int",
}
DECL = re.compile(r"^\s*(static\s+|const\s+)*(s8|u8|s16|u16|s32|u32|f32|f64|float|double|int|HuVecF|Vec|BOOL)\b")
FUNC = re.compile(
    r"(?m)^[ \t]*((?:[A-Za-z_]\w*[ \t\*]+)+?)([A-Za-z_]\w*)\s*\(([^;{}()]*(?:\([^()]*\)[^;{}()]*)*)\)\s*\{")
KEYWORDS = {"if", "for", "while", "switch", "return", "else", "do", "sizeof"}


def blank_comments(s):
    out = list(s)
    for m in re.finditer(r"/\*.*?\*/|//[^\n]*", s, flags=re.S):
        for i in range(m.start(), m.end()):
            if out[i] != "\n":
                out[i] = " "
    return "".join(out)


def call_span(s, start):
    i = s.index("(", start)
    depth = 0
    for j in range(i, len(s)):
        if s[j] == "(":
            depth += 1
        elif s[j] == ")":
            depth -= 1
            if depth == 0:
                return start, j + 1
    raise ValueError


def module_of(path):
    parts = path.replace("\\", "/").split("/")
    return "/".join(parts[:3]) if len(parts) > 2 and parts[1] == "REL" else "game"


def discover(files):
    defs = {}
    for f in files:
        try:
            code = blank_comments(open(f, encoding="utf-8", errors="replace").read())
        except OSError:
            continue
        mod = module_of(f)
        for m in FUNC.finditer(code):
            ret, name, params = m.group(1).strip(), m.group(2), m.group(3)
            if name in KEYWORDS or any(k in ret.split() for k in KEYWORDS) or ret.endswith("*"):
                continue
            rtype = " ".join(w for w in ret.split() if w not in ("static", "inline", "extern", "M_PC_C"))
            if not rtype or rtype == "void" or rtype.startswith("typedef"):
                continue
            start = m.end() - 1
            depth, i = 0, start
            while i < len(code):
                if code[i] == "{":
                    depth += 1
                elif code[i] == "}":
                    depth -= 1
                    if depth == 0:
                        break
                i += 1
            defs.setdefault(mod, {})[name] = (rtype, code[start:i], "Rand" in params)
    found = {}
    for mod in sorted(defs, key=lambda m: m != "game"):  # the game's own first: modules call them
        rng = {}
        known = dict(BASE_RNG)
        known.update(found.get("game", {}))
        changed = True
        while changed:
            changed = False
            pat = re.compile(r"\b(" + "|".join(sorted(set(known) | set(rng), key=len, reverse=True)) + r")\s*\(")
            for name, (rtype, body, rand_param) in defs[mod].items():
                if name in rng or name in known:
                    continue
                if rand_param or pat.search(body):
                    rng[name] = rtype
                    changed = True
        found[mod] = rng
    return found


def scan(path, rng, apply):
    token = re.compile(r"\b(" + "|".join(sorted(rng, key=len, reverse=True)) + r")\s*\(")
    src = open(path, encoding="utf-8", newline="").read()
    code = blank_comments(src)
    edits, manual = [], []
    for m in re.finditer(r"[^;{}]*", code):
        st = m.group(0)
        calls = [(t.start() + m.start(), t.group(1)) for t in token.finditer(st)]
        if len(calls) < 2:
            continue
        stripped = st.strip()
        if stripped.startswith("#") or "#define" in st or re.search(r"&&|\|\||\?", st):
            continue
        if re.match(r"\s*(?:[\w\*]+\s+)+\w+\s*\([^)]*\)\s*$", st):  # a prototype or definition head
            continue
        spans = [(call_span(code, pos), name) for pos, name in calls]
        if any(a[0][0] < b[0][0] < a[0][1] for a in spans for b in spans if a is not b):
            continue  # nested: the inner call is necessarily first
        line = src.count("\n", 0, m.start() + len(st) - len(st.lstrip())) + 1
        end = m.end()
        if end >= len(code) or code[end] != ";" or DECL.match(st) or re.match(r"\s*(if|switch|while|for)\b", st):
            manual.append((line, " ".join(stripped.split())[:120]))
            continue
        first = m.start() + len(st) - len(st.lstrip())
        edits.append((first, end + 1, spans, line))
    if apply and edits:
        nl = "\r\n" if "\r\n" in src else "\n"
        for first, end, spans, line in reversed(edits):
            line_start = src.rfind("\n", 0, first) + 1
            indent = src[line_start:first]
            if indent.strip():
                manual.append((line, "statement does not start its line"))
                continue
            original = src[first:end]
            body = original
            for k, ((a, b), name) in reversed(list(enumerate(spans))):
                body = body[: a - first] + f"rng{k}" + body[b - first:]
            temps = " ".join(f"{rng[name]} rng{k} = {src[a:b]};" for k, ((a, b), name) in enumerate(spans))
            text = (f"#ifdef TARGET_PC{nl}"
                    f"{indent}{{ /* netplay: C leaves the evaluation order open, MSVC and clang differ: draw left to right */{nl}"
                    f"{indent}    {temps}{nl}"
                    f"{indent}    {' '.join(body.split())}{nl}"
                    f"{indent}}}{nl}#else{nl}{indent}{original}{nl}#endif")
            src = src[:line_start] + text + src[end:]
        open(path, "w", encoding="utf-8", newline="").write(src)
    return edits, manual


def main():
    apply = "--apply" in sys.argv
    import os
    files = [f for ext in ("c", "cpp", "inc", "h") for f in glob.glob(f"src/**/*.{ext}", recursive=True)
             if os.path.isfile(f) and not f.replace("\\", "/").startswith("src/port") and "mainupdate.c" not in f]
    found = discover(files)
    total = 0
    for f in sorted(x for x in files if not x.endswith(".h")):
        rng = dict(BASE_RNG)
        rng.update(found.get("game", {}))
        rng.update(found.get(module_of(f), {}))
        try:
            edits, manual = scan(f, rng, apply)
        except (UnicodeDecodeError, ValueError):
            continue
        for e in edits:
            total += 1
            print(f"fix    {f}:{e[3]}")
        for line, text in manual:
            print(f"MANUAL {f}:{line}: {text}")
    if "-v" in sys.argv:
        for mod, rng in sorted(found.items()):
            if rng:
                print(f"rng {mod}: {' '.join(sorted(rng))}")
    print("rewritten" if apply else "to rewrite", total)


if __name__ == "__main__":
    main()
