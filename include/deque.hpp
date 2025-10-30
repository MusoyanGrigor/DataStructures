#pragma once
#include <algorithm>
#include <cstddef>
#include <stdexcept>

#include "algorithm.hpp"

template<typename T>
class Deque {
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
    Deque() : m_map_capacity(4), m_num_blocks(0), m_size(0) {
        m_map = new pointer[m_map_capacity]();

        m_front_block = m_back_block = m_map_capacity / 2;
        m_map[m_front_block] = new value_type[BLOCK_SIZE];
        ++m_num_blocks;

        m_front_index = BLOCK_SIZE / 2;
        m_back_index = m_front_index - 1;
    }

    Deque(const Deque &other) : m_map_capacity(other.m_map_capacity), m_num_blocks(other.m_num_blocks),
    m_front_block(other.m_front_block), m_back_block(other.m_back_block), m_front_index(other.m_front_index),
    m_back_index(other.m_back_index), m_size(other.m_size) {
        m_map = new pointer[m_map_capacity];
        for (size_type i = 0; i < m_map_capacity; ++i) {
            if (other.m_map[i]) {
                m_map[i] = new value_type[BLOCK_SIZE];
                std::copy(other.m_map[i], other.m_map[i] + BLOCK_SIZE, m_map[i]);
            } else {
                m_map[i] = nullptr;
            }
        }
    }

    Deque(Deque&& other) noexcept
    : m_map(other.m_map), m_map_capacity(other.m_map_capacity), m_num_blocks(other.m_num_blocks),
      m_front_block(other.m_front_block), m_back_block(other.m_back_block), m_front_index(other.m_front_index),
      m_back_index(other.m_back_index), m_size(other.m_size) {
        other.m_map = nullptr;
        other.m_map_capacity = 0;
        other.m_num_blocks = 0;
        other.m_front_block = 0;
        other.m_back_block = 0;
        other.m_front_index = 0;
        other.m_back_index = 0;
        other.m_size = 0;
    }

    Deque(std::initializer_list<value_type> i_list) : Deque() {
        for (const auto& value : i_list) {
            push_back(value);
        }
    }

    explicit Deque(const size_type count) : Deque() {
        for (size_type i = 0; i < count; ++i) {
            push_back(value_type());
        }
    }

    Deque(const size_type count, const_reference value) : Deque() {
        for (size_type i = 0; i < count; ++i) {
            push_back(value);
        }
    }

    template<typename InputIt, typename = std::enable_if_t<it::is_iterator<InputIt>::value>>
    Deque(InputIt first, InputIt last) : Deque() {
        for (auto it = first; it != last; ++it) {
            push_back(*it);
        }
    }

    // Assignment operator
    Deque& operator=(const Deque& other) {
        if (this != &other) {
            Deque temp(other);
            this->swap(temp);
        }
        return *this;
    }

    Deque& operator=(Deque&& other) noexcept {
        if (this != &other) {
            this->swap(other);
        }
        return *this;
    }

    Deque& operator=(std::initializer_list<value_type> i_list) {
        Deque temp(i_list);
        this->swap(temp);
        return *this;
    }

    // Destructor
    ~Deque() {
        cleanup();
        delete[] m_map;
    }

    // Element access
    reference operator[](const size_type index) {
        const size_type absolute_position = m_front_index + index;
        const size_type block_number = absolute_position / BLOCK_SIZE + m_front_block;
        const size_type offset_in_block = absolute_position % BLOCK_SIZE;
        return m_map[block_number][offset_in_block];
    }

    const_reference operator[](const size_type index) const {
        const size_type absolute_position = m_front_index + index;
        const size_type block_number = absolute_position / BLOCK_SIZE + m_front_block;
        const size_type offset_in_block = absolute_position % BLOCK_SIZE;
        return m_map[block_number][offset_in_block];
    }

    reference at(const size_type index) {
        if (index >= size()) throw std::out_of_range("Index out of range");
        return (*this)[index];
    }

    const_reference at(const size_type index) const {
        if (index >= size()) throw std::out_of_range("Index out of range");
        return (*this)[index];
    }

    reference front() {
        return (*this)[0];
    }

    const_reference front() const {
        return (*this)[0];
    }

    reference back() {
        return (*this)[size() - 1];
    }

    const_reference back() const {
        return (*this)[size() - 1];
    }

    // Capacity & size
    size_type size() const noexcept {
        return m_size;
    }

    bool empty() const noexcept {
        return m_size == 0;
    }

    size_type max_size() const noexcept {
        return std::numeric_limits<size_type>::max() / BLOCK_SIZE;
    }

    void shrink_to_fit() {
        if (m_size == 0) {
            cleanup();
            return;
        }

        const size_type used_blocks = m_back_block - m_front_block + 1;
        const size_type new_capacity = std::max(static_cast<size_type>(4), used_blocks);

        value_type** new_map = new pointer[new_capacity]();
        for (size_type i = 0; i < used_blocks; ++i) {
            new_map[i] = m_map[m_front_block + i];
        }

        delete[] m_map;
        m_map = new_map;
        m_map_capacity = new_capacity;
        m_front_block = 0;
        m_back_block = used_blocks - 1;
    }

