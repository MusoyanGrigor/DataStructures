#pragma once
#include <algorithm>
#include <cstddef>
#include <stdexcept>

template<typename T>
class Deque {
public:
    Deque() : m_map_capacity(4), m_num_blocks(0), m_size(0) {
        m_map = new T *[m_map_capacity];
        for (std::size_t i = 0; i < m_map_capacity; ++i) {
            m_map[i] = nullptr;
        }

        m_front_block = m_back_block = m_map_capacity / 2;
        m_map[m_front_block] = new T[BLOCK_SIZE];
        ++m_num_blocks;

        m_front_index = BLOCK_SIZE / 2;
        m_back_index = m_front_index - 1;
    }

    Deque(const Deque &other) : m_map_capacity(other.m_map_capacity), m_num_blocks(other.m_num_blocks),
    m_front_block(other.m_front_block), m_back_block(other.m_back_block), m_front_index(other.m_front_index),
    m_back_index(other.m_back_index), m_size(other.m_size) {
        m_map = new T *[m_map_capacity];
        for (std::size_t i = 0; i < m_map_capacity; ++i) {
            if (other.m_map[i]) {
                m_map[i] = new T[BLOCK_SIZE];
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

    ~Deque() {
        cleanup();
    }

    void push_front(const T &value) {
        if (m_front_index == 0) {
            allocate_block_front();
            m_front_index = BLOCK_SIZE - 1;
        } else { --m_front_index; }
        m_map[m_front_block][m_front_index] = value;
        ++m_size;
    }

    void push_back(const T &value) {
        if (m_back_index == BLOCK_SIZE - 1) {
            allocate_block_back();
            m_back_index = 0;
        } else { ++m_back_index; }
        m_map[m_back_block][m_back_index] = value;
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

    bool empty() const noexcept {
        return m_size == 0;
    }

    std::size_t size() const noexcept {
       return m_size;
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

private:
    T **m_map;
    std::size_t m_map_capacity;
    std::size_t m_num_blocks;
    std::size_t m_front_block, m_back_block;
    std::size_t m_front_index, m_back_index;
    std::size_t m_size;

    static constexpr size_t TARGET_BLOCK_BYTES = 512;
    static constexpr size_t BLOCK_SIZE = std::max(static_cast<std::size_t>(1), TARGET_BLOCK_BYTES / sizeof(T));

    void allocate_block_front() {
        if (m_front_block == 0) expand_map();
        m_map[--m_front_block] = new T[BLOCK_SIZE];
        ++m_num_blocks;
    }

    void allocate_block_back() {
        if (m_back_block == m_map_capacity - 1) expand_map();
        m_map[++m_back_block] = new T[BLOCK_SIZE];
        ++m_num_blocks;
    }

    void expand_map() {
        const std::size_t new_capacity = m_map_capacity * 2;
        T **new_map = new T *[new_capacity];
        const std::size_t shift = new_capacity / 4;
        for (std::size_t i = 0; i < shift; ++i) {
            new_map[i] = nullptr;
        }

        for (std::size_t i = 0; i < m_map_capacity; ++i) {
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
            m_map[m_front_block] = new T[BLOCK_SIZE];
            m_num_blocks = 1;
        }
    }

    void cleanup() noexcept {
        if (m_map) {
            for (std::size_t i = 0; i < m_map_capacity; ++i) {
                delete[] m_map[i];
            }
            delete[] m_map;
        }
    }
};