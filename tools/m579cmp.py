# usage: python tools/m579cmp.py [compiled.o] : per-function opcode similarity vs original m579.o (default: ninja-built m579.o)
import difflib, re, subprocess, sys, os
OD="G:/decomp/MP5/build/binutils/powerpc-eabi-objdump.exe"
def funcs(path):
    out=subprocess.run([OD,"-d","--no-show-raw-insn",path],capture_output=True,text=True).stdout
    res={};cur=None
    for l in out.splitlines():
        m=re.match(r"^[0-9a-f]+ <(\S+)>:",l)
        if m: cur=m.group(1);res[cur]=[];continue
        m=re.match(r"^\s+[0-9a-f]+:\s+(\S+)",l)
        if m and cur: res[cur].append(m.group(1))
    return res
root=os.path.dirname(os.path.dirname(os.path.abspath(__file__))).replace("\\","/")
orig=funcs(root+"/build/GP5E01_00/m579Dll/obj/REL/m579Dll/m579.o")
mine=funcs(sys.argv[1] if len(sys.argv)>1 else root+"/build/GP5E01_00/src/REL/m579Dll/m579.o")
for n,a in orig.items():
    if n not in mine: print("%-14s MISSING"%n); continue
    r=difflib.SequenceMatcher(None,a,mine[n],autojunk=False).ratio()
    print("%-14s %.2f (%d vs %d)"%(n,r,len(a),len(mine[n])))
