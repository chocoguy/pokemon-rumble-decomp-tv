#include <nw4r/ut/ut_list.h>

#if defined(PORT)
/* The console's list code, with the link read and written through accessors so a list
 * can also hold Link32 links (see ut_list.h). */
namespace nw4r {
namespace ut {
namespace {

inline bool IsLink32(const List* pList) {
    return (pList->offset & NW4R_UT_LIST_LINK32) != 0;
}

inline void* LinkAddr(const List* pList, const void* pObject) {
    return reinterpret_cast<u8*>(const_cast<void*>(pObject)) + (pList->offset & ~NW4R_UT_LIST_LINK32);
}

inline void* GetPrev(const List* pList, const void* pObject) {
    if (IsLink32(pList)) {
        return PortPtrRaw(static_cast<Link32*>(LinkAddr(pList, pObject))->prevObject);
    }
    return static_cast<Link*>(LinkAddr(pList, pObject))->prevObject;
}

inline void* GetNext(const List* pList, const void* pObject) {
    if (IsLink32(pList)) {
        return PortPtrRaw(static_cast<Link32*>(LinkAddr(pList, pObject))->nextObject);
    }
    return static_cast<Link*>(LinkAddr(pList, pObject))->nextObject;
}

inline void SetPrev(const List* pList, void* pObject, void* pValue) {
    if (IsLink32(pList)) {
        static_cast<Link32*>(LinkAddr(pList, pObject))->prevObject = PortAddr(pValue);
    } else {
        static_cast<Link*>(LinkAddr(pList, pObject))->prevObject = pValue;
    }
}

inline void SetNext(const List* pList, void* pObject, void* pValue) {
    if (IsLink32(pList)) {
        static_cast<Link32*>(LinkAddr(pList, pObject))->nextObject = PortAddr(pValue);
    } else {
        static_cast<Link*>(LinkAddr(pList, pObject))->nextObject = pValue;
    }
}

}

void List_Init(List* pList, u16 offset) {
    pList->headObject = nullptr;
    pList->tailObject = nullptr;
    pList->numObjects = 0;
    pList->offset = offset;
}

static void SetFirstObject(List* pList, void* pObject) {
    SetNext(pList, pObject, nullptr);
    SetPrev(pList, pObject, nullptr);
    pList->headObject = pObject;
    pList->tailObject = pObject;
    pList->numObjects++;
}

void List_Append(List* pList, void* pObject) {
    if (pList->headObject == nullptr) {
        SetFirstObject(pList, pObject);
        return;
    }
    SetPrev(pList, pObject, pList->tailObject);
    SetNext(pList, pObject, nullptr);
    SetNext(pList, pList->tailObject, pObject);
    pList->tailObject = pObject;
    pList->numObjects++;
}

void List_Prepend(List* pList, void* pObject) {
    if (pList->headObject == nullptr) {
        SetFirstObject(pList, pObject);
        return;
    }
    SetPrev(pList, pObject, nullptr);
    SetNext(pList, pObject, pList->headObject);
    SetPrev(pList, pList->headObject, pObject);
    pList->headObject = pObject;
    pList->numObjects++;
}

void List_Insert(List* pList, void* pTarget, void* pObject) {
    if (pTarget == nullptr) {
        List_Append(pList, pObject);
        return;
    }
    if (pTarget == pList->headObject) {
        List_Prepend(pList, pObject);
        return;
    }
    void* pPrev = GetPrev(pList, pTarget);
    SetPrev(pList, pObject, pPrev);
    SetNext(pList, pObject, pTarget);
    SetNext(pList, pPrev, pObject);
    SetPrev(pList, pTarget, pObject);
    pList->numObjects++;
}

void List_Remove(List* pList, void* pObject) {
    void* pPrev = GetPrev(pList, pObject);
    void* pNext = GetNext(pList, pObject);
    if (pPrev == nullptr) {
        pList->headObject = pNext;
    } else {
        SetNext(pList, pPrev, pNext);
    }
    if (pNext == nullptr) {
        pList->tailObject = pPrev;
    } else {
        SetPrev(pList, pNext, pPrev);
    }
    SetPrev(pList, pObject, nullptr);
    SetNext(pList, pObject, nullptr);
    pList->numObjects--;
}

void* List_GetNext(const List* pList, const void* pObject) {
    if (pObject == nullptr) {
        return pList->headObject;
    }
    return GetNext(pList, pObject);
}

void* List_GetPrev(const List* pList, const void* pObject) {
    if (pObject == nullptr) {
        return pList->tailObject;
    }
    return GetPrev(pList, pObject);
}

void* List_GetNth(const List* pList, u16 index) {
    int i;
    void* pObject;
    for (i = 0, pObject = nullptr; (pObject = List_GetNext(pList, pObject)) != nullptr; i++) {
        if (index == i) {
            return pObject;
        }
    }
    return nullptr;
}

}
}
#else

