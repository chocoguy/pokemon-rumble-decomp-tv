#ifndef WCHAR_T_H
#define WCHAR_T_H

#if defined(PORT)
/* wchar_t is 16 bits on the port as on the Wii (-fshort-wchar); stddef.h defines it in C. */
#include <stddef.h>
#include <sys/_types/_wint_t.h>
#else
#if (!defined(__cplusplus))
    #ifdef __cplusplus
        extern "C" {
    #endif

    typedef unsigned short wchar_t;
    typedef wchar_t wint_t;

    #ifdef __cplusplus
    }
    #endif
#else
    typedef int wint_t;
#endif
#endif

#endif
