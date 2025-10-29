#pragma once
#include <algorithm>
#include <cstddef>


template<typename T>
class Deque {
public:
    Deque() : m_map_capacity(4), m_num_blocks(0) {
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

    ~Deque() {
        for (std::size_t i = 0; i < m_map_capacity; ++i) {
            delete m_map[i];
        }

        delete[] m_map;
    }

    void push_front(const T &value) {
        if (m_front_index == 0) {
            allocate_block_front();
            m_front_index = BLOCK_SIZE - 1;
        } else { --m_front_index; }
        m_map[m_front_block][m_front_index] = value;
    }

    void push_back(const T &value) {
        if (m_back_index == BLOCK_SIZE - 1) {
            allocate_block_back();
            m_back_index = 0;
        } else { ++m_back_index; }
        m_map[m_back_block][m_back_index] = value;
    }

private:
    T **m_map;
    std::size_t m_map_capacity;
    std::size_t m_num_blocks;
    std::size_t m_front_block, m_back_block;
    std::size_t m_front_index, m_back_index;

    static constexpr size_t TARGET_BLOCK_BYTES = 512;
    static constexpr size_t BLOCK_SIZE = std::max(static_cast<std::size_t>(1), TARGET_BLOCK_BYTES / sizeof(T));

    void allocate_block_front() {
        if (m_front_block == 0) expand_map();
        m_map[--m_front_block] = new T[BLOCK_SIZE];
    }

    void allocate_block_back() {
        if (m_back_block == m_map_capacity - 1) expand_map();
        m_map[++m_back_block] = new T[BLOCK_SIZE];
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
};
