#pragma once

#include <functional>

namespace st {
    template <typename Container,  typename Compare = std::less<>>
    void selection_sort(Container &c, Compare comp = Compare{}) {
        for (size_t i = 0; i < c.size(); ++i) {
            size_t index = i;
            for (size_t j = i + 1; j < c.size(); ++j) {
                if (comp(c[j], c[index])) {
                    index = j;
                }
            }
            std::swap(c[index], c[i]);
        }
    }
}
