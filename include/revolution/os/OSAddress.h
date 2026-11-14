#ifndef REVOLUTION_OS_OSADDRESS_H
#define REVOLUTION_OS_OSADDRESS_H

#include "revolution/types.h"

#define OS_BASE_CACHED 0x80000000
#define OS_BASE_UNCACHED 0xC0000000

#if defined(PORT)
/* Wii addresses are offsets in the game address window on the port (port_addr.h). */
#define OSPhysicalToCached(paddr) PortPtrRaw(PORT_ADDR_CACHED | (u32)(paddr))
#define OSPhysicalToUncached(paddr) PortPtrRaw(PORT_ADDR_UNCACHED | (u32)(paddr))
#define OSCachedToPhysical(caddr) PortPhys((const void*)(caddr))
#define OSUncachedToPhysical(ucaddr) PortPhys((const void*)(ucaddr))
#define OSCachedToUncached(caddr) (caddr)
#define OSUncachedToCached(ucaddr) (ucaddr)
#else
#define OSPhysicalToCached(paddr) ((void*)((u32)(paddr) + OS_BASE_CACHED))
#define OSPhysicalToUncached(paddr) ((void*)((u32)(paddr) + OS_BASE_UNCACHED))
#define OSCachedToPhysical(caddr) ((u32)((u8*)(caddr) - OS_BASE_CACHED))
#define OSUncachedToPhysical(ucaddr) ((u32)((u32)(ucaddr) & ~OS_BASE_UNCACHED))
#endif
#if defined(PORT)
#define OSRoundUp32B(x) (((uintptr_t)(x) + 32 - 1) & ~(uintptr_t)(32 - 1))
#define OSRoundDown32B(x) (((uintptr_t)(x)) & ~(uintptr_t)(32 - 1))
#else
#define OSRoundUp32B(x) (((u32)(x) + 32 - 1) & ~(32 - 1))
#define OSRoundDown32B(x) (((u32)(x)) & ~(32 - 1))
#endif

#endif
