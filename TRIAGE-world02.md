# w02sDll/REL/w02sDll/world02 — triage

Measured 2026-10-03 in worktree `G:\decomp\mp5-wt\w02sDll`, branch
`decomp/w02sDll`, object `build/GP5E01_00/src/REL/w02sDll/world02.o` against
`build/GP5E01_00/w02sDll/obj/REL/w02sDll/world02.o`.

## The headline: the unit is a different build of the same board, not a typo

`src/REL/w02sDll/world02.c` was a first-draft m2c port of **w02Dll's** world02
object, never verified against w02sDll's own target. Its own header comment says
`w02Dll - "Rainbow Dream" board`. That is not a near-miss; the two objects
disagree on which functions exist:

| | w02sDll/world02.o (target) | w02Dll/world02.o (target) |
|---|---|---|
| `.text` | 63 functions, 42440 B | 77 functions, 56532 B |
| only here | `MB2Ev_HatenaTaiyo`, `MB2_CoinExec`, `fn_1_42FC`, `fn_1_5260`, `MB2_ModelObjPtrGet` | `MB2Ev_WindGuru2`, `MB2Ev_Thunder`, `MB2Ev_Rain`, `MB2Ev_Zen2`, `MB2Ev_Zen4`, `MB2_TimerObjUpdate`, `MB2_DentyuObjFlash`, `MB2_WaterSplashUpdate`, `MB2_CoinUpdate`, `MB2_WindGuru2MasuNoGet`, `MB2_PathCheck`, `MB2_PathLenGet`, `PathCheck`, `fn_1_BF54`, `fn_1_CEB8` |

Three pairs are the *same function under two names*, provable because the target
sizes are byte-identical:

| w02sDll name | size | w02Dll name | size |
|---|---|---|---|
| `fn_1_42FC` | 0x3F8 = 1016 | `fn_1_BF54` | 0x3F8 = 1016 |
| `fn_1_5260` | 0x168 = 360 | `fn_1_CEB8` | 0x168 = 360 |
| `MB2_CoinExec` | 0xA70 = 2672 | `MB2_CoinUpdate` | 0xA70 = 2672 |

**Action for the next session: check every REL module whose source has a
`w*`/`m*` sibling.** A unit that is `Object(NonMatching, ...)` with a source
file on disk is never compiled into the `.plf`, never linked, and never
byte-verified, so a draft copied from a sibling module survives indefinitely
without anyone noticing. This one was 75 % wrong code for that reason.

## `NonMatching` with a source file on disk — present, and benign here

`configure.py` line: `Object(NonMatching, "REL/w02sDll/world02.c")`, and
`src/REL/w02sDll/world02.c` exists (5936 lines). So the BRIEF section 3 hazard
is nominally present. It is **not** currently breaking the build, because
`build.ninja` does emit a real `mwcc_sjis` edge for
`build/GP5E01_00/src/REL/w02sDll/world02.o`. But the `.plf` does not depend on
it (NonMatching means unlinked), so `ninja <mod>.plf` would never rebuild it
and `unit_status.py`'s numbers come from whatever the object happened to be.
**Build the object explicitly** — `ninja build/GP5E01_00/src/REL/w02sDll/world02.o`
— or the measurements are stale.

## Section sizes, before and after the structural fix

| section | target | before | after | note |
|---|---|---|---|---|
| `.text` | 42440 | 74212 | 40568 | 38.5 KB was w02Dll-only code |
| `.rodata` | 416 | 656 | **416** | exact |
| `.data` | 1292 | 4047 | 4033 | w02Dll Zen2/Rain tables still present |
| `.bss` | 236 | 340 | 340 | w02Dll path/river statics still present |

`.data` and `.bss` are still w02Dll's. They carry no `.text` and no relocation
into a kept function, so they cost nothing in objdiff terms yet, but they must
be replaced before this unit can be anything but `Equivalent`.

## What remains, ranked by target size

Nothing here is capped by the toolchain except where noted; all of it is work.

### Band 1 — unwritten, 0 % (10884 B, 25.6 % of the unit)

