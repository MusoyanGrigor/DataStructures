#pragma once
#include <cstddef>

#include "node.hpp"

template<typename T>
class List {
public:
    List() : m_head(nullptr), m_tail(nullptr), m_size(0) {
    }

    ~List() {clear_data();}

    void push_front(const T &value) {
        auto new_node = new Node<T>(value);
        if (!m_head) {
            m_head = m_tail = new_node;
        } else {
            new_node->next = m_head;
            m_head->prev = new_node;
            m_head = new_node;
        }
        ++m_size;
    }

    void push_back(const T &value) {
        auto new_node = new Node<T>(value);
        if (!m_head) {
            m_head = m_tail = new_node;
        } else {
            m_tail->next = new_node;
            new_node->prev = m_tail;
            m_tail = new_node;
        }
        ++m_size;
    }

private:
    Node<T> *m_head;
    Node<T> *m_tail;
    std::size_t m_size;

    void clear_data() {
        while (m_head) {
            auto temp = m_head;
            m_head = m_head->next;
            delete temp;
        }
        m_head = m_tail = nullptr;
        m_size = 0;
    }
};
