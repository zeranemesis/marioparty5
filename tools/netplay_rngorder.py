"""Find (and with --apply, rewrite) statements of the game code that draw the RNG more than once with no sequence
point between the draws: argument and operand evaluation order is unspecified in C, MSVC and clang differ, so the
same draws land in different places on a PC and a phone. The rewrite draws left to right into temporaries, under
TARGET_PC (the GameCube build keeps the original)."""
import glob
import re
import sys

RNG = {
    "MBCapsuleEffRandF": "float", "MBEffRandF": "float", "MBEffSRandF": "float",
    "randomMantissa__7CRandomFv": "float", "frand": "u32", "frandmod": "u32", "rand8": "int",
}
TOKEN = re.compile(r"\b(" + "|".join(RNG) + r")\s*\(")
DECL = re.compile(r"^\s*(static\s+|const\s+)*(s8|u8|s16|u16|s32|u32|f32|f64|float|double|int|HuVecF|Vec|BOOL)\b")


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


def scan(path, apply):
    src = open(path, encoding="utf-8", newline="").read()
    code = blank_comments(src)
    edits, manual = [], []
    for m in re.finditer(r"[^;{}]*", code):
        st = m.group(0)
        calls = [(t.start() + m.start(), t.group(1)) for t in TOKEN.finditer(st)]
        if len(calls) < 2:
            continue
        stripped = st.strip()
        if stripped.startswith("#") or "#define" in st or re.search(r"&&|\|\||\?", st):
            continue
        spans = [(call_span(code, pos), name) for pos, name in calls]
        nested = any(a[0][0] < b[0][0] < a[0][1] for a in spans for b in spans if a is not b)
        line = src.count("\n", 0, m.start() + len(st) - len(st.lstrip())) + 1
        if nested:
            continue
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
            temps = " ".join(f"{RNG[name]} rng{k} = {src[a:b]};" for k, ((a, b), name) in enumerate(spans))
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
    files = [f for ext in ("c", "cpp", "inc") for f in glob.glob(f"src/**/*.{ext}", recursive=True)
             if not f.replace("\\", "/").startswith("src/port") and "mainupdate.c" not in f]
    total = 0
    for f in sorted(files):
        try:
            edits, manual = scan(f, apply)
        except UnicodeDecodeError:
            continue
        for e in edits:
            total += 1
            print(f"fix    {f}:{e[3]}")
        for line, text in manual:
            print(f"MANUAL {f}:{line}: {text}")
    print("rewritten" if apply else "to rewrite", total)


if __name__ == "__main__":
    main()
