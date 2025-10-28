#pragma once

#include <algorithm>
#include <cstddef>
#include <limits>
#include <random>
#include <stdexcept>

#include "iterator.hpp"
#include "algorithm.hpp"

template <typename T>
class Vector {
public:
    using value_type = T;
    using pointer = T *;
    using const_pointer = const T *;
    using reference = T &;
    using const_reference = const T &;
    using size_type = std::size_t;
    using iterator = Random_access_iterator<T>;
    using const_iterator = Random_access_iterator<const T>;
    using reverse_iterator = Reverse_random_access_iterator<T>;
    using const_reverse_iterator = Reverse_random_access_iterator<const T>;

    // Constructors
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

    explicit Vector(const size_type count, const_reference value) : m_size(count), m_capacity(count) {
        m_data = new value_type[m_capacity];
        for (size_type i = 0; i < m_size; ++i) {
            m_data[i] = value;
        }
    }

    template<typename InputIt, typename = std::enable_if_t<std::is_base_of_v<std::input_iterator_tag,
        typename std::iterator_traits<InputIt>::iterator_category> > >
    explicit Vector(InputIt begin, InputIt end) {
        m_size = 0;
        m_capacity = it::distance(begin, end) * 2;
        m_data = new value_type[m_capacity];
        for (auto it = begin; it != end; ++it) {
            m_data[m_size++] = *it;
        }
    }

    Vector(const size_type size, std::pair<T, T> range) : m_size(size), m_capacity(size * 2) {
        m_data = new value_type[m_capacity];
        std::random_device rd;
        std::mt19937 gen(rd());
        if constexpr (std::is_integral_v<T>) {
            std::uniform_int_distribution<T> dis(range.first, range.second);
            for (size_type i = 0; i < m_size; ++i) {
                m_data[i] = dis(gen);
            }
        } else if constexpr (std::is_floating_point_v<T>) {
            std::uniform_real_distribution<T> dis(range.first, range.second);
            for (size_type i = 0; i < m_size; ++i) {
                m_data[i] = dis(gen);
            }
        } else {
            throw std::invalid_argument("Vector size must be integer or floating point type");
        }
    }

    // Assignment operator
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

    // Destructor
    ~Vector() {
        delete[] m_data;
    }

    // Element access
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

    [[nodiscard]] pointer data() {
        return m_data;
    }

    [[nodiscard]] const_pointer data() const {
        return m_data;
    }

    // Capacity & size
    [[nodiscard]] bool empty() const {
        return m_size == 0;
    }

    [[nodiscard]] size_type size() const {
        return m_size;
    }

    [[nodiscard]] size_type max_size() const {
        return std::numeric_limits<size_type>::max() / (sizeof(value_type) * 2);
    }

