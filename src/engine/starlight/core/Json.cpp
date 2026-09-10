#include "Json.hh"

namespace sl {

Result<Json> parseJson(const Str& str) {
    try {
        return Json::parse(str);
    } catch (const Json::parse_error& e) {
        return Error::unexpected(
            ErrorCode::jsonParsingError, "Could not parse json: {}", e.what()
        );
    }
}

}  // namespace sl
