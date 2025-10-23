#pragma once
#include <cstddef>

template <typename Category, typename T,
typename Distance = std::ptrdiff_t, typename Pointer = T*, typename Reference = T&>
struct Iterator {
    using iterator_category = Category;
    using value_type = T;
    using difference_type = Distance;
    using pointer = Pointer;
    using reference = Reference;
};