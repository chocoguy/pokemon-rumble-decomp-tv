#include <revolution/types.h>
#include <stddef.h>
#include <stdlib.h>
#include <new>
#if defined(PORT)
#include <port_os.h>
// Game allocations keep 32-bit addresses, so they come from the address window.
#define malloc PortGameAlloc
#define free PortGameFree
#endif

extern std::new_handler __new_handler;

void* operator new(size_t size) throw(std::bad_alloc) {
    if (size == 0) {
        size = 4;
    }
    for (;;) {
        void* pMemory = malloc(size);
        if (pMemory != nullptr) {
            return pMemory;
        }
        if (__new_handler != nullptr) {
            __new_handler();
        } else {
            return nullptr;
        }
    }
}

void* operator new[](size_t size) throw(std::bad_alloc) {
    return ::operator new(size);
}

void operator delete(void* pMemory) throw() {
    if (pMemory != nullptr) {
        free(pMemory);
    }
}

void operator delete[](void* pMemory) throw() {
    if (pMemory != nullptr) {
        free(pMemory);
    }
}

