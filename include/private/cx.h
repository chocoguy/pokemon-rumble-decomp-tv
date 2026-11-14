#ifndef PRIVATE_CX_H
#define PRIVATE_CX_H

#include "revolution/cx.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Compressed data stores its sizes and words little-endian: the console swapped them, a
 * little-endian host reads them as they are. */
#if defined(PORT)
static inline u32 CXiConvertEndian_(u32 x) {
    return x;
}

static inline u16 CXiConvertEndian16_(u16 x) {
    return x;
}
#else
static inline u32 CXiConvertEndian_(u32 x) {
    return ((x & 0xFF000000) >> 24) | ((x & 0x00FF0000) >> 8) | ((x & 0x0000FF00) << 8) | ((x & 0x000000FF) << 24);
}

static inline u16 CXiConvertEndian16_(u16 x) {
    return (u16)(((x & 0xFF00) >> 8) | ((x & 0x00FF) << 8));
}
#endif

BOOL CXiVerifyHuffmanTable_(const void* pTable, u8 bit);

#ifdef __cplusplus
}
#endif

#endif