| function | target | note |
|---|---|---|
| `MB2Ev_HatenaTaiyo` | 6836 | **the whole ballgame.** No `MB2Ev_HatenaTaiyo` exists in any other module's symbols.txt, so there is nothing to copy; it must be read out of the asm. Needs the three `mb2ev_TaiyoRainbow{1,2,3}VtxTbl` tables (240 B each) plus `mb2ev_TaiyoRainbowVtxTbl`/`VtxNumTbl`/`DestTbl`/`CoinBasePos` from the target `.data`. Budget a full session for this alone. |
| `MB2_CoinExec` | 2672 | was w02Dll's `MB2_CoinUpdate`; currently 75.32 % |
| `fn_1_42FC` | 1016 | was w02Dll's `fn_1_BF54`; currently 87.35 % — closest of the three |
| `fn_1_5260` | 360 | was w02Dll's `fn_1_CEB8`; currently 84.11 % |

### Band 2 — multi-kilobyte, wrong because the w02sDll body is SHORTER (4292 B)

These are not register problems. w02sDll's versions are roughly half the size of
w02Dll's, so the whole body has to be re-derived, not tuned.

| function | target | ours | w02Dll's | delta |
|---|---|---|---|---|
| `MB2_Create` | 1772 | 3068 | 3020 | +1296 |
| `MB2_MainObjUpdate` | 1620 | 3152 | 2976 | +1532 |
| `MB2Ev_Rainbow` | 4056 | 4788 | 4536 | +732 |
| `objCall` | 2120 | 2396 | 2120 | +276 |

`MB2_MainObjUpdate` and `MB2Ev_Rainbow` are also the last two callers of the
w02Dll-only helpers `MB2_PathLenGet` / `MB2_PathCheck` / `PathCheck`
(`src` lines 1141-1148, 3495, 3539). **Rewriting those two functions is what
deletes 1012 B of non-target `.text` and unblocks the rest of the unit.**

### Band 3 — hundreds of bytes, ordinary matching work (about 9000 B)

Best value first, all already written and all in the 75-99 % band:
`MB2_SplashEffCreateRandX` 92.22 % (600 B, 26 rows — dominated by frame layout),
`MB2_ParticleCreate` 89.89 % (1208 B), `MB2_RainbowObjCreate` 53.26 % (2976 B),
`MB2_CoinEffCreateRand` 55.13 % (1104 B), `ParticleDraw` 60.04 % (2624 B),
`MB2_CoinEffCreateColRand` 37.05 % (844 B), `MB2_CoinCreate` 65.80 % (544 B),
`MB2_CoinNumGet` 77.00 % (140 B), `MB2_SplashEffNumGet` 63.41 % (88 B),
`MB2_YajiHiliteSet` 37.40 % (80 B), `MB2_CoinEffNumGet` 48.35 % (80 B).

### Band 4 — capped by the CodeWarrior-version wall

| function | target | residue |
|---|---|---|
| `MB2_YajiUpdate` | 548 | 7 of 137 rows: pure r28/r29 tie-break between `dat` and `modelP`. All 24 orderings of the four locals compiled; they split into exactly two classes (7 rows or 13) and none reaches the target's `dat=r28`. Same shape as m574Dll `player` `fn_1_E98`. |
| `MB2_CoinEffUpdate` | 496 | 10 of 125 rows: `obj` r26 vs r27, `p` r27 vs r3, and the constant-vs-value f1/f0 choice. All 24 orderings land on 10 or 16; three spellings of the inner `!=` produce a byte-identical object, so that operand is not source-addressable. |
| `MB2_CoinEffCreate` | 476 | 18 of 121 rows, all register tie-break (r5→r23 vs r30, etc.). |

`MB2Ev_HatenaTaiyo` also has to be checked against `singleton.h` before anyone
budgets a session on it — run `python tools\assert_scan.py`.

## Status

Unit stays `Object(NonMatching, ...)`. Not `Equivalent` yet: four functions
have no body at all, so `Equivalent` would be a false claim.