#include <cstdint>
#include <cstring>
#include <memory>

extern "C" {
#include "dolphin/dvd.h"
#include "dolphin/os.h"
}
#include "port/dolassets.h"

namespace {

struct Blob {
    uint32_t address;
    uint32_t size;
};

// Addresses and sizes in GP5E01_00's main.dol (config/GP5E01_00/symbols.txt)
constexpr Blob kDolBlobs[DOL_INCLUDE_COUNT] = {
    /* ank8x8_4b      */ { 0x801AD020, 0x2000 },
    /* Ascii8x8_1bpp  */ { 0x801BB05F, 0x0800 },

    /* refMapData0    */ { 0x801AF800, 0x1240 },
    /* refMapData1    */ { 0x801B0A40, 0x1100 },
    /* refMapData2    */ { 0x801B1B40, 0x2080 },
    /* refMapData3    */ { 0x801B3BC0, 0x2080 },
    /* refMapData4    */ { 0x801B5C40, 0x2080 },

    /* toonMapData    */ { 0x801B7CC0, 0x0880 },
    /* toonMapData2   */ { 0x801B8540, 0x1080 },

    /* hiliteData     */ { 0x801B95C0, 0x0480 },
    /* hiliteData2    */ { 0x801B9A40, 0x0480 },
    /* hiliteData3    */ { 0x801B9EC0, 0x0480 },
    /* hiliteData4    */ { 0x801BA340, 0x0480 },

    /* coveropen_en   */ { 0x801C1A10, 0x1384 },
    /* fatalerror_en  */ { 0x801C2D94, 0x1384 },
    /* loading_en     */ { 0x801C4118, 0x1384 },
    /* nodisc_en      */ { 0x801C549C, 0x1384 },
    /* retryerror_en  */ { 0x801C6820, 0x1384 },
    /* wrongdisc_en   */ { 0x801C7BA4, 0x1384 },
};

struct RelBlob {
    const char *path;
    uint32_t section; // index in the REL section table
    uint32_t offset;  // offset inside that section
    uint32_t size;
};

// .data is section 5 in these modules (.text .ctors .dtors .rodata .data .bss)
constexpr RelBlob kRelBlobs[REL_INCLUDE_COUNT] = {
    /* bootDll   nintendoData */ { "dll/bootDll.rel", 5, 0xC0, 0x307A },
    /* e3bootdll nintendoData */ { "dll/e3bootdll.rel", 5, 0x80, 0x307A },
};

std::unique_ptr<uint8_t[]> sDol;
size_t sDolSize;
std::unique_ptr<uint8_t[]> sRelBlobs[REL_INCLUDE_COUNT];

uint32_t read_be32(const uint8_t *p)
{
    return (uint32_t(p[0]) << 24) | (uint32_t(p[1]) << 16) | (uint32_t(p[2]) << 8) | uint32_t(p[3]);
}

// Translates an address of the loaded executable to its place in the DOL file
const uint8_t *dol_pointer(uint32_t address, uint32_t size)
{
    constexpr size_t kSections = 18; // 7 text + 11 data
    if (!sDol || sDolSize < 0x100) {
        return nullptr;
    }
    for (size_t i = 0; i < kSections; i++) {
        uint32_t offset = read_be32(&sDol[0x00 + i * 4]);
        uint32_t start = read_be32(&sDol[0x48 + i * 4]);
        uint32_t length = read_be32(&sDol[0x90 + i * 4]);
        if (length == 0 || address < start || address - start + uint64_t(size) > length) {
            continue;
        }
        uint64_t fileOffset = uint64_t(offset) + (address - start);
        if (fileOffset + size > sDolSize) {
            return nullptr;
        }
        return &sDol[fileOffset];
    }
    return nullptr;
}

} // namespace

extern "C" {

void InitializeDol(void)
{
    s32 size = 0;
    const u8 *dol = DVDGetDOLLocation(&size);
    if (!dol || size <= 0) {
        OSReport("dolassets> main.dol is not available on the disc\n");
        return;
    }
    sDol = std::make_unique<uint8_t[]>(size_t(size));
    std::memcpy(sDol.get(), dol, size_t(size));
    sDolSize = size_t(size);
}

void *GetDolIncludeData(int index)
{
    if (index < 0 || index >= DOL_INCLUDE_COUNT) {
        return nullptr;
    }
    const Blob &blob = kDolBlobs[index];
    // Casting away const is deliberate: the game patches some of these in
    // place (sprite banks turn offsets into pointers), as it did in RAM.
    auto *data = const_cast<uint8_t *>(dol_pointer(blob.address, blob.size));
    if (!data) {
        OSReport("dolassets> blob %d (%08x) not found in main.dol\n", index, blob.address);
    }
    return data;
}

void *GetRelIncludeData(int index)
{
    if (index < 0 || index >= REL_INCLUDE_COUNT) {
        return nullptr;
    }
    if (sRelBlobs[index]) {
        return sRelBlobs[index].get();
    }
    const RelBlob &blob = kRelBlobs[index];
    DVDFileInfo file;
    if (!DVDOpen(blob.path, &file)) {
        OSReport("dolassets> cannot open %s\n", blob.path);
        return nullptr;
    }
    uint8_t header[0x14];
    uint8_t entry[8];
    void *result = nullptr;
    if (DVDReadPrio(&file, header, sizeof(header), 0, 2) == sizeof(header)) {
        uint32_t sectionCount = read_be32(&header[0x0C]);
        uint32_t sectionTable = read_be32(&header[0x10]);
        if (blob.section < sectionCount
            && DVDReadPrio(&file, entry, sizeof(entry), s32(sectionTable + blob.section * 8), 2) == sizeof(entry)) {
            uint32_t sectionOffset = read_be32(entry) & ~1u; // bit 0 marks executable sections
            uint32_t sectionSize = read_be32(entry + 4);
            if (sectionOffset != 0 && blob.offset + uint64_t(blob.size) <= sectionSize) {
                auto buffer = std::make_unique<uint8_t[]>(blob.size);
                if (DVDReadPrio(&file, buffer.get(), s32(blob.size), s32(sectionOffset + blob.offset), 2) == s32(blob.size)) {
                    sRelBlobs[index] = std::move(buffer);
                    result = sRelBlobs[index].get();
                }
            }
        }
    }
    DVDClose(&file);
    if (!result) {
        OSReport("dolassets> blob %d not found in %s\n", index, blob.path);
    }
    return result;
}

} // extern "C"
