# carddll/player - triage of what is left

State at this commit: **11/43 functions byte-identical, 3820/136164 code bytes
(2.81 %)**. 32 functions remain. The unit is `Object(NonMatching, ...)`
(`configure.py:899`), so its object is out of the link and the `carddll` module
hash is unaffected either way; do not flip the status line until the whole unit
is at 100 %.

`.rodata` 0x2f8 + `.data` 0x708 + `.bss` 0x580 = 3.9 KB against 136 KB of
`.text`. **This is a pure code unit - data reconstruction is not worth a
session here.** All the bytes are in functions.

## Ranked by size, with what is known about each

| # | function | bytes | notes |
|---|---|---|---|
| 1 | fn_1_1B2F0 | 23284 | 17 % of the unit alone. Expect a state machine plus big tables. Read the frame size and the `.rodata` relocations first. |
| 2 | fn_1_258E4 | 13252 | |
| 3 | fn_1_2B280 | 13000 | called from fn_1_2B10C on flag bit 0x4000 (see the mask note below) |
| 4 | fn_1_227A4 | 11424 | |
| 5 | fn_1_13728 | 9744 | called from fn_1_FB88, state 0 |
| 6 | fn_1_FCD4 | 8052 | called from fn_1_FB88, state 1 |
| 7 | fn_1_2E548 | 5764 | called from fn_1_2B10C on flag bit 0x2000 |
| 8 | fn_1_198F4 | 4664 | |
| 9 | fn_1_187FC | 4344 | |
| 10 | fn_1_2930C | 3924 | |
| 11 | fn_1_2A260 | 3756 | |
| 12 | fn_1_16DBC | 3628 | |
| 13 | fn_1_21028 | 3196 | called twice from fn_1_20DE4; worth doing early because it is already referenced |
| 14 | fn_1_21CA4 | 2816 | |
| 15 | fn_1_15D38 | 2132 | |
| 16 | fn_1_11C48 | 2040 | called from fn_1_FB88, state 2 |
| 17 | fn_1_1658C | 2096 | called from fn_1_FB88, state 3 |
| 18 | fn_1_1AB2C | 1988 | |
| 19 | fn_1_127F8 | 1548 | called from fn_1_FB88, state 4; no argument |
| 20 | fn_1_182E0 | 1308 | |
| 21 | fn_1_1328C | 1180 | |
| 22 | fn_1_12E04 | 1160 | |
| 23 | fn_1_2FFAC | 1076 | |
| 24 | fn_1_28CA8 | 1044 | |
| 25 | fn_1_12440 | 952 | |
| 26 | fn_1_305B4 | 860 | |
| 27 | fn_1_2FBCC | 808 | **98.42 %, blocked** - see caps |
| 28 | fn_1_30B00 | 776 | |
| 29 | fn_1_25624 | 704 | |
| 30 | fn_1_18054 | 652 | |
| 31 | fn_1_290BC | 592 | **99.97 %, 1 row** - see caps. Cheapest win on the board. |
| 32 | fn_1_20DE4 | 580 | **99.48 %, 15 rows** - see caps |

## Measured MWCC facts from this unit (all confirmed by prediction, not just correlation)

1. **`register` on a parameter changes its register allocation.** On
   `fn_1_304D8` it moved the second parameter from r26 up to r29, which is
   where the target keeps it, and the function went straight to 100 %. It does
   nothing when the parameter already holds a register (measured on
   `fn_1_20DE4`: `register` on the first, the second, and both, all identical).
2. **MWCC 1.3.2 folds `x & (1 << b)` into `rlwinm ..., 31-b, 31-b`** - the
   rotate mask is built with the bit index counted from the MSB. Ten
   independent masks, and the prediction held: writing the mask bit-reversed
   produced the target's bytes. Multi-bit masks are fine (`& 0xfffffffd` is
   unaffected), and the `u` suffix does not help. `fn_1_2B10C` is already
   byte-identical with bit-reversed constants, which is the same rule seen from
   the other side. **Every single-bit mask test in the remaining 32 functions
   will need the reversed constant plus a comment saying which bit is really
   meant**, or it will never reach 100 %.
3. **A ternary costs one instruction more than if/else.** `a = c ? x : y;`
   emits the test, `li temp`, and `mr a, temp` after the join; if/else writes
   each constant straight into its destination. In `fn_1_17BE8` four ternaries
   were the entire reason two temporaries spilled into callee-saved registers
   and the frame grew from 0x20 to 0x30.
4. **Negating an if condition flips which branch is inline.** `if (c) v = 1;
   else v = 2;` puts 1 in the fall-through with a `bne` over it;
   `if (!c) v = 2; else v = 1;` puts 2 in the fall-through with a `beq`.
   Needed in `fn_1_20DE4` and worth trying before touching registers.
5. **A loop counter shared by two loops frees a callee-saved register** and
   shifts every other local up by one, which is what finally matched the
   locals in `fn_1_2FBCC`. The target's two loops really do share one variable.
6. **`lbl_1_bss_468` is indexed by slot number, not slot*2.** `slwi r4, r30, 1`
   once per call site, not `slwi r0` + `slwi r4`.
7. **Three-step selects must stay three locals.** `if (a) v=1; else if (b) v=2;
   else v=3;` collapses to one variable; the target allocates three and copies
   outwards (r27<-r26<-r29). Spelled as nested blocks it matches.

## Proven caps - do not spend a session here

- **fn_1_2FBCC (808 B, 98.42 %).** MWCC puts the function-scope loop counter in
  r31 and the parameter in r30; the target has r30 and r31 the other way round,
  and the swap then propagates into every row that touches either. `register` on
  the parameter, on the local, and a nested-block declaration were all measured;
  none moves it. Everything else in the function matches.
- **fn_1_20DE4 (580 B, 15 rows).** Same wall: MWCC gives the two parameters
  r31/r30 in the order (arg0, arg1); the target gives r30/r31. All three
  `register` variants measured, no change. Every other row, the frame, the
  prologue and the three store chains match.
- **fn_1_290BC (592 B, 1 row).** The target's `blt` for
  `lbl_1_bss_C48[0] >= 0xF` lands on the block-exit `b` that closes the if:
  CodeWarrior kept an empty `else` and MWCC folds it, so MWCC aims the `blt`
  straight at the epilogue. `else {}` is folded too; `else { return; }` adds a
  second `b` (149 rows, 6 differing) and is worse. Block-placement artefact.
- **`singleton.h` assert sites** (BRIEF section 6) - none seen in this unit yet,
  but the six large state-machine functions are where to look.

## Suggested order for the next session

Take the four remaining `fn_1_FB88` / `fn_1_2B10C` leaf handlers first -
`fn_1_21028` (3196), `fn_1_127F8` (1548), `fn_1_11C48` (2040), `fn_1_1658C`
(2096) - because they are already referenced from functions at 100 %, their
structure is known, and together they are 8.9 KB. Then `fn_1_30B00` (776),
`fn_1_25624` (704), `fn_1_18054` (652) and `fn_1_305B4` (860), the remaining
sub-1 KB functions. Only then the multi-kilobyte state machines, where the
frame size and the number of callee-saved registers in the prologue should be
read before anything is written: BRIEF section 6's structural diagnostics are
what pay off there, and the register-allocation facts above decide the spelling.