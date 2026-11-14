#if defined(PORT)
/* The port uses the system C library; see port/include/port_libc.h for the additions. */
#include_next <stdbool.h>
#include <port_libc.h>
#else
#ifndef STDBOOL_H
#define STDBOOL_H

#ifndef __cplusplus
#ifndef bool
typedef unsigned char bool;
#endif
#define false 0
#define true 1
#endif

#endif
#endif
