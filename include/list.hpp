#pragma once
#include <algorithm>
#include <cstddef>

#include "algorithm.hpp"
#include "node.hpp"

template<typename T>
class List {
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    using iterator = Bidirectional_iterator<T>;
    using const_iterator = Bidirectional_iterator<const T>;
    using reverse_iterator = Reverse_random_access_iterator<T>;
    using const_reverse_iterator = Reverse_random_access_iterator<const T>;

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

    List(std::initializer_list<value_type> i_list) : List() {
        for (auto& value : i_list) {
            push_back(value);
        }
    }

    explicit List(const size_type count) : List() {
        for (size_type i = 0; i < count; ++i) {
            push_back(value_type());
        }
    }

    List(const size_type count, const_reference value) : List() {
        for (size_type i = 0; i < count; ++i) {
            push_back(value);
        }
    }

    ~List() {clear_data();}

    void push_front(const_reference value) {
        auto new_node = new Node<value_type>(value);
        if (!m_head) {
            m_head = m_tail = new_node;
        } else {
            new_node->next = m_head;
            m_head->prev = new_node;
            m_head = new_node;
        }
        ++m_size;
    }

    void push_back(const_reference value) {
        auto new_node = new Node<value_type>(value);
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
    Node<value_type> *m_head;
    Node<value_type> *m_tail;
    size_type m_size;

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
