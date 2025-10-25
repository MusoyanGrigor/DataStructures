#pragma once

#include <cstddef>

#include "node.hpp"

template <typename T>
class Forward_list {
public:
    Forward_list() : m_head(nullptr), m_size(0) {}

    ~Forward_list() {
        clear_data();
    }

    void push_front(const T &value) {
        auto new_node = new Node<T>(value);
        new_node->next = m_head;
        m_head = new_node;
        ++m_size;
    }

    void push_back(const T &value) {
        auto new_node = new Node<T>(value);
       if (!m_head) {
           m_head = new_node;
       } else {
            auto temp = m_head;
            while (temp->next) {
               temp = temp->next;
           }
           temp->next = new_node;
       }
        ++m_size;
    }



private:
    Node<T>* m_head;
    std::size_t m_size;

    void clear_data() noexcept {
        while (m_head) {
            auto temp = m_head;
            m_head = m_head->next;
            delete temp;
        }
    }
};