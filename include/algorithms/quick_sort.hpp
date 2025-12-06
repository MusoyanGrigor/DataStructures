#pragma once

#include <functional>

namespace st {
    template <typename Container, typename Compare = std::less<> >
    int partition(Container &c, const int left, const int right, Compare comp = Compare{}) {
        auto pivot = c[right];
        int i = left - 1;

        for (int j = left; j < right; ++j) {
            if (comp(c[j], pivot)) {
                ++i;
                std::swap(c[i], c[j]);
            }
        }
        std::swap(c[i + 1], c[right]);
        return i + 1;
    }

    template <typename Container, typename Compare = std::less<> >
    void quick_sort(Container &c, const int left, const int right, Compare comp = Compare{}) {
        if (left < right) {
            const int pivot = partition(c, left, right, comp);
            quick_sort(c, left, pivot - 1, comp);
            quick_sort(c, pivot + 1, right, comp);
        }
    }

    // Wrapper function for simple call
    template <typename Container, typename Compare = std::less<> >
    void quick_sort(Container &c, Compare comp = Compare{}) {
        if (!c.empty()) {
            quick_sort(c, 0, c.size() - 1, comp);
        }
    }
}
