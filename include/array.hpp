#pragma once

#include <iostream>
#include "iterator.hpp"

template <typename T, std::size_t N>
class Array {
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    using iterator = Random_access_iterator<T>;
    using const_iterator = Random_access_iterator<const T>;
    using reverse_iterator = Reverse_random_access_iterator<T>;
    using const_reverse_iterator = Reverse_random_access_iterator<const T>;

    // Constructors
    constexpr Array() noexcept = default;

    constexpr Array(const Array& other) {
        for (size_type i = 0; i < N; ++i) {
            m_data[i] = other.m_data[i];
        }
    }

    constexpr Array(Array&& other) noexcept {
        for (size_type i = 0; i < N; ++i) {
            m_data[i] = std::move(other.m_data[i]);
        }
    }

    Array(std::initializer_list<T> init) {
        if (N != init.size()) {
            throw std::invalid_argument("Initializer list size does not match array size");
        }
        for (size_type i = 0; i < N; ++i) {
            m_data[i] = init.begin()[i];
        }
    }

    // Assignment operator
    Array& operator=(const Array& other) {
        if (this != &other) {
            for (size_type i = 0; i < N; ++i) {
                m_data[i] = other.m_data[i];
            }
        }
        return *this;
    }

    Array& operator=(Array&& other) noexcept {
        if (this != &other) {
            for (size_type i = 0; i < N; ++i) {
                m_data[i] = std::move(other.m_data[i]);
            }
        }
        return *this;
    }

    // Destructor
    ~Array() = default;

    // Element access
    reference operator[](size_type index) {
        return m_data[index];
    }

    const_reference operator[](size_type index) const {
        return m_data[index];
    }

    reference at(size_type index) {
        if (index >= N) throw std::out_of_range("Array index out of range");
        return m_data[index];
    }

    const_reference at(size_type index) const {
        if (index >= N) throw std::out_of_range("Array index out of range");
        return m_data[index];
    }

    reference front() {
        return m_data[0];
    }

    const_reference front() const {
        return m_data[0];
    }

    reference back() {
        return m_data[N - 1];
    }

    const_reference back() const {
        return m_data[N - 1];
    }

    pointer data() noexcept {
        return m_data;
    }

    const_pointer data() const noexcept {
        return m_data;
    }

    // Size
    [[nodiscard]] constexpr bool empty() const noexcept {
        return N == 0;
    }

    [[nodiscard]] constexpr size_type size() const noexcept {
        return N;
    }

    [[nodiscard]] constexpr size_type max_size() const noexcept {
        return N;
    }

    // Operations
    void fill(const_reference value) {
        for (size_type i = 0; i < N; ++i) {
            m_data[i] = value;
        }
    }

    void swap(Array& other) noexcept(noexcept(swap(std::declval<reference>(), std::declval<reference>()))) {
        for (size_type i = 0; i < N; ++i) {
            std::swap(m_data[i], other.m_data[i]);
        }
    }

    // Iterators
    iterator begin() noexcept {
        return iterator(m_data);
    }

    const_iterator begin() const noexcept {
        return const_iterator(m_data);
    }

    const_iterator cbegin() const noexcept {
        return const_iterator(m_data);
    }

    iterator end() noexcept {
        return iterator(m_data + N);
    }

    const_iterator end() const noexcept {
        return const_iterator(m_data + N);
    }

    const_iterator cend() const noexcept {
        return const_iterator(m_data + N);
    }

    reverse_iterator rbegin() noexcept {
        return reverse_iterator(m_data + N);
    }

    const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator(m_data + N);
    }

    const_reverse_iterator crbegin() const noexcept {
        return const_reverse_iterator(m_data + N);
    }

    reverse_iterator rend() noexcept {
        return reverse_iterator(m_data);
    }

    const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator(m_data);
    }

    const_reverse_iterator crend() const noexcept {
        return const_reverse_iterator(m_data);
    }

private:
    value_type m_data[N];
};