    [[nodiscard]] size_type capacity() const {
        return m_capacity;
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

    // Modifiers
    template<typename U>
    void push_back(U &&value) {
        if (m_size == m_capacity) resize_data(m_size * 2);
        m_data[m_size++] = std::forward<U>(value);
    }

    void pop_back() {
        if (m_size == 0) throw std::runtime_error("Vector is empty");
        m_size--;
    }

    void insert(const_iterator pos, const_reference value) {
        const size_type index = pos - cbegin();
        if (index > m_size) throw std::out_of_range("Index out of range");
        if (m_size == m_capacity) resize_data(m_size * 2);

        for (size_type i = m_size; i > index; --i) {
            m_data[i] = std::move(m_data[i - 1]);
        }

        m_data[index] = value;
        ++m_size;
    }

    void insert(const_iterator pos, value_type &&value) {
        const size_type index = pos - cbegin();
        if (index > m_size) throw std::out_of_range("Index out of range");
        if (m_size == m_capacity) resize_data(m_size * 2);

        for (size_type i = m_size; i > index; --i) {
            m_data[i] = std::move(m_data[i - 1]);
        }

        m_data[index] = std::move(value);
        ++m_size;
    }

    void insert(const_iterator pos, const size_type count, const_reference value) {
        if (count == 0) return;

        const size_type index = pos - cbegin();
        if (index > m_size) throw std::out_of_range("Index out of range");
        if (m_size + count > m_capacity) resize_data(m_size + count);

        for (size_type i = m_size; i > index; --i) {
            m_data[i + count - 1] = std::move(m_data[i - 1]);
        }

        for (size_type i = 0; i < count; ++i) {
            m_data[index + i] = value;
        }
        m_size += count;
    }

    template<typename InputIt, typename = std::enable_if_t<it::is_iterator<InputIt>::value> >
    void insert(const_iterator pos, InputIt first, InputIt last) {
        const size_type count = it::distance(first, last);
        if (count == 0) return;

        const size_type index = pos - cbegin();
        if (index > m_size) throw std::out_of_range("Index out of range");
        if (m_size + count > m_capacity) resize_data(m_size + count);

        for (size_type i = m_size; i > index; --i) {
            m_data[i + count - 1] = std::move(m_data[i - 1]);
        }

        size_type j = 0;
        for (auto it = first; it != last; ++it) {
            m_data[index + j++] = *it;
        }
        m_size += count;
    }

    void insert(const_iterator pos, std::initializer_list<T> i_list) {
        const size_type count = i_list.size();
        if (count == 0) return;

        const size_type index = pos - cbegin();
        if (index > m_size) throw std::out_of_range("Index out of range");
        if (m_size + count > m_capacity) resize_data(m_size + count);

        for (size_type i = m_size; i > index; --i) {
            m_data[i + count - 1] = std::move(m_data[i - 1]);
        }

        size_type j = 0;
        for (size_type i = 0; i < count; ++i) {
            m_data[index + i] = i_list.begin()[j++];
        }
        m_size += count;
    }

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

    template<typename... Args>
    void emplace(const_iterator pos, Args &&... args) {
        const size_type index = pos - cbegin();
        if (index > m_size) throw std::out_of_range("Index out of range");
        if (m_size == m_capacity) resize_data(m_size * 2);

        for (size_type i = m_size; i > index; --i) {
            m_data[i] = std::move(m_data[i - 1]);
        }

        m_data[index] = value_type(std::forward<Args>(args)...);
        ++m_size;
    }

    template<typename... Args>
    void emplace_back(Args &&... args) {
        if (m_size == m_capacity) resize_data(m_size * 2);
        push_back(value_type(std::forward<Args>(args)...));
    }

    iterator erase(const_iterator pos) {
        size_type index = pos - cbegin();
        if (index > m_size) throw std::out_of_range("Index out of range");

        for (size_type i = index; i < m_size - 1; ++i) {
            m_data[i] = std::move(m_data[i + 1]);
        }

        --m_size;
        return iterator(m_data + index);
    }

    iterator erase(const_iterator first, const_iterator last) {
        if (first > last || first < cbegin() || last > cend())
            throw std::out_of_range("Index out of range");

        size_type index = first - cbegin();
        const size_type count = last - first;

        if (!count) return iterator(m_data + index);

        for (size_type i = index; i + count < m_size; ++i) {
            m_data[i] = std::move(m_data[i + count]);
        }
        m_size -= count;

        return iterator(m_data + index);
    }

    void clear() {
        m_size = 0;
    }

    void assign(const size_type count, const_reference value) {
        clear();
        if (count > m_size) resize_data(count);

        for (size_type i = 0; i < count; ++i) {
            m_data[i] = value;
        }
        m_size = count;
    }

    template<typename InputIt, typename = std::enable_if_t<it::is_iterator<InputIt>::value> >
    void assign(InputIt first, InputIt last) {
        clear();
        auto count = std::distance(first, last);
        if (count > m_size) resize_data(count);

        for (auto it = first; it != last; ++it) {
            m_data[m_size++] = *it;
        }
    }

    void assign(std::initializer_list<value_type> i_list) {
        assign(i_list.begin(), i_list.end());
    }

    void swap(Vector &other) noexcept {
        std::swap(m_data, other.m_data);
        std::swap(m_size, other.m_size);
        std::swap(m_capacity, other.m_capacity);
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
        return iterator(m_data + m_size);
    }

    const_iterator end() const noexcept {
        return const_iterator(m_data + m_size);
    }

    const_iterator cend() const noexcept {
        return const_iterator(m_data + m_size);
    }

    reverse_iterator rbegin() noexcept {
        return reverse_iterator(m_data + m_size);
    }

    const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator(m_data + m_size);
    }

    const_reverse_iterator crbegin() const noexcept {
        return const_reverse_iterator(m_data + m_size);
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

    // Relational operators
    auto operator<=>(const Vector &other) const {
        return std::lexicographical_compare_three_way(m_data, m_data + m_size,
                                                      other.m_data, other.m_data + other.m_size);
    }

    bool operator==(const Vector &other) const {
        return (*this <=> other) == 0;
    }

    bool operator!=(const Vector &other) const {
        return !(*this == other);
    }

private:
    pointer m_data;
    size_type m_size;
    size_type m_capacity;

    // Helper function to resize internal storage
    void resize_data(const size_type min_capacity = 0) {
        if (m_capacity == 0) m_capacity = 4;

        while (m_capacity < min_capacity) {
            m_capacity *= 2;
        }

        auto new_data = new value_type[m_capacity];

        for (size_type i = 0; i < m_size; ++i)
            new_data[i] = std::move(m_data[i]);

        delete[] m_data;
        m_data = new_data;
    }
};

template<typename T>
void swap(Vector<T> &lhs, Vector<T> &rhs) noexcept {
    lhs.swap(rhs);
}