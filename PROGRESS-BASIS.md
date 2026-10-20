# Progress measurement

Verified on 2026-10-20 with a rebuilt `build/GP5E01_00/report.json`.

Use the top-level `measures` totals over the entire game. Do not divide by only
the subset of objects with compiled source or per-function comparison scores.

| Measure | Whole-game basis | Verified result |
|---|---|---|
| Matched code | 2,844,800 / 9,481,492 bytes | 30.00% |
| Code linked from source | 1,959,380 / 9,481,492 bytes | 20.67% |
| Complete units | 346 / 881 | 346 |
| Matched functions | 7,854 / 21,157 | 7,854 |

The DOL has 85.50% matched code; REL modules have 17.61%. These are separate
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
