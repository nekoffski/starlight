#include "Core.hh"

#include "Random.hh"

namespace starlight {

Tag<Str> Tag<Str>::fromUuid() { return Tag<Str>{RandomEngine::get().uuid()}; }

}  // namespace starlight
