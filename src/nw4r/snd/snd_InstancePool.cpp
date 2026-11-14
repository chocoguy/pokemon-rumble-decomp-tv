#include <nw4r/ut/ut_algorithm.h>

#include <nw4r/snd/snd_InstancePool.h>

#include <nw4r/ut/ut_Lock.h>

namespace nw4r {
namespace snd {
namespace detail {

u32 PoolImpl::CreateImpl(void* pBuffer, u32 size, u32 stride) {
    ut::AutoInterruptLock lock;

    /* Pointer alignment: 4 on the console, 8 on a 64-bit host. */
    u8* pPtr = static_cast<u8*>(ut::RoundUp(pBuffer, sizeof(void*)));
    stride = ut::RoundUp(stride, sizeof(void*));

    u32 count = (size - (pPtr - static_cast<u8*>(pBuffer))) / stride;

    for (u32 i = 0; i < count; i++, pPtr += stride) {
        PoolImpl* pHead = reinterpret_cast<PoolImpl*>(pPtr);
        pHead->mNext = mNext;
        mNext = pHead;
    }

    return count;
}

void PoolImpl::DestroyImpl(void* pBuffer, u32 size) {
    ut::AutoInterruptLock lock;

    void* pBegin = pBuffer;
    void* pEnd = static_cast<u8*>(pBegin) + size;

    PoolImpl* pPrev = this;
    for (PoolImpl* pIt = mNext; pIt != nullptr; pIt = pIt->mNext) {
        if (pBegin <= pIt && pIt < pEnd) {
            pPrev->mNext = pIt->mNext;
        } else {
            pPrev = pIt;
        }
    }
}

int PoolImpl::CountImpl() const {
    ut::AutoInterruptLock lock;

    int count = 0;
    for (PoolImpl* pIt = mNext; pIt != nullptr; pIt = pIt->mNext) {
        count++;
    }

    return count;
}

void* PoolImpl::AllocImpl() {
    ut::AutoInterruptLock lock;

    if (mNext == nullptr) {
        return nullptr;
    }

    PoolImpl* pHead = mNext;
    mNext = pHead->mNext;
    return pHead;
}

void PoolImpl::FreeImpl(void* pElem) {
    ut::AutoInterruptLock lock;

    PoolImpl* pHead = static_cast<PoolImpl*>(pElem);
    pHead->mNext = mNext;
    mNext = pHead;
}

}
}
}
