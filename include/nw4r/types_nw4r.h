#ifndef NW4R_TYPES_H
#define NW4R_TYPES_H
#include <revolution/types.h>
#include <stddef.h>

#define NW4R_BYTEORDER_BIG 0xFEFF
#define NW4R_BYTEORDER_LITTLE 0xFFFE

#if defined(NW4R_LITTLE_ENDIAN)
#define NW4R_BYTEORDER_NATIVE NW4R_BYTEORDER_LITTLE
#else
#define NW4R_BYTEORDER_NATIVE NW4R_BYTEORDER_BIG
#endif

/* Offset of an intrusive list node that directly follows the vtable pointer: 4 on the
 * console, 8 on a 64-bit host. */
#define NW4R_LINK_AFTER_VPTR sizeof(void*)

#define NW4R_VERSION(major, minor) ((major & 0xFF) << 8 | minor & 0xFF)

#define FOURCC(c0, c1, c2, c3)                                                 \
    (u32)((c0 & 0xFF) << 24 | (c1 & 0xFF) << 16 | (c2 & 0xFF) << 8 |           \
          (c3 & 0xFF))

#define NW4R_LIB_VERSION(NAME, ORIGINAL_DATE, ORIGINAL_TIME, ORIGINAL_CWCC)    \
    const char* NW4R_##NAME##_Version_ =                                       \
        "<< NW4R    - " #NAME " \tfinal   build: " ORIGINAL_DATE               \
        " " ORIGINAL_TIME " (" ORIGINAL_CWCC ") >>"

#endif
