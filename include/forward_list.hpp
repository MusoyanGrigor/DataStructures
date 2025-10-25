#pragma once

#include <initializer_list>
#include <ranges>
#include "node.hpp"
#include "iterator.hpp"

template<typename T>
class Forward_list {
public:
    using value_type = T;
    using pointer = T *;
    using const_pointer = const T *;
    using reference = T &;
    using const_reference = const T &;
    using size_type = std::size_t;
    using iterator = Forward_iterator<T>;
    using const_iterator = Forward_iterator<const T>;

    Forward_list() : m_head(nullptr), m_size(0) {}

    Forward_list(const Forward_list &other) : m_head(nullptr), m_size(0) {
        auto current = other.m_head;
        while (current) {
            push_back(current->value);
            current = current->next;
        }
    }

    Forward_list(Forward_list &&other) noexcept : m_head(other.m_head), m_size(other.m_size) {
        other.m_head = nullptr;
        other.m_size = 0;
    }

    template<typename InputIt, typename = std::enable_if_t<std::is_base_of_v<input_iterator_tag,
        typename InputIt::iterator_category> > >
    Forward_list(InputIt first, InputIt last) {
        for (auto it = first; it != last; ++it) {
            push_back(*it);
        }
    }

    template<std::input_iterator InputIt>
    Forward_list(InputIt first, InputIt last) {
        for (auto it = first; it != last; ++it) {
            push_back(*it);
        }
    }


    Forward_list(std::initializer_list<T> i_list) {
        for (auto I: i_list)
            push_back(I);
    }

    explicit Forward_list(const size_type count) {
        for (size_type i = 0; i < count; ++i) {
            push_front(value_type());
        }
    }

    explicit Forward_list(const size_type count, const T &value) {
        for (size_type i = 0; i < count; ++i) push_back(value);
    }

    Forward_list &operator=(const Forward_list &other) {
        if (this != &other) {
            clear_data();
            auto current = other.m_head;
            while (current) {
                push_back(current->value);
                current = current->next;
            }
        }
        return *this;
    }

    Forward_list &operator=(Forward_list &&other) noexcept {
        if (this != &other) {
            clear_data();
            m_head = other.m_head;
            m_size = other.m_size;
            other.m_head = nullptr;
            other.m_size = 0;
        }
        return *this;
    }

    Forward_list &operator=(std::initializer_list<T> i_list) {
        clear_data();
        for (auto i: i_list) push_back(i);
        return *this;
    }

    ~Forward_list() { clear_data(); }

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
    Node<value_type> *m_head;
    size_type m_size;

    void clear_data() noexcept {
        while (m_head) {
            auto temp = m_head;
            m_head = m_head->next;
            delete temp;
        }
        m_size = 0;
    }
};
