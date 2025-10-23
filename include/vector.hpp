#pragma once
#include <cstddef>
#include <stdexcept>

template <typename T>
class Vector {
public:
    Vector() : m_size(0), m_capacity(4) {
        m_data = new T[m_capacity];
    }

    Vector(const Vector& other) : m_size(other.m_size), m_capacity(other.m_capacity) {
        m_data = new T[m_capacity];
        for (size_t i = 0; i < m_size; ++i) {
            m_data[i] = other.m_data[i];
        }
    }

    Vector& operator=(const Vector& other) {
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

    Vector(Vector&& other) noexcept : m_size(other.m_size), m_capacity(other.m_capacity) {
        m_data = other.m_data;
        other.m_size = 0;
        other.m_capacity = 0;
        other.m_data = nullptr;
    }

    Vector& operator=(Vector&& other) noexcept {
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

    Vector(std::initializer_list<T> init) {
        m_size = init.size();
        m_capacity = m_size * 2;
        m_data = new T[m_capacity];
        for (size_t i = 0; i < m_size; ++i) {
            m_data[i] = init.begin()[i];
        }
    }

    ~Vector() {
        delete[] m_data;
    }

    [[nodiscard]] size_t size() const { return m_size; }
    [[nodiscard]] size_t capacity() const { return m_capacity; }

    template <typename U>
    void push_back(U&& value) {
        if (m_size == m_capacity) resize_data();
        m_data[m_size++] = std::forward<U>(value);
    }

    void pop_back() {
        if (m_size == 0) throw std::runtime_error("Vector is empty");
        m_size--;
    }

    T& operator[](size_t index) {
        return m_data[index];
    }

    const T& operator[](size_t index) const {
        return m_data[index];
    }

    T& at(std::size_t index) {
        if (index >= m_size) throw std::out_of_range("Index out of range");
        return m_data[index];
    }

    const T& at(std::size_t index) const {
        if (index >= m_size) throw std::out_of_range("Index out of range");
        return m_data[index];
    }

    [[nodiscard]] bool empty() const { return m_size == 0; }

    void clear() {
        delete[] m_data;
        m_size = 0;
        m_capacity = 4;
        m_data = new T[m_capacity];
    }

private:
    T* m_data;
    std::size_t m_size;
    std::size_t m_capacity;

    void resize_data() {
        if (!m_capacity) {
            m_capacity = 4;
        } else {
            m_capacity *= 2;
        }

        T* new_data = new T[m_capacity];
        for (std::size_t i = 0; i < m_size; ++i) {
            new_data[i] = m_data[i];
        }
        delete[] m_data;
        m_data = new_data;
    }
};
