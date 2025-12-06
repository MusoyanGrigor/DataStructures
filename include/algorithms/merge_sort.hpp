#pragma once

#include <functional>

namespace st {
    template <typename Container, typename Compare = std::less<>>
    void merge(Container &c, const int left, const int mid, const int right, Compare comp = Compare{}) {
        const int L_size = mid - left + 1;
        const int R_size = right - mid;

        Container L, R;
        for (int i = 0; i < L_size; ++i) L.push_back(c[left + i]);
        for (int i = 0; i < R_size; ++i) R.push_back(c[mid + 1 + i]);

        int L_index = 0;
        int R_index = 0;
        int c_index = left;
        while (L_index < L_size && R_index < R_size) {
            if (comp(L[L_index], R[R_index])) c[c_index++] = L[L_index++];
            else c[c_index++] = R[R_index++];
        }

        while (L_index < L_size) c[c_index++] = L[L_index++];
        while (R_index < R_size) c[c_index++] = R[R_index++];
    }

    template <typename Container, typename Compare = std::less<>>
    void merge_sort(Container &c, int left, int right, Compare comp = Compare{}) {
        if (left < right) {
            int mid = left + (right - left) / 2;
            merge_sort(c, left, mid, comp);
            merge_sort(c, mid + 1, right, comp);
            merge(c, left, mid, right, comp);
        }
    }

    template <typename Container, typename Compare = std::less<>>
    void merge_sort(Container &c, Compare comp = Compare{}) {
        if (!c.empty()) merge_sort(c, 0, c.size() - 1, comp);
    }
}