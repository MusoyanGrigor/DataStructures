#pragma once
#include <algorithm>
#include <cstddef>

#include "algorithm.hpp"
#include "node.hpp"

template<typename T>
class List {
public:
    List() : m_head(nullptr), m_tail(nullptr), m_size(0) {}

    List(const List& other) : List() {
        auto temp = other.m_head;
        while (temp) {
            push_back(temp->value);
            temp = temp->next;
        }
    }

    List(List&& other) noexcept : m_head(other.m_head), m_tail(other.m_tail), m_size(other.m_size) {
        other.m_head = nullptr;
        other.m_tail = nullptr;
        other.m_size = 0;
    }

    template<typename InputIt, typename = std::enable_if_t<it::is_iterator<InputIt>::value> >
    List(InputIt first, InputIt last) : List() {
        for (auto it = first; it != last; ++it) {
            push_back(*it);
        }
    }

    List(std::initializer_list<T> i_list) : List() {
        for (auto& value : i_list) {
            push_back(value);
        }
    }

    explicit List(const std::size_t count) : List() {
        for (std::size_t i = 0; i < count; ++i) {
            push_back(T());
        }
    }

    List(const std::size_t count, const T& value) : List() {
        for (std::size_t i = 0; i < count; ++i) {
            push_back(value);
        }
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
