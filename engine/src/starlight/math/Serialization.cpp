#include "Serialization.hh"

#include "glm/gtx/string_cast.hpp"

#include <boost/algorithm/string.hpp>

namespace sl {

template <> Vec4<f32> fromString<Vec4<f32>>(std::string_view v) {
    v.remove_prefix(1);

    u8 el = 0u;
    Vec4<f32> out;
    std::string part;

    for (u16 i = 0; i < v.size(); ++i) {
        if (v[i] == ' ') {
            continue;
        } else if (v[i] == ',' || v[i] == ']') {
            out[el++] = std::stof(part);
            part.clear();
        } else {
            part += v[i];
        }
    }

    return out;
}

}  // namespace sl
