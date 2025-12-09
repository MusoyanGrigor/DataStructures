#pragma once

#include <functional>

namespace st {
    template <typename Container, typename Compare>
    void heapify(Container &c, size_t i, size_t size, Compare comp) {
        size_t largest = i;
        size_t left = 2 * i + 1;
        size_t right = 2 * i + 2;

        if (left < size && comp(c[largest], c[left])) {
            largest = left;
        }

        if (right < size && comp(c[largest], c[right])) {
            largest = right;
        }

        if (largest != i) {
            std::swap(c[i], c[largest]);
            heapify(c, largest, size, comp);
        }
    }

    template <typename Container, typename Compare>
    void build_heap(Container &c, Compare comp) {
        for (int i = c.size() / 2 - 1; i >= 0; --i) {
            heapify(c, i, c.size(), comp);
        }
    }

    template <typename Container, typename Compare = std::less<>>
    void heap_sort(Container &c, Compare comp = Compare{}) {
        build_heap(c, comp);

        for (int i = c.size() - 1; i > 0; --i) {
            std::swap(c[0], c[i]);
            heapify(c, 0, i, comp);
        }
    }
}