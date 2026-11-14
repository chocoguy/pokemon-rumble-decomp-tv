#include <nw4r/g3d/g3d_transform.h>

namespace nw4r {
namespace g3d {
namespace detail {

using math::MTX33;
using math::MTX34;

const f32 sEpsilon = 1e-36f;

#if !defined(PORT) /* port version below */
asm bool CalcViewNrmMtx(register math::MTX33 *pOut, const register math::MTX34 *pW) {
    nofralloc
    psq_l      f0,  MTX34._00(pW), 1, 0
    psq_l      f1,  MTX34._01(pW), 0, 0
    psq_l      f2,  MTX34._10(pW), 1, 0
    ps_merge10 f6,  f1,  f0
    psq_l      f3,  MTX34._11(pW), 0, 0
    psq_l      f4,  MTX34._20(pW), 1, 0
    ps_merge10 f7,  f3,  f2
    psq_l      f5,  MTX34._21(pW), 0, 0
    ps_mul     f11, f3,  f6
    ps_merge10 f8,  f5,  f4
    ps_mul     f13, f5,  f7
    ps_msub    f11, f1,  f7,  f11
    ps_mul     f12, f1,  f8
    ps_msub    f13, f3,  f8,  f13
    lis        r12, sEpsilon@ha
    ps_msub    f12, f5,  f6,  f12
    ps_mul     f7,  f0,  f13
    addi       r12, r12, sEpsilon@l
    ps_mul     f10, f3,  f4
    ps_madd    f7,  f2,  f12, f7
    psq_l      f6,  0x0(r12), 1, 0
    ps_mul     f9,  f0,  f5
    ps_madd    f7,  f4,  f11, f7
    ps_msub    f10, f2,  f5,  f10
    ps_msub    f9,  f1,  f4,  f9
    ps_abs     f8,  f7
    ps_cmpo0   cr0, f8,  f6
    bge        _invertible
    psq_st     f0,  MTX33._00(pOut), 1, 0
    psq_st     f2,  MTX33._10(pOut), 1, 0
    psq_st     f4,  MTX33._20(pOut), 1, 0
    psq_st     f1,  MTX33._01(pOut), 0, 0
    psq_st     f3,  MTX33._11(pOut), 0, 0
    psq_st     f5,  MTX33._21(pOut), 0, 0
    li         r3,  0x0
    blr

_invertible:
    fres       f5,  f7
    ps_mul     f8,  f1,  f2
    ps_sub     f6,  f6,  f6
    ps_add     f1,  f5,  f5
    ps_msub    f8,  f0,  f3,  f8
    ps_mul     f2,  f7,  f5
    ps_nmsub   f0,  f5,  f2,  f1
    ps_muls0   f13, f13, f0
    ps_muls0   f12, f12, f0
    psq_st     f13, MTX33._00(pOut), 0, 0
    ps_muls0   f11, f11, f0
    psq_st     f12, MTX33._10(pOut), 0, 0
    ps_muls0   f10, f10, f0
    psq_st     f11, MTX33._20(pOut), 0, 0
    ps_muls0   f9,  f9,  f0
    psq_st     f10, MTX33._02(pOut), 1, 0
    ps_muls0   f8,  f8,  f0
    psq_st     f9,  MTX33._12(pOut), 1, 0
    psq_st     f8,  MTX33._22(pOut), 1, 0
    li         r3,  0x1
    blr
}
#else
// Cofactors of pW's 3x3 part arranged as the inverse transpose (multiply by 1 / det).
static f32 Det3x3Cofactors(const math::MTX34* pW, f32 c[3][3]) {
    c[0][0] = pW->_11 * pW->_22 - pW->_12 * pW->_21;
    c[0][1] = pW->_12 * pW->_20 - pW->_10 * pW->_22;
    c[0][2] = pW->_10 * pW->_21 - pW->_11 * pW->_20;
    c[1][0] = pW->_02 * pW->_21 - pW->_01 * pW->_22;
    c[1][1] = pW->_00 * pW->_22 - pW->_02 * pW->_20;
    c[1][2] = pW->_01 * pW->_20 - pW->_00 * pW->_21;
    c[2][0] = pW->_01 * pW->_12 - pW->_02 * pW->_11;
    c[2][1] = pW->_02 * pW->_10 - pW->_00 * pW->_12;
    c[2][2] = pW->_00 * pW->_11 - pW->_01 * pW->_10;
    return pW->_00 * c[0][0] + pW->_01 * c[0][1] + pW->_02 * c[0][2];
}

static bool IsInvertible(f32 det) {
    return __builtin_fabsf(det) >= sEpsilon;
}

bool CalcViewNrmMtx(math::MTX33* pOut, const math::MTX34* pW) {
    // Normal matrix: the inverse transpose, or pW's 3x3 part unchanged when singular.
    f32 c[3][3];
    const f32 det = Det3x3Cofactors(pW, c);
    if (!IsInvertible(det)) {
        pOut->_00 = pW->_00; pOut->_01 = pW->_01; pOut->_02 = pW->_02;
        pOut->_10 = pW->_10; pOut->_11 = pW->_11; pOut->_12 = pW->_12;
        pOut->_20 = pW->_20; pOut->_21 = pW->_21; pOut->_22 = pW->_22;
        return false;
    }
    const f32 inv = 1.0f / det;
    pOut->_00 = c[0][0] * inv; pOut->_01 = c[0][1] * inv; pOut->_02 = c[0][2] * inv;
    pOut->_10 = c[1][0] * inv; pOut->_11 = c[1][1] * inv; pOut->_12 = c[1][2] * inv;
    pOut->_20 = c[2][0] * inv; pOut->_21 = c[2][1] * inv; pOut->_22 = c[2][2] * inv;
    return true;
}
#endif

#if !defined(PORT) /* port version below */
asm bool CalcViewTexMtx(register math::MTX34 *pOut, const register math::MTX34 *pW) {
    nofralloc
    psq_l      f0,  MTX34._00(pW), 1, 0
    psq_l      f1,  MTX34._01(pW), 0, 0
    psq_l      f2,  MTX34._10(pW), 1, 0
    ps_merge10 f6,  f1,  f0
    psq_l      f3,  MTX34._11(pW), 0, 0
    psq_l      f4,  MTX34._20(pW), 1, 0
    ps_merge10 f7,  f3,  f2
    psq_l      f5,  MTX34._21(pW), 0, 0
    ps_mul     f11, f3,  f6
    ps_merge10 f8,  f5,  f4
    ps_mul     f13, f5,  f7
    ps_msub    f11, f1,  f7,  f11
    ps_mul     f12, f1,  f8
    ps_msub    f13, f3,  f8,  f13
    lis        r12, sEpsilon@ha
    ps_msub    f12, f5,  f6,  f12
    ps_mul     f7,  f0,  f13
    addi       r12, r12, sEpsilon@l
    ps_mul     f10, f3,  f4
    ps_madd    f7,  f2,  f12, f7
    psq_l      f6,  0x0(r12), 1, 0
    ps_mul     f9,  f0,  f5
    ps_madd    f7,  f4,  f11, f7
    ps_msub    f10, f2,  f5,  f10
    ps_msub    f9,  f1,  f4,  f9
    ps_abs     f8,  f7
    ps_cmpo0   cr0, f8,  f6
    bge        _invertible
    cmplw      pOut,  pW
    beq        _singular_done
    psq_st     f0, MTX34._00(pOut), 1, 0
    psq_st     f2, MTX34._10(pOut), 1, 0
    psq_st     f4, MTX34._20(pOut), 1, 0
    ps_sub     f6,  f6,  f6
    psq_st     f1, MTX34._01(pOut), 0, 0
    psq_st     f3, MTX34._11(pOut), 0, 0
    psq_st     f5, MTX34._21(pOut), 0, 0
    psq_st     f6, MTX34._03(pOut), 1, 0
    psq_st     f6, MTX34._13(pOut), 1, 0
    psq_st     f6, MTX34._23(pOut), 1, 0
_singular_done:
    li         r3, 0x0
    blr
_invertible:
    fres       f5,  f7
    ps_mul     f8,  f1,  f2
    ps_sub     f6,  f6,  f6
    ps_add     f1,  f5,  f5
    ps_msub    f8,  f0,  f3,  f8
    psq_st     f6,  MTX34._03(pOut), 1, 0
    ps_mul     f2,  f7,  f5
    psq_st     f6,  MTX34._13(pOut), 1, 0
    psq_st     f6,  MTX34._23(pOut), 1, 0
    ps_nmsub   f0,  f5,  f2,  f1
    ps_muls0   f13, f13, f0
    ps_muls0   f12, f12, f0
    psq_st     f13, MTX34._00(pOut), 0, 0
    ps_muls0   f11, f11, f0
    psq_st     f12, MTX34._10(pOut), 0, 0
    ps_muls0   f10, f10, f0
    psq_st     f11, MTX34._20(pOut), 0, 0
    ps_muls0   f9,  f9,  f0
    psq_st     f10, MTX34._02(pOut), 1, 0
    ps_muls0   f8,  f8,  f0
    psq_st     f9,  MTX34._12(pOut), 1, 0
    psq_st     f8,  MTX34._22(pOut), 1, 0
    li         r3,  0x1
    blr
}
#else
bool CalcViewTexMtx(math::MTX34* pOut, const math::MTX34* pW) {
    // As CalcViewNrmMtx, into a 3x4 with zero translation. When singular and pOut is
    // pW, the matrix is left exactly as it was.
    f32 c[3][3];
    const f32 det = Det3x3Cofactors(pW, c);
    if (!IsInvertible(det)) {
        if (pOut != pW) {
            pOut->_00 = pW->_00; pOut->_01 = pW->_01; pOut->_02 = pW->_02; pOut->_03 = 0.0f;
            pOut->_10 = pW->_10; pOut->_11 = pW->_11; pOut->_12 = pW->_12; pOut->_13 = 0.0f;
            pOut->_20 = pW->_20; pOut->_21 = pW->_21; pOut->_22 = pW->_22; pOut->_23 = 0.0f;
        }
        return false;
    }
    const f32 inv = 1.0f / det;
    pOut->_00 = c[0][0] * inv; pOut->_01 = c[0][1] * inv; pOut->_02 = c[0][2] * inv; pOut->_03 = 0.0f;
    pOut->_10 = c[1][0] * inv; pOut->_11 = c[1][1] * inv; pOut->_12 = c[1][2] * inv; pOut->_13 = 0.0f;
    pOut->_20 = c[2][0] * inv; pOut->_21 = c[2][1] * inv; pOut->_22 = c[2][2] * inv; pOut->_23 = 0.0f;
    return true;
}
#endif

#if !defined(PORT) /* port version below */
asm bool CalcInvWorldMtx(register math::MTX34 *pOut, const register math::MTX34 *pW) {
    nofralloc
    psq_l      f0, MTX34._00(pW), 1, 0
    psq_l      f1, MTX34._01(pW), 0, 0
    psq_l      f2, MTX34._10(pW), 1, 0
    ps_merge10 f6,  f1,  f0
    psq_l      f3, MTX34._11(pW), 0, 0
    psq_l      f4, MTX34._20(pW), 1, 0
    ps_merge10 f7,  f3,  f2
    psq_l      f5, MTX34._21(pW), 0, 0
    ps_mul     f11, f3,  f6
    ps_merge10 f8,  f5,  f4
    ps_mul     f13, f5,  f7
    ps_msub    f11, f1,  f7,  f11
    ps_mul     f12, f1,  f8
    ps_msub    f13, f3,  f8,  f13
    lis        r12, sEpsilon@ha
    ps_msub    f12, f5,  f6,  f12
    ps_mul     f7,  f0,  f13
    addi       r12, r12, sEpsilon@l
    ps_mul     f10, f3,  f4
    ps_madd    f7,  f2,  f12, f7
    psq_l      f6, 0x0(r12), 1, 0
    ps_mul     f9,  f0,  f5
    ps_madd    f7,  f4,  f11, f7
    ps_msub    f10, f2,  f5,  f10
    ps_msub    f9,  f1,  f4,  f9
    ps_abs     f8,  f7
    ps_cmpo0   cr0, f8,  f6
    bge        _invertible
    li         r3, 0x0
    blr
_invertible:
    fres       f6,  f7
    ps_mul     f8,  f1,  f2
    ps_add     f5,  f6,  f6
    ps_mul     f4,  f7,  f6
    ps_msub    f8,  f0,  f3,  f8
    ps_nmsub   f0,  f6,  f4,  f5
    lfs        f1,  MTX34._03(pW)
    ps_muls0   f13, f13, f0
    lfs        f2,  MTX34._13(pW)
    ps_muls0   f12, f12, f0
    lfs        f3,  MTX34._23(pW)
    ps_muls0   f11, f11, f0
    ps_merge00 f5,  f13, f12
    ps_merge11 f4,  f13, f12
    ps_mul     f6,  f13, f1
    psq_st     f5,  MTX34._00(pOut), 0, 0
    psq_st     f4,  MTX34._10(pOut), 0, 0
    ps_muls0   f10, f10, f0
    ps_muls0   f9,  f9,  f0
    ps_madd    f6,  f12, f2,  f6
    psq_st     f10, MTX34._20(pOut), 1, 0
    ps_muls0   f8,  f8,  f0
    ps_nmadd   f6,  f11, f3,  f6
    psq_st     f9,  MTX34._21(pOut), 1, 0
    ps_mul     f7,  f10, f1
    ps_merge00 f5,  f11, f6
    psq_st     f8,  MTX34._22(pOut), 1, 0
    ps_madd    f7,  f9,  f2,  f7
    ps_merge11 f4,  f11, f6
    psq_st     f5,  MTX34._02(pOut), 0, 0
    ps_nmadd   f7,  f8,  f3,  f7
    psq_st     f4,  MTX34._12(pOut), 0, 0
    psq_st     f7,  MTX34._23(pOut), 1, 0
    li         r3,  0x1
    blr
}
#else
bool CalcInvWorldMtx(math::MTX34* pOut, const math::MTX34* pW) {
    // Full inverse of the 3x4 matrix; pOut is untouched when singular.
    f32 c[3][3];
    const f32 det = Det3x3Cofactors(pW, c);
    if (!IsInvertible(det)) {
        return false;
    }
    const f32 inv = 1.0f / det;
    const f32 t0 = pW->_03, t1 = pW->_13, t2 = pW->_23;
    // The inverse is the transpose of the cofactor matrix over det.
    const f32 i00 = c[0][0] * inv, i01 = c[1][0] * inv, i02 = c[2][0] * inv;
    const f32 i10 = c[0][1] * inv, i11 = c[1][1] * inv, i12 = c[2][1] * inv;
    const f32 i20 = c[0][2] * inv, i21 = c[1][2] * inv, i22 = c[2][2] * inv;
    pOut->_00 = i00; pOut->_01 = i01; pOut->_02 = i02; pOut->_03 = -(i00 * t0 + i01 * t1 + i02 * t2);
    pOut->_10 = i10; pOut->_11 = i11; pOut->_12 = i12; pOut->_13 = -(i10 * t0 + i11 * t1 + i12 * t2);
    pOut->_20 = i20; pOut->_21 = i21; pOut->_22 = i22; pOut->_23 = -(i20 * t0 + i21 * t1 + i22 * t2);
    return true;
}
#endif

}
}
}
