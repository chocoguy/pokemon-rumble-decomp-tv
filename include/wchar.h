/* MSL's wide-character functions. The port compiles MSL's own versions (src/MSL_C), because the host's assume a 4-byte wchar_t; see compile_check.py's msl group. */
#ifndef MSL_WCHAR_H
#define MSL_WCHAR_H

#include <stddef.h>
#include <stdarg.h>
#include <wchar_t.h>

#ifdef __cplusplus
extern "C" {
#endif

wchar_t* wmemcpy(wchar_t* dest, const wchar_t* src, size_t count) __attribute__((nothrow));
wchar_t* wmemmove(wchar_t* dest, const wchar_t* src, size_t count) __attribute__((nothrow));
wchar_t* wmemset(wchar_t* dest, wchar_t value, size_t count);
wchar_t* wmemchr(const wchar_t* src, wchar_t value, size_t count);
int wmemcmp(const wchar_t* lhs, const wchar_t* rhs, size_t count) __attribute__((nothrow));
long wcstol(const wchar_t* str, wchar_t** end, int base);
float wcstof(const wchar_t* str, wchar_t** end);
int vswprintf(wchar_t* dst, size_t count, const wchar_t* format, va_list args);
int swprintf(wchar_t* dst, size_t count, const wchar_t* format, ...);

#ifdef __cplusplus
}
#endif

#endif
