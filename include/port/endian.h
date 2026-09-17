#ifndef PORT_ENDIAN_H_
#define PORT_ENDIAN_H_

/*
 * Everything the game reads from the disc is big-endian, like the GameCube.
 * Every host the port targets (x86-64, arm64) is little-endian, so reading a
 * value stored in game data means swapping its bytes.
 */

#include <stdint.h>

#if defined(_MSC_VER) && !defined(__clang__)
#include <stdlib.h>
#define PORT_BSWAP16(x) ((uint16_t)_byteswap_ushort((uint16_t)(x)))
#define PORT_BSWAP32(x) ((uint32_t)_byteswap_ulong((uint32_t)(x)))
#else
#define PORT_BSWAP16(x) ((uint16_t)__builtin_bswap16((uint16_t)(x)))
#define PORT_BSWAP32(x) ((uint32_t)__builtin_bswap32((uint32_t)(x)))
#endif

/* value of a big-endian field read from game data */
#define PORT_BE16(x) PORT_BSWAP16(x)
#define PORT_BE32(x) PORT_BSWAP32(x)

static inline float PORT_BEF32(uint32_t raw)
{
    uint32_t swapped = PORT_BSWAP32(raw);
    float value;
    char *dst = (char *)&value;
    const char *src = (const char *)&swapped;
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
    dst[3] = src[3];
    return value;
}

#endif
