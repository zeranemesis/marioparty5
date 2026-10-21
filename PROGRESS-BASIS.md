# Progress measurement

Verified on 2026-10-21 with a rebuilt `build/GP5E01_00/report.json`.

Use the top-level `measures` totals over the entire game. Do not divide by only
the subset of objects with compiled source or per-function comparison scores.

| Measure | Whole-game basis | Verified result |
|---|---|---|
| Matched code | 2,848,524 / 9,481,492 bytes | 30.04% |
| Code linked from source | 1,980,928 / 9,481,492 bytes | 20.89% |
| Complete units | 349 / 881 | 349 |
| Matched functions | 7,860 / 21,157 | 7,860 |

The DOL has 85.58% matched code; REL modules have 17.64%. These are separate
subsets, not competing whole-game percentages. The full reconstruction passes
all 135 original SHA-1 checks because unfinished units use extracted objects.

`Matching` and `Equivalent` serve different purposes: matching units can enter
the exact rebuild; equivalent units enable the native port and do not establish
an identical instruction stream or proven runtime equivalence.

Regenerate the comparison report with `ninja`, then run
`python tools/avancement.py` to update `docs/avancement.md`. The validated
comparison-tool version is objdiff-cli 3.3.1. The legacy function-diff helper
may separately require objdiff-cli 2.7.1.

Earlier notes in this file used inconsistent denominators and reported a
2.6-fold discrepancy. The current top-level report and Ninja totals agree;
those historical subset percentages should not be used as whole-game progress.
