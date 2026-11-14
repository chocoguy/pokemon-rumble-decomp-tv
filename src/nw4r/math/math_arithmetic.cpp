#include <nw4r/math/math_arithmetic.h>

namespace nw4r {
namespace math {

#if !defined(PORT) /* port version below */
f32 FrSqrt(register f32 x) {
    register f32 rsqrt;
    register f32 c_half = 0.5f, c_three = 3.0f;
    register f32 work0, work1;

    asm (
        frsqrte rsqrt, x

        fmuls work0, rsqrt, rsqrt
        fmuls work1, rsqrt, c_half
        fnmsubs work0, work0, x, c_three
        fmuls work1, work0, work1
    )

    return work1;
}
#else
f32 FrSqrt(f32 x) {
    // The original refines frsqrte with one Newton step.
    return 1.0f / __builtin_sqrtf(x);
}
#endif

u32 CntBit1(u32 x) {
    u32 n = x - ((x >> 1) & 0x55555555);
    n = (n & 0x33333333) + ((n >> 2) & 0x33333333);
    n = (n + (n >> 4)) & 0x0F0F0F0F;
    n += n >> 8;
    n += n >> 16;
    return n & 0x3F;
}

}
}

#if defined(PORT)
/* The table-driven versions were dropped by the Wii linker (nothing in the game called
 * them directly); the C library computes the same functions. */
#include <math.h>
namespace nw4r {
namespace math {
namespace detail {

f32 FExp(f32 x) {
    return expf(x);
}

f32 FLog(f32 x) {
    return logf(x);
}

}
}
}
#endif
