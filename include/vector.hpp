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
        m_data = new T[m_capacity];
    }

    Vector(const Vector &other) : m_size(other.m_size), m_capacity(other.m_capacity) {
        m_data = new T[m_capacity];
        for (size_t i = 0; i < m_size; ++i) {
            m_data[i] = other.m_data[i];
        }
    }

    Vector(Vector &&other) noexcept : m_size(other.m_size), m_capacity(other.m_capacity) {
        m_data = other.m_data;
        other.m_size = 0;
        other.m_capacity = 0;
        other.m_data = nullptr;
    }

    Vector(std::initializer_list<T> init) {
        m_size = init.size();
        m_capacity = m_size * 2;
        m_data = new T[m_capacity];
        for (size_t i = 0; i < m_size; ++i) {
            m_data[i] = init.begin()[i];
        }
    }

    explicit Vector(const std::size_t count) : m_size(count), m_capacity(count) {
        m_data = new T[m_capacity]();
    }

    explicit Vector(const std::size_t count, const T &value) : m_size(count), m_capacity(count) {
        m_data = new T[m_capacity];
        for (size_t i = 0; i < m_size; ++i) {
            m_data[i] = value;
        }
    }

    explicit Vector(iterator begin, iterator end) {
        m_size = 0;
        m_capacity = (end - begin) * 2;
        m_data = new T[m_capacity];
        for (auto it = begin; it != end; ++it) {
            m_data[m_size++] = *it;
        }
    }

    Vector &operator=(const Vector &other) {
        if (this != &other) {
            delete[] m_data;
            m_size = other.m_size;
            m_capacity = other.m_capacity;
            m_data = new T[m_capacity];
            for (size_t i = 0; i < m_size; ++i) {
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

    Vector &operator=(const std::initializer_list<T> &init) {
        delete[] m_data;
        m_size = init.size();
        m_capacity = m_size * 2;
        m_data = new T[m_capacity];
        for (size_t i = 0; i < m_size; ++i) {
            m_data[i] = init.begin()[i];
        }
        return *this;
    }

    ~Vector() {
        delete[] m_data;
    }

    [[nodiscard]] std::size_t size() const { return m_size; }
    [[nodiscard]] std::size_t max_size() const { return std::numeric_limits<std::size_t>::max() / (sizeof(T) * 2); }
    [[nodiscard]] std::size_t capacity() const { return m_capacity; }
    [[nodiscard]] T *data() const { return m_data; }


    void resize(const std::size_t count, const T &value = T()) {
        if (count < m_size) {
            m_size = count;
        } else if (count > m_size) {
            if (count > m_capacity) {
                const std::size_t new_capacity = std::max(count, m_capacity * 2 + 1);
                T *new_data = new T[new_capacity];

                for (std::size_t i = 0; i < m_size; ++i) {
                    new_data[i] = std::move(m_data[i]);
                }

                delete[] m_data;
                m_data = new_data;
                m_capacity = new_capacity;
            }

            for (std::size_t i = m_size; i < count; ++i) {
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
        push_back(T(std::forward<Args>(args)...));
    }

    T &operator[](size_t index) {
        return m_data[index];
    }

    const T &operator[](size_t index) const {
        return m_data[index];
    }

    T &at(std::size_t index) {
        if (index >= m_size) throw std::out_of_range("Index out of range");
        return m_data[index];
    }

    const T &at(std::size_t index) const {
        if (index >= m_size) throw std::out_of_range("Index out of range");
        return m_data[index];
    }

    [[nodiscard]] bool empty() const { return m_size == 0; }

    void clear() {
        m_size = 0;
    }

    void reserve(const size_t new_capacity) {
        if (new_capacity > m_capacity) {
            T *new_data = new T[new_capacity];
            for (size_t i = 0; i < m_size; ++i) {
                new_data[i] = m_data[i];
            }
            delete[] m_data;
            m_data = new_data;
            m_capacity = new_capacity;
        }
    }

    void shrink_to_fit() {
        if (m_size == m_capacity) return;

        T *new_data = new T[m_size];
        for (size_t i = 0; i < m_size; ++i) {
            new_data[i] = std::move(m_data[i]);
        }
        delete[] m_data;
        m_data = new_data;
        m_capacity = m_size;
    }

    T &front() {
        if (m_size == 0) throw std::runtime_error("Vector is empty");
        return m_data[0];
    }

    const T &front() const {
        if (m_size == 0) throw std::runtime_error("Vector is empty");
        return m_data[0];
    }

    T &back() {
        if (m_size == 0) throw std::runtime_error("Vector is empty");
        return m_data[m_size - 1];
    }

    const T &back() const {
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

    const_iterator begin() const noexcept {
        return const_iterator(m_data);
    }

    iterator end() noexcept {
        return iterator(m_data + m_size);
    }

    const_iterator end() const noexcept {
        return const_iterator(m_data + m_size);
    }

private:
    T *m_data;
    std::size_t m_size;
    std::size_t m_capacity;

    void resize_data() {
        if (!m_capacity) m_capacity = 4;
        else m_capacity *= 2;

        T *new_data = new T[m_capacity];
        for (std::size_t i = 0; i < m_size; ++i) {
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
