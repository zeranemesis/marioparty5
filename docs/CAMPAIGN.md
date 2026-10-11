# MP5 decomp — 10-agent campaign

Started 2026-10-04. Branch `decomp/m524Dll`, HEAD `1cc89b2`. Local only, nothing pushed.

This file records the parallel-campaign layout. The decompilation *method* is in
[decomp_workflow.md](decomp_workflow.md); verified project state and banked codegen facts are in
[HANDOVER.md](HANDOVER.md) — read that first, it is still authoritative. This file only covers what
is specific to running ten agents at once.

## Baseline at campaign start

`ninja` green, `dtk shasum -q -c config\GP5E01_00\build.sha1` → `135 files OK`.

| scope | code matched | data matched | linked |
|---|---|---|---|
| All | 2 679 184 / 9 481 492 (28.26 %) | 623 715 / 2 644 182 (23.59 %) | 346 / 881 |
| DOL | 1 478 600 / 1 730 512 (85.44 %) | 515 474 / 926 941 (55.61 %) | 232 / 307 |
| Modules | 1 200 584 / 7 750 980 (15.49 %) | 108 241 / 1 717 241 (6.30 %) | 114 / 574 |

Functions: 6447 / 21157.

## Why these ten units

HANDOVER §11: **opening a closed unit beats refining an open one.** A unit with no source file
reports neither code nor data, so it contributes nothing to any total while costing nothing to
look at. All ten targets below had **no source file at all** when they were dispatched. Together
they hold 663 748 bytes of code that was previously invisible to every progress number, which is
26× the entire byte gain of the previous session.

`tools\fnlist.py --units N` ranks the unopened units by missing bytes; that is where the list came
from.

| slot | worktree | unit | bytes opened | functions assigned |
|---|---|---|---|---|
| slot01 | `mp5-wt\slot01` | `systemDll/REL/systemDll/nel` | 116 136 | 6 smallest |
| slot02 | `mp5-wt\slot02` | `carddll/REL/carddll/status` | 104 648 | 6 smallest |
| slot03 | `mp5-wt\slot03` | `mgwarsDll/REL/mgwarsDll/mgwars` | 58 064 | 6 smallest |
| slot04 | `mp5-wt\slot04` | `m527Dll/REL/m527Dll/m527` | 53 928 | 5 smallest |
| slot05 | `mp5-wt\slot05` | `m528Dll/REL/m528Dll/m528` | 52 628 | 6 smallest |
| slot06 | `mp5-wt\slot06` | `m535Dll/REL/m535Dll/m535` | 48 856 | 5 smallest |
| slot07 | `mp5-wt\slot07` | `m543Dll/REL/m543Dll/game` | 48 552 | 5 smallest |
| slot08 | `mp5-wt\slot08` | `mdstoryDll/REL/mdstoryDll/mdstory` | 44 076 | 5 smallest |
| slot09 | `mp5-wt\slot09` | `mdoptionDll/REL/mdoptionDll/mdoption` | 43 628 | 4 smallest |
| slot10 | `mp5-wt\slot10` | `rescardDll/REL/rescardDll/rescard` | 43 232 | 5 smallest |

Two of the ten modules have a header on the include path (`systemDll.h`, `mdstoryDll.h`); the other
eight do not, so their agents declare types locally in the unit's own `.c` and report every
declaration they invent. That is the existing status quo in this repo, not a regression.

## Why one worktree per agent

Two `ninja` processes in one build directory race on `.ninja_log` / `.ninja_deps` and on object
writes, and one agent editing a file another agent is also editing makes integration ambiguous. So:
one worktree, one source file, one agent. DOL units are the exception — they exist only in the main
checkout, so DOL work stays serial.

### What a slot worktree contains

`tools\..\..\Temp\kilo\mk_slots.ps1` builds it. The important decisions:

- **Read-only inputs are hard links** — `orig/` (2.54 GB), `build\tools`, `build\compilers`,
  `build\binutils`. Ten slots cost almost no disk.
- **Build outputs are real copies**, `build\GP5E01_00` (0.22 GB). This is deliberate and must not be
  "optimised" into hard links: a compiler that opens its output with `O_TRUNC` writes *through* a
  hard link, so a linked `.o` would be silently overwritten in the main checkout. 0.22 GB a slot is
  cheap; a corrupted main build is not.
