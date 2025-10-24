#pragma once

#include <iostream>

template <typename T, std::size_t N>
class Array {
public:
    Array() = default;

    Array(const Array& other) {
        for (std::size_t i = 0; i < N; ++i) {
            m_data[i] = other.m_data[i];
        }
    }

    Array(Array&& other) noexcept {
        for (std::size_t i = 0; i < N; ++i) {
            m_data[i] = std::move(other.m_data[i]);
        }
    }

    Array(std::initializer_list<T> init) {
        if (N != init.size()) {
            throw std::invalid_argument("Initializer list size does not match array size");
        }
        for (std::size_t i = 0; i < N; ++i) {
            m_data[i] = init.begin()[i];
        }
    }

    Array& operator=(const Array& other) {
        if (this != &other) {
            for (std::size_t i = 0; i < N; ++i) {
                m_data[i] = other.m_data[i];
            }
        }
        return *this;
    }

    Array& operator=(Array&& other) noexcept {
        if (this != &other) {
            for (std::size_t i = 0; i < N; ++i) {
                m_data[i] = std::move(other.m_data[i]);
            }
        }
        return *this;
    }



    ~Array() = default;
private:
    T m_data[N];
};