    // Modifiers
    template<typename U>
    void push_front(U&& value) {
        if (m_front_index == 0) {
            allocate_block_front();
            m_front_index = BLOCK_SIZE - 1;
        } else { --m_front_index; }
        m_map[m_front_block][m_front_index] = std::forward<U>(value);
        ++m_size;
    }

    template<typename U>
    void push_back(U&& value) {
        if (m_back_index == BLOCK_SIZE - 1) {
            allocate_block_back();
            m_back_index = 0;
        } else { ++m_back_index; }
        m_map[m_back_block][m_back_index] = std::forward<U>(value);
        ++m_size;
    }

    void pop_front() {
        if (empty()) throw std::out_of_range("Deque is empty");
        if (m_front_index == BLOCK_SIZE - 1) {
            delete[] m_map[m_front_block];
            m_map[m_front_block] = nullptr;
            ++m_front_block;
            --m_num_blocks;
            m_front_index = 0;
        } else { ++m_front_index; }
        if (m_num_blocks == 0) reset_indices();
        --m_size;
    }

    void pop_back() {
        if (empty()) throw std::out_of_range("Deque is empty");
        if (m_back_index == 0) {
            delete[] m_map[m_back_block];
            m_map[m_back_block] = nullptr;
            --m_back_block;
            --m_num_blocks;
            m_back_index = BLOCK_SIZE - 1;
        } else { --m_back_index; }
        if (m_num_blocks == 0) reset_indices();
        --m_size;
    }

    void swap(Deque &other) noexcept {
        std::swap(m_map, other.m_map);
        std::swap(m_map_capacity, other.m_map_capacity);
        std::swap(m_num_blocks, other.m_num_blocks);
        std::swap(m_front_block, other.m_front_block);
        std::swap(m_back_block, other.m_back_block);
        std::swap(m_front_index, other.m_front_index);
        std::swap(m_back_index, other.m_back_index);
        std::swap(m_size, other.m_size);
    }

    void clear() noexcept {
        cleanup();
    }


    void assign(const size_type count, const_reference value) {
        cleanup();
        for (size_type i = 0; i < count; ++i) {
            push_back(value);
        }
    }

    void assign(std::initializer_list<value_type> i_list) {
        *this = i_list;
    }

    template<typename InputIt, typename = std::enable_if_t<it::is_iterator<InputIt>::value>>
    void assign(InputIt first, InputIt last) {
        cleanup();
        for (auto it = first; it != last; ++it) {
            push_back(*it);
        }
    }

    // Iterators
    iterator begin() {
        return iterator(&m_map[m_front_block][m_front_index]);
    }

    const_iterator begin() const {
        return const_iterator(&m_map[m_front_block][m_front_index]);
    }

    const_iterator cbegin() const {
        return const_iterator(&m_map[m_front_block][m_front_index]);
    }

    iterator end() {
        return iterator(&m_map[m_back_block][m_back_index + 1]);
    }

    const_iterator end() const {
        return const_iterator(&m_map[m_back_block][m_back_index + 1]);
    }

    const_iterator cend() const {
        return const_iterator(&m_map[m_back_block][m_back_index + 1]);
    }

    reverse_iterator rbegin() {
        return reverse_iterator(end());
    }

    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(end());
    }

    const_reverse_iterator crbegin() const {
        return const_reverse_iterator(cend());
    }

    reverse_iterator rend() {
        return reverse_iterator(begin());
    }

    const_reverse_iterator rend() const {
        return const_reverse_iterator(begin());
    }

    const_reverse_iterator crend() const {
        return const_reverse_iterator(cbegin());
    }

private:
    value_type **m_map;
    size_type m_map_capacity;
    size_type m_num_blocks;
    size_type m_front_block, m_back_block;
    size_type m_front_index, m_back_index;
    size_type m_size;

    static constexpr size_t TARGET_BLOCK_BYTES = 512;
    static constexpr size_t BLOCK_SIZE = std::max(static_cast<size_type>(1), TARGET_BLOCK_BYTES / sizeof(value_type));

    void allocate_block_front() {
        if (m_front_block == 0) expand_map();
        m_map[--m_front_block] = new value_type[BLOCK_SIZE];
        ++m_num_blocks;
    }

    void allocate_block_back() {
        if (m_back_block == m_map_capacity - 1) expand_map();
        m_map[++m_back_block] = new value_type[BLOCK_SIZE];
        ++m_num_blocks;
    }

    void expand_map() {
        const size_type new_capacity = m_map_capacity * 2;
        value_type **new_map = new pointer[new_capacity]();
        const size_type shift = new_capacity / 4;

        for (size_type i = 0; i < m_map_capacity; ++i) {
            new_map[i + shift] = m_map[i];
        }

        m_front_block += shift;
        m_back_block += shift;
        delete[] m_map;
        m_map = new_map;
        m_map_capacity = new_capacity;
    }

    void reset_indices() {
        m_front_index = BLOCK_SIZE / 2;
        m_back_index = m_front_index - 1;
        m_front_block = m_back_block = m_map_capacity / 2;

        if (m_size == 0) {
            m_map[m_front_block] = new value_type[BLOCK_SIZE];
            m_num_blocks = 1;
        }
    }

    void cleanup() noexcept {
        if (m_map) {
            for (size_type i = 0; i < m_map_capacity; ++i) {
                delete[] m_map[i];
            }
            m_size = 0;
            m_num_blocks = 0;
            reset_indices();
        }
    }
};