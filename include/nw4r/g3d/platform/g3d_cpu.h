#ifndef NW4R_G3D_PLATFORM_CPU_H
#define NW4R_G3D_PLATFORM_CPU_H
#include <nw4r/types_nw4r.h>

#include <revolution/os.h>
#include <revolution/os/OSCache.h>
#include <revolution/os/OSFastCast.h>

namespace nw4r {
namespace g3d {

namespace fastcast {

inline f32 U8_0ToF32(const u8 *pPtr) {
    f32 x;
    OSu8tof32(pPtr, &x);
    return x;
}

inline f32 U16_0ToF32(const u16 *pPtr) {
    f32 x;
    OSu16tof32(pPtr, &x);
    return x;
}

inline f32 S7_8ToF32(const s16 *pPtr) {
    // Quantised load, signed 16-bit with 8 fraction bits.
    return static_cast<f32>(*pPtr) * (1.0f / 256.0f);
}

inline f32 S10_5ToF32(const s16 *pPtr) {
    // Quantised load, signed 16-bit with 5 fraction bits.
    return static_cast<f32>(*pPtr) * (1.0f / 32.0f);
}

inline u8 F32ToU8_0(f32 f) {
    u8 x;
    OSf32tou8(&f, &x);
    return x;
}

inline s16 F32ToS10_5(f32 f) {
    // Quantised store: scale by 2^5, truncate toward zero, saturate.
    return __OSf32tos16(f * 32.0f);
}

inline void SetGQR6_S10_5() {
    // No quantisation registers on the port; the conversions above carry their own scale.
}
inline void SetGQR7_S7_8() {
    // See SetGQR6_S10_5.
}

namespace detail {

inline void Init() {
    OSInitFastCast();
    SetGQR6_S10_5();
    SetGQR7_S7_8();
}

}

}

namespace DC {

inline void StoreRange(void *pBase, u32 size) {
    DCStoreRange(pBase, size);
}

inline void StoreRangeNoSync(void *pBase, u32 size) {
    DCStoreRangeNoSync(pBase, size);
}

inline void FlushRangeNoSync(void *pBase, u32 size) {
    DCFlushRangeNoSync(pBase, size);
}

inline void InvalidateRange(void *pBase, u32 size) {
    DCInvalidateRange(pBase, size);
}

}

namespace detail {

void Copy32ByteBlocks(void *pDst, const void *pSrc, u32 size);
void ZeroMemory32ByteBlocks(void *pDst, u32 size);
void ZeroMemory16ByteBlocks(void *pDst, u32 size);

}

inline void InitFastCast() {
    fastcast::detail::Init();
}

}
}

#endif
