#pragma once

#include <nlohmann/json.hpp>

#include "Error.hh"

namespace sl {

using namespace nlohmann;

using Json = nlohmann::json;

Result<Json> parseJson(const Str& str);

}  // namespace sl
