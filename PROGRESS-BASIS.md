# Which number is the real one

Two progress numbers exist in this repository and they disagree by a factor of
2.6. Optimising the wrong one is the easiest way to waste a session, so this file
states both and says what is actually known about the gap.

*(An earlier version of this file claimed 473 split objects had no objdiff entry
and needed registering. That was wrong -- it came from a path-prefix bug in
`tools/next_targets.py --unregistered`, now fixed. Measured: **all 881** split
objects have an objdiff entry, and the tool reports 0 unregistered.)*

## The two numbers

| source | basis | result |
|---|---|---|
| `ninja`'s `PROGRESS` step, `All:` line | all 881 split objects | **29.38 %** matched, 20.66 % linked, 346 / 881 files |
| `build/GP5E01_00/report.json` via `tools/avancement.py` | 408 units | **73.80 %** matched (2 593 340 / 3 514 016 B) |

The `PROGRESS` line is the one that sums to the binary: measured 2026-10-05,
`All:` reports `29.38% matched ... Code: 2786052 / 9481492 bytes (7305 / 21157
functions)`, and summing `matched_code` over every unit that has a `total_code`
in `report.json` gives exactly 2 786 052 of 9 481 492 -- the same figure. So the
`PROGRESS` step and `report.json` agree on matched bytes; they differ only in the
denominator, because `report.json` leaves 473 units unscored.

Split per family, from the `PROGRESS` step:

| | code | data | files linked |
|---|---|---|---|
| DOL | 1 479 008 / 1 730 512 B (85.47 %) | 515 474 / 926 941 B (55.61 %) | 232 / 307 |
| REL modules | 1 307 044 / 7 750 980 B (16.86 %) | 109 433 / 1 717 241 B (6.37 %) | 114 / 574 |

So `report.json` sees 3 514 016 bytes of code where the binary holds 9 481 492.
The REL side is where nearly all of the difference lives: 1 986 867 B counted by
`report.json` against 7 750 980 B actual, a gap of about 5.76 MB.

**Not yet established:** why. Coverage is not the reason -- every one of the 881
objects has an objdiff entry, so nothing is invisible. It is a difference in what
per-unit `total_code` is measured over. Do not build a strategy on either number
until `tools/next_targets.py` and the `PROGRESS` step have been reconciled for
one module by hand. That reconciliation is itself a cheap, high-value task: it
decides whether a session spent raising a per-unit percentage can move the
headline at all.

## What to use, today

- **Size the remaining job with the `PROGRESS` step.** Code 6 695 440 B and data
  2 019 275 B unmatched, measured 2026-10-05.
- **Locate the work with `docs/avancement.md`** (generated from `report.json`,
  correct per module and per family) and `tools/next_targets.py`, which rank the
  408 scored units by unmatched bytes, by cheapest gap, or by closest to 100 %.
- **`matched_code_percent` in `report.json` is not the project's progress.** It
  reads 73.80 % while a quarter of the game is unmatched.

## What "100 %" means here

`ninja` ending in `135 files OK` is not the proof. The proof is the last ninja
edge, `CHECK config/GP5E01_00/build.sha1` -- the SHA-1 of `main.dol` plus every
`.rel` against the expected digests. It runs on every build, so a green tail
means the binary is byte-identical right now; it says nothing about whether a
change kept it that way unless you compare before and after. BRIEF section 9
covers why a link failure seen once in your own worktree is not evidence, and
why the hash is.