- **`.ninja_log` and `.ninja_deps` are copied.** Without them ninja does not know the recorded
  command hash for any output and rebuilds all 881 objects. With them a fresh slot's `ninja` is a
  no-op and reports exactly the main checkout's numbers.
- **main's `src/`, `include/`, `config/`, `configure.py`, `tools/` are copied over the git
  checkout**, preserving `LastWriteTime`. The worktree therefore starts from main's *working* state
  including files that are untracked in git (15 REL headers, several unit sources), not from HEAD.
  Consequence: `git status` inside a slot shows those pre-existing differences. They are not the
  agent's edits, and an agent that reports them as dirty state is correct, not confused.

## Dispatch rules (learned the hard way — HANDOVER §6)

Eight agents have died of context exhaustion, never on the decomp problem: whole-unit dispatches on
large units, one 8332-byte function alone was enough to kill a dedicated agent.

- Name **3-6 specific functions, ascending by size.** Never a whole unit.
- Explicitly list the giants the agent must **not** attempt.
- One source file per agent, so integration is always a single clean copy.
- Cap `fndiff` anchors with `--max-diff 40`; page large functions with `--rows START:END`.

## Integration procedure (validated on slot03)

`Temp\kilo\integrate.ps1 -Slot <slot> -Module <mod> -File <file> -ModuleFilter <unit>` does all of
it and prints a verdict. The steps, and why each is mandatory:

1. Copy the unit's source into main and **force a fresh `LastWriteTime`**. `Copy-Item -Force`
   preserves the source timestamp, so a copy can look older than the built `.o`; ninja then reports
   "no work to do" and the measurement describes a stale object. This has already caused one wrong
   "before" number.
2. Re-run `configure.py` if the unit is new to main. The generator gates both the compile edge and
   objdiff's `base_path` on the source file existing; without it the unit reports
   `[no compiled source]` forever no matter what the agent wrote. Pass **binaries**, never the tool
   directories.
3. `ninja -j 8` must end with no `FAILED:`.
4. `build\tools\dtk.exe shasum -q -c config\GP5E01_00\build.sha1` → `135 files OK`. Run it
   explicitly: the ninja `CHECK` edge can pass against a stale `.rel` because no relink happens when
   only a source changed (HANDOVER §10).
5. `python tools\unit_status.py <unit>` **in the main checkout**, compared per function against
   main's previous state. Never trust an agent's self-reported baseline.

Only integrate when the main-checkout measurement is strictly better. If it is neutral — new bodies
but no bytes credited — it is usually still worth keeping, because the final criterion needs real
bodies; but say so honestly rather than claiming a byte gain.

### Integration log

Main checkout totals after each row. Baseline at campaign start: 2 679 184 B / 6447 functions /
623 715 data B, `135 files OK`.

| # | slot | unit | main before | main after | sha | verdict |
|---|---|---|---|---|---|---|
| 1 | slot03 | `mgwarsDll/mgwars` | 0 B, 0/93 fns | 180 B, 8/93 fns | `135 files OK` | kept, +180 B +8 fns |
| 2 | slot05 | `m528Dll/m528` | 0 B, 0/103 fns | 360 B, 9/103 fns | `135 files OK` | kept, +360 B +9 fns, +672 data B |
| 3 | slot07 | `m543Dll/game` | 0 B, 0/30 fns | 596 B, 7/30 fns | `135 files OK` | kept, +596 B +7 fns |
| 4 | slot08 | `mdstoryDll/mdstory` | 0 B, 0/43 fns | 556 B, 8/43 fns | `135 files OK` | kept, +556 B +8 fns |
| 5 | slot02 | `carddll/status` | 0 B, 0/59 fns | 268 B, 8/59 fns | `135 files OK` | kept, +268 B +8 fns |
| 6 | slot04 | `m527Dll/m527` | 0 B, 0/16 fns | 748 B, 6/16 fns | `135 files OK` | kept, +748 B +6 fns |
| 7 | slot01 | `systemDll/nel` | 0 B, 0/159 fns | 196 B, 4/159 fns | `135 files OK` | kept, +196 B +4 fns — **salvaged from a dead agent** |
| 8 | slot10 | `rescardDll/rescard` | 0 B, 0/44 fns | 556 B, 6/44 fns | `135 files OK` | kept, +556 B +6 fns — agent died before reporting |
| 9 | slot03 | `mgwarsDll/mgwars` (2nd) | 180 B, 8 fns | 564 B, 16/93 fns | `135 files OK` | kept, +384 B +8 fns |
| 10 | slot09 | `mdoptionDll/mdoption` | 0 B, 0/24 fns | 908 B, 6/24 fns | `135 files OK` | kept, +908 B +6 fns |
| 11 | slot04 | `m567Dll/m567` (new unit) | 0 B, 0/55 fns | 232 B, 6/55 fns | `135 files OK` | kept, +232 B +6 fns |
| 12 | slot01 | `systemDll/nel` (2nd) | 196 B, 4 fns | 548 B, 9/159 fns | `135 files OK` | kept, +352 B +5 fns |

