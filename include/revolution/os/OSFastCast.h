#ifndef OSFASTCAST_H
#define OSFASTCAST_H
#include <revolution/types.h>

/* The Wii converted between floats and small integers with quantised paired-single
 * loads and stores (scale 0): float to integer truncates toward zero and saturates. */
#ifdef __cplusplus
extern "C" {
#endif

#define OS_GQR_TYPE_U8 4
#define OS_GQR_TYPE_U16 5
#define OS_GQR_TYPE_S8 6
#define OS_GQR_TYPE_S16 7

static inline void OSInitFastCast(void) {}

static inline u8 __OSf32tou8(f32 in) { return in >= 255.0f ? 255 : in > 0.0f ? (u8)in : 0; }
static inline u16 __OSf32tou16(f32 in) { return in >= 65535.0f ? 65535 : in > 0.0f ? (u16)in : 0; }
static inline s8 __OSf32tos8(f32 in) { return in >= 127.0f ? 127 : in <= -128.0f ? -128 : in == in ? (s8)in : 0; }
static inline s16 __OSf32tos16(f32 in) { return in >= 32767.0f ? 32767 : in <= -32768.0f ? -32768 : in == in ? (s16)in : 0; }
static inline void OSf32tou8(const f32* in, volatile u8* out) { *out = __OSf32tou8(*in); }
static inline void OSf32tou16(const f32* in, volatile u16* out) { *out = __OSf32tou16(*in); }
static inline void OSf32tos8(const f32* in, volatile s8* out) { *out = __OSf32tos8(*in); }
static inline void OSf32tos16(const f32* in, volatile s16* out) { *out = __OSf32tos16(*in); }

static inline f32 __OSu8tof32(const u8* in) { return (f32)*in; }
static inline f32 __OSu16tof32(const u16* in) { return (f32)*in; }
static inline f32 __OSs8tof32(const s8* in) { return (f32)*in; }
static inline f32 __OSs16tof32(const s16* in) { return (f32)*in; }
static inline void OSu8tof32(const u8* in, volatile f32* out) { *out = (f32)*in; }
static inline void OSu16tof32(const u16* in, volatile f32* out) { *out = (f32)*in; }
static inline void OSs8tof32(const s8* in, volatile f32* out) { *out = (f32)*in; }
static inline void OSs16tof32(const s16* in, volatile f32* out) { *out = (f32)*in; }

#ifdef __cplusplus
}
#endif
#endif
