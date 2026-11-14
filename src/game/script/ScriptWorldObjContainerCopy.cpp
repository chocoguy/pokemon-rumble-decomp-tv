#define GAME_SCRIPTWORLDOBJ_SEPARATE_TEMPLATES
#define GAME_SCRIPTWORLDOBJ_CONTAINER_TAIL
#define GAME_SCRIPTWORLDOBJ_COPY_CTORS
#include <game/script/detail/ScriptWorldObjContainers.h>
#if defined(PORT)
// The storage's copy constructor is specialized before the pair's uses it (standard C++
// does not allow the reverse order the original used).
namespace Metrowerks {
namespace detail {
template<> CompressedPairStorage<OneNode,WorldIdTree::node*,1>::CompressedPairStorage(const CompressedPairStorage<OneNode,WorldIdTree::node*,1>& rOther) : mSecond(rOther.mSecond) {}
}
template<> NodeCapacity::compressed_pair(const NodeCapacity& rOther) : Storage(rOther) {}
}
#else
namespace Metrowerks {
template<> NodeCapacity::compressed_pair(const NodeCapacity& rOther) : Storage(rOther) {}
namespace detail {
template<> CompressedPairStorage<OneNode,WorldIdTree::node*,1>::CompressedPairStorage(const CompressedPairStorage<OneNode,WorldIdTree::node*,1>& rOther) : mSecond(rOther.mSecond) {}
}
}
#endif