Running total: **2 684 520 B, 6528 functions**, data 624 387 B (23.61 %). Session gain:
**+5336 code bytes, +81 functions**. `135 files OK` has held through all twelve integrations, and
nine units that previously reported *nothing at all* are now measurable.

### Two agents failed to report, and both still had value

This is the HANDOVER §6 rule working as intended, so it is worth stating plainly:

- **slot01 (`systemDll/nel`) died of context exhaustion** — "Compaction did not run: the model
  returned an empty summary", the same failure that killed eight agents last session. `nel` is the
  largest unit in the project (116 KB, 159 functions) and it had been given six functions.
  `Temp\kilo\measure_wt.ps1` measured its worktree without touching main: 4 functions at 100 %,
  196 B. Installed, verified, and the slot was refilled with **three** functions instead of six.
- **slot10 (`rescardDll/rescard`) completed with an empty report.** No handoff text at all.
  Measuring its worktree showed 6 functions at 100 %, 556 B, which was real and was integrated.
  The refill agent was told explicitly that it has no predecessor notes and must read the existing
  file as the only record.

Never discard a dead agent's worktree on the strength of the failure alone. Measure it first.

### Slot retirement: when a unit runs out of small functions

slot04's `m527Dll/m527` was down to `fn_1_A0` at 1300 B as its smallest remaining function, with two
monsters at 12 KB and 19 KB. Forcing a 1300-byte dispatch would have gambled a slot on the exact
failure mode that has killed agents all session. Instead the slot was **retired onto a fresh
unopened unit** (`m567Dll/m567`, 39852 B / 55 functions) and the file the new agent inherited was
explicitly declared off-limits. Worktrees are just build sandboxes; nothing requires a slot to stay
on the same unit.

### Codegen facts established by agents this session

Beyond the banked facts in HANDOVER §8, these are new and verified:

- **`fndiff` 2.7.1 flags a row as differing when the callee is undefined in our object but defined
  in the target**, even though relocation type and target name are identical. `lis/addi
  <undefined-in-ours>` therefore reads as a difference, while the two `bl <undefined extern>` rows do
  not — which is why one function read 100 % and its three siblings read 86.67 %. objdiff 3.x, which
  produces `report.json` and therefore every number that counts, is relocation-aware and scores them
  all 100.00 %. **An 86.67 % with 0 differing and 0 label-only is DONE.** Do not spend an iteration
  on it.
- **REL units compile `-O0,p`** (`configure.py:303`), the trailing flag overriding the earlier `-O4,p`.
  So MWCC does not CSE: four separate byte stores emit four `lis`/`addi` pairs, and the single hoisted
  pair the target has only appears when the copy is written as one 4-byte struct assignment. A local
  pointer variable is wrong there — it lands in r31 and forces a frame the target does not have.
- **A load into a local that is never read is often kept**, and reproducing it needs the local:
  `s32 oldParam = actorP->param;` matches a stack store that reads like dead code.
- **A small `void` function stuck at 70-95 % is usually a missing frame, not wrong logic.** In `nel`,
  two functions at 91 % and 75 % were closed by adding one `NEL_SYS *sys = &sysWork;` local, which
  forced r31 to be saved and restored and the frame to 0x20 — the target stored to `0x8(r1)` and never
  reloaded r3 before `blr`. Diagnostic: a missing `stw`/`lwz` pair.
- **For an `s32`-returning function whose local holds an s16 value, the local must be `u32`**: with
  `s32` MWCC drops the dead `li r31,0`; an `s16` local defers the `extsh` to the return and mismatches.
- **`while (frames--) { f(); }` matches where `while (frames) { f(); frames--; }` does not** — the
  latter matched all but 3 rows because the compiler fused the compare after the decrement, and a
  separate induction variable spilled r3 to a 0x20 frame.
