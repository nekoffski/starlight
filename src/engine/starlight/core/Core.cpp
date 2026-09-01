#include "Core.hh"

#include "Random.hh"

namespace sl {

Tag<Str> Tag<Str>::fromUuid() { return Tag<Str>{RandomEngine::get().uuid()}; }

}  // namespace sl
