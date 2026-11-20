// Netplay rollback: page-level undo logs of the game's memory (rollback_pages.cpp).
#pragma once
#include <dolphin/types.h>

#include <cstddef>
#include <utility>
#include <vector>

namespace mp5rb {

// write-protects these ranges (their whole pages; the caller saves the partial pages at the ends itself) and starts
// logging the first write to each page
void track(const std::vector<std::pair<u8*, size_t>>& ranges);
// lifts the protection and drops every log
void stop();
bool active();
// starts a new interval: the memory as it is now can be restored with restore(label)
void mark(u32 label);
// puts the memory back as it was at mark(label); later intervals are dropped (false: not possible)
bool restore(u32 label);
// drops the intervals marked before label (no restore will go back that far)
void forget(u32 label);

size_t pageSize();
u64 faults();
bool overflowed();
size_t logged();

} // namespace mp5rb