- **MWCC rejects implicit `struct (*)[1]` → `struct *`**; use `&array[0]`, not `&array`.
- **`ParticleGlowKill` / `ParticleDotKill` / `ParticleGlowPosSet` are `int parManId[3]`** — 4-byte
  slots holding sign-extended s16 ids at offsets 0/4/8. Independently confirmed in mdstory, m543, nel
  and rescard; each matched at 100 % on the first attempt in every unit.
- **`_prolog` / `_epilog` are the MWCC crt stubs**: walk `_ctors` / `_dtors` calling each entry until
  NULL, and `_prolog` then calls `ObjectSetup` and returns 0. Matched at 100 % first try in m527,
  m528, m535, m543, mdoption, nel, m567 and rescard. The `VoidFunc` typedef plus
  `extern const VoidFunc _ctors[]; _dtors[]` declarations are the project-wide convention.

### Refused work — recorded so it is not redone

- `mdoptionDll/mdoption`: **`MdMain` (180 B) is blocked on `MdFlowTbl`**, a 6-entry `.data` table at
  `.data+0x00` per `splits.txt` holding six `MD_FUNC` relocations. Candidate members are `fn_1_B54,
  fn_1_CBC, fn_1_133C, fn_1_1BD8, fn_1_2160, fn_1_2D20` but the order is unverified. Verification path:
  `objdump -r` on the original object, or read the six relocation entries in address order from
  `.data` in the target `.s`. A wrong table poisons the unit's `.data` and misleads everyone after.
- `mdoptionDll/mdoption`: **`fn_1_A514` (240 B) is blocked on two `.rodata` doubles at `0xA0`/`0xB0`**
  used by an `atan2` result division. Values not recoverable from the worktree. Verification path:
  grep `^\.obj lbl_1_rodata_` in the target `.s`, or read the pool with `objdump -s` — noting the
  handover's warning that the target `.o` has zeroed section file-offsets so ELF reads can fail.
- `systemDll/nel`: **`ObjectSetup` (1048 B)** is declared locally, called from `_prolog`, and still
  undefined in `nel.o`. Harmless for the link (the module's other object supplies it) but it is the
  largest outstanding gap in that unit.

## Shared-header fix: `include/REL/mdstoryDll.h`

Found by the slot08 agent, confirmed and applied by the integrator. Two changes, both verified
zero-blast-radius *before* being made — the lesson from HANDOVER §10, where a 4-byte header change
was deliberately left unfixed because the header was widely shared.

1. **The header did not compile at all.** `#define MDSTORY_PLAYERSPR_MAX 1` (line 41) and
   `#define MDSTORY_MODELWORK_MAX 7` (line 42) expand the *enumerator names* at lines 140 and 149
   into the literals `1` and `7`, and MWCC rejects a macro-expanded enumerator
   (`error: identifier expected`). Renamed to `MDSTORY_PLAYERSPR_NUM` / `MDSTORY_MODELWORK_NUM`.
   The values 1 and 7 were already correct, and only the names collided — adding a trailing comma
   does not help, MWCC still rejects `1` as an enumerator name.
   *Verified first:* both macro names occur nowhere else in the tree except the two copies of this
   header, and no source file included `mdstoryDll.h` yet. Renaming could not break a matching unit.
2. **`MDDOT_WORK.man` was the wrong width.** `HU3DPARMANID` is `s16`, so `man[3]` is 6 bytes, but
   the real object is `HuMemDirectMallocNum(0, 0xc, 0x1000)` with `extsh`-widened ids stored by
   `stw` at 0/4/8 — 12 bytes. Changed to `s32 man[3]`, which is what `mdbeachDll.h` already had.
   *Verified first:* `MDDOT_WORK` is referenced nowhere outside these two headers.

## Environment gotcha: empty submodules in a fresh worktree

`git worktree add` leaves submodule directories empty, so `extern/musyx/include` was missing in
every slot and any unit reaching `include/msm.h` → `musyx/musyx.h` failed to compile. The slot08
agent hit this and fixed it locally; `Temp\kilo\link_extern.ps1` now hard links `extern/` into all
ten slots (400 files each). Hard links are safe: submodules are read-only inputs. This is now part
of slot preparation and was missing from `mk_slots.ps1`.

