#pragma once

namespace sl::core {

template <typename T, typename Alloc, template <typename, typename> typename C>
inline int indexOf(const C<T, Alloc>& container, const T& value, const int notFoundIndex = -1) {
    for (typename C<T, Alloc>::size_type i = 0; i < container.size(); ++i)
        if (container[i] == value)
            return i;
    return notFoundIndex;
}

}