#pragma once

#include <cstddef>
#include <limits>
#include <stdexcept>

#include "iterator.hpp"

template<typename T>
class Vector {
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    using iterator = Random_access_iterator<T>;
    using const_iterator = Random_access_iterator<const T>;

    Vector() : m_size(0), m_capacity(4) {
        m_data = new value_type[m_capacity];
    }

    Vector(const Vector &other) : m_size(other.m_size), m_capacity(other.m_capacity) {
        m_data = new value_type[m_capacity];
        for (size_type i = 0; i < m_size; ++i) {
            m_data[i] = other.m_data[i];
        }
    }

    Vector(Vector &&other) noexcept : m_size(other.m_size), m_capacity(other.m_capacity) {
        m_data = other.m_data;
        other.m_size = 0;
        other.m_capacity = 0;
        other.m_data = nullptr;
    }

    Vector(std::initializer_list<value_type> init) {
        m_size = init.size();
        m_capacity = m_size * 2;
        m_data = new value_type[m_capacity];
        for (size_type i = 0; i < m_size; ++i) {
            m_data[i] = init.begin()[i];
        }
    }

    explicit Vector(const size_type count) : m_size(count), m_capacity(count) {
        m_data = new value_type[m_capacity]();
    }

    explicit Vector(const size_type count,  const_reference value) : m_size(count), m_capacity(count) {
        m_data = new value_type[m_capacity];
        for (size_type i = 0; i < m_size; ++i) {
            m_data[i] = value;
        }
    }

    explicit Vector(iterator begin, iterator end) {
        m_size = 0;
        m_capacity = (end - begin) * 2;
        m_data = new value_type[m_capacity];
        for (auto it = begin; it != end; ++it) {
            m_data[m_size++] = *it;
        }
    }

    Vector &operator=(const Vector &other) {
        if (this != &other) {
            delete[] m_data;
            m_size = other.m_size;
            m_capacity = other.m_capacity;
            m_data = new value_type[m_capacity];
            for (size_type i = 0; i < m_size; ++i) {
                m_data[i] = other.m_data[i];
            }
        }
        return *this;
    }

    Vector &operator=(Vector &&other) noexcept {
        if (this != &other) {
            delete[] m_data;
            m_size = other.m_size;
            m_capacity = other.m_capacity;
            m_data = other.m_data;
            other.m_size = 0;
            other.m_capacity = 0;
            other.m_data = nullptr;
        }
        return *this;
    }

    Vector &operator=(const std::initializer_list<value_type> &init) {
        delete[] m_data;
        m_size = init.size();
        m_capacity = m_size * 2;
        m_data = new value_type[m_capacity];
        for (size_type i = 0; i < m_size; ++i) {
            m_data[i] = init.begin()[i];
        }
        return *this;
    }

    ~Vector() {
        delete[] m_data;
    }

    [[nodiscard]] size_type size() const { return m_size; }
    [[nodiscard]] size_type max_size() const { return std::numeric_limits<size_type>::max() / (sizeof(value_type) * 2); }
    [[nodiscard]] size_type capacity() const { return m_capacity; }
    [[nodiscard]] pointer data() { return m_data; }
    [[nodiscard]] const_pointer data() const { return m_data; }


    void resize(const size_type count, const_reference value = value_type()) {
        if (count < m_size) {
            m_size = count;
        } else if (count > m_size) {
            if (count > m_capacity) {
                const size_type new_capacity = std::max(count, m_capacity * 2 + 1);
                auto new_data = new value_type[new_capacity];

                for (size_type i = 0; i < m_size; ++i) {
                    new_data[i] = std::move(m_data[i]);
                }

                delete[] m_data;
                m_data = new_data;
                m_capacity = new_capacity;
            }

            for (size_type i = m_size; i < count; ++i) {
                m_data[i] = value;
            }
            m_size = count;
        }
    }

    template<typename U>
    void push_back(U &&value) {
        if (m_size == m_capacity) resize_data();
        m_data[m_size++] = std::forward<U>(value);
    }

    void pop_back() {
        if (m_size == 0) throw std::runtime_error("Vector is empty");
        m_size--;
    }

    template<class... Args>
    void emplace_back(Args&&... args) {
        if (m_size == m_capacity) resize_data();
        push_back(value_type(std::forward<Args>(args)...));
    }

    reference operator[](size_type index) {
        return m_data[index];
    }

    const_reference operator[](size_type index) const {
        return m_data[index];
    }

    reference at(size_type index) {
        if (index >= m_size) throw std::out_of_range("Index out of range");
        return m_data[index];
    }

    const_reference at(size_type index) const {
        if (index >= m_size) throw std::out_of_range("Index out of range");
        return m_data[index];
    }

    [[nodiscard]] bool empty() const { return m_size == 0; }

    void clear() {
        m_size = 0;
    }

    void reserve(const size_type new_capacity) {
        if (new_capacity > m_capacity) {
            auto new_data = new value_type[new_capacity];
            for (size_type i = 0; i < m_size; ++i) {
                new_data[i] = m_data[i];
            }
            delete[] m_data;
            m_data = new_data;
            m_capacity = new_capacity;
        }
    }

    void shrink_to_fit() {
        if (m_size == m_capacity) return;

        auto new_data = new value_type[m_size];
        for (size_type i = 0; i < m_size; ++i) {
            new_data[i] = std::move(m_data[i]);
        }
        delete[] m_data;
        m_data = new_data;
        m_capacity = m_size;
    }

    reference front() {
        if (m_size == 0) throw std::runtime_error("Vector is empty");
        return m_data[0];
    }

    const_reference front() const {
        if (m_size == 0) throw std::runtime_error("Vector is empty");
        return m_data[0];
    }

    reference back() {
        if (m_size == 0) throw std::runtime_error("Vector is empty");
        return m_data[m_size - 1];
    }

    const_reference back() const {
        if (m_size == 0) throw std::runtime_error("Vector is empty");
        return m_data[m_size - 1];
    }

    void swap(Vector& other) noexcept {
        std::swap(m_data, other.m_data);
        std::swap(m_size, other.m_size);
        std::swap(m_capacity, other.m_capacity);
    }

    iterator begin() noexcept {
        return iterator(m_data);
    }

    const_iterator cbegin() const noexcept {
        return const_iterator(m_data);
    }

    iterator end() noexcept {
        return iterator(m_data + m_size);
    }

    const_iterator cend() const noexcept {
        return const_iterator(m_data + m_size);
    }

private:
    pointer m_data;
    size_type m_size;
    size_type m_capacity;

    void resize_data() {
        if (!m_capacity) m_capacity = 4;
        else m_capacity *= 2;

        auto new_data = new value_type[m_capacity];
        for (size_type i = 0; i < m_size; ++i) {
            new_data[i] = m_data[i];
        }
        delete[] m_data;
        m_data = new_data;
    }
};

template <typename T>
void swap(Vector<T>& lhs, Vector<T>& rhs) noexcept {
    lhs.swap(rhs);
}