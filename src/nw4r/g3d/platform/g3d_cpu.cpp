#include <nw4r/g3d/platform/g3d_cpu.h>

namespace nw4r {
namespace g3d {
namespace detail {

#if !defined(PORT) /* port version below */
void Copy32ByteBlocks(register void *pDst, register const void *pSrc, register u32 size) {
    register f32 work0, work1, work2, work3;

    for (size /= 32; size > 0; size--) {
        asm {
            lfd  work0, 0(pSrc)
            stfd work0, 0(pDst)

            lfd  work1, 8(pSrc)
            stfd work1, 8(pDst)

            lfd  work2, 16(pSrc)
            stfd work2, 16(pDst)

            lfd  work3, 24(pSrc)
            stfd work3, 24(pDst)
        }

        pDst = static_cast<u8 *>(pDst) + 32;
        pSrc = static_cast<const u8 *>(pSrc) + 32;
    }
}
#else
void Copy32ByteBlocks(void* pDst, const void* pSrc, u32 size) {
    // Whole 32-byte blocks only, as the original.
    __builtin_memcpy(pDst, pSrc, size & ~31u);
}
#endif

#if !defined(PORT) /* port version below */
void ZeroMemory32ByteBlocks(register void *pDst, register u32 size) {
    register f32 zero = 0.0f;

    for (size /= 32; size > 0; size--) {
        asm {
            psq_st zero,  0(pDst), 0, 0
            psq_st zero,  8(pDst), 0, 0
            psq_st zero, 16(pDst), 0, 0
            psq_st zero, 24(pDst), 0, 0
        }

        pDst = static_cast<u8 *>(pDst) + 32;
    }
}
#else
void ZeroMemory32ByteBlocks(void* pDst, u32 size) {
    __builtin_memset(pDst, 0, size & ~31u);
}
#endif

#if !defined(PORT) /* port version below */
void ZeroMemory16ByteBlocks(register void *pDst, register u32 size) {
    register f32 zero = 0.0f;

    for (size /= 16; size > 0; size--) {
        asm {
            psq_st zero,  0(pDst), 0, 0
            psq_st zero,  8(pDst), 0, 0
        }

        pDst = static_cast<u8 *>(pDst) + 16;
    }
}
#else
void ZeroMemory16ByteBlocks(void* pDst, u32 size) {
    __builtin_memset(pDst, 0, size & ~15u);
}
#endif

}
}
}