namespace nw4r {
namespace ut {

void List_Init(List* pList, u16 offset) {
    pList->headObject = nullptr;
    pList->tailObject = nullptr;
    pList->numObjects = 0;
    pList->offset = offset;
}

static void SetFirstObject(List* pList, void* pObject) {
    Link* pLink = NW4R_UT_LIST_GET_LINK(*pList, pObject);
    pLink->nextObject = nullptr;
    pLink->prevObject = nullptr;

    pList->headObject = pObject;
    pList->tailObject = pObject;
    pList->numObjects++;
}

void List_Append(List* pList, void* pObject) {
    if (pList->headObject == nullptr) {
        SetFirstObject(pList, pObject);
        return;
    }

    Link* pLink = NW4R_UT_LIST_GET_LINK(*pList, pObject);
    pLink->prevObject = pList->tailObject;
    pLink->nextObject = nullptr;

    NW4R_UT_LIST_GET_LINK(*pList, pList->tailObject)->nextObject = pObject;
    pList->tailObject = pObject;
    pList->numObjects++;
}

void List_Prepend(List* pList, void* pObject) {
    if (pList->headObject == nullptr) {
        SetFirstObject(pList, pObject);
        return;
    }

    Link* pLink = NW4R_UT_LIST_GET_LINK(*pList, pObject);
    pLink->prevObject = nullptr;
    pLink->nextObject = pList->headObject;

    NW4R_UT_LIST_GET_LINK(*pList, pList->headObject)->prevObject = pObject;
    pList->headObject = pObject;
    pList->numObjects++;
}

void List_Insert(List* pList, void* pTarget, void* pObject) {
    if (pTarget == nullptr) {
        List_Append(pList, pObject);
        return;
    }

    if (pTarget == pList->headObject) {
        List_Prepend(pList, pObject);
        return;
    }

    Link* pLink = NW4R_UT_LIST_GET_LINK(*pList, pObject);
    void* pPrev = NW4R_UT_LIST_GET_LINK(*pList, pTarget)->prevObject;
    Link* pPrevLink = NW4R_UT_LIST_GET_LINK(*pList, pPrev);

    pLink->prevObject = pPrev;
    pLink->nextObject = pTarget;
    pPrevLink->nextObject = pObject;
    NW4R_UT_LIST_GET_LINK(*pList, pTarget)->prevObject = pObject;
    pList->numObjects++;
}

void List_Remove(List* pList, void* pObject) {
    Link* pLink = NW4R_UT_LIST_GET_LINK(*pList, pObject);

    if (pLink->prevObject == nullptr) {
        pList->headObject = pLink->nextObject;
    } else {
        NW4R_UT_LIST_GET_LINK(*pList, pLink->prevObject)->nextObject = pLink->nextObject;
    }

    if (pLink->nextObject == nullptr) {
        pList->tailObject = pLink->prevObject;
    } else {
        NW4R_UT_LIST_GET_LINK(*pList, pLink->nextObject)->prevObject = pLink->prevObject;
    }

    pLink->prevObject = nullptr;
    pLink->nextObject = nullptr;
    pList->numObjects--;
}

void* List_GetNext(const List* pList, const void* pObject) {
    if (pObject == nullptr) {
        return pList->headObject;
    }

    return NW4R_UT_LIST_GET_LINK(*pList, pObject)->nextObject;
}

void* List_GetPrev(const List* pList, const void* pObject) {
    if (pObject == nullptr) {
        return pList->tailObject;
    }

    return NW4R_UT_LIST_GET_LINK(*pList, pObject)->prevObject;
}

void* List_GetNth(const List* pList, u16 index) {
    int i;
    void* pObject;

    for (i = 0, pObject = nullptr; (pObject = List_GetNext(pList, pObject)) != nullptr; i++) {
        if (index == i) {
            return pObject;
        }
    }

    return nullptr;
}

}
}
#endif
