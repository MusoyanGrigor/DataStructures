#pragma once

#include <initializer_list>
#include <ranges>

#include "algorithm.hpp"
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

    // Constructors
    Forward_list() : m_head(nullptr), m_size(0) {
    }

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

    template<typename InputIt, typename = std::enable_if_t<it::is_iterator<InputIt>::value> >
    Forward_list(InputIt first, InputIt last) {
        for (auto it = first; it != last; ++it) {
            push_back(*it);
        }
    }

    Forward_list(std::initializer_list<T> i_list) : m_head(nullptr), m_size(0) {
        Node<T>* tail = nullptr;
        for (const auto &value : i_list) {
            auto new_node = new Node<T>(value);
            if (!m_head) m_head = new_node;
            else tail->next = new_node;
            tail = new_node;
            ++m_size;
        }
    }

    explicit Forward_list(const size_type count) {
        for (size_type i = 0; i < count; ++i) {
            push_front(value_type());
        }
    }

    explicit Forward_list(const size_type count, const T &value) {
        for (size_type i = 0; i < count; ++i) push_back(value);
    }

    // Assignment operator
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
        Node<T>* tail = nullptr;
        for (const auto &value : i_list) {
            auto new_node = new Node<T>(value);
            if (!m_head) m_head = new_node;
            else tail->next = new_node;
            tail = new_node;
            ++m_size;
        }
        return *this;
    }

    // Destructor
    ~Forward_list() { clear_data(); }

    // Element Access
    value_type &front() {
        return m_head->value;
    }

    const_reference front() const {
        return m_head->value;
    }

    // Size
    [[nodiscard]] size_type size() const {
        return m_size;
    }

    [[nodiscard]] bool empty() const noexcept {
        return m_size == 0;
    }

    // Modifiers
    void clear() noexcept {
        clear_data();
    }

    template<typename U>
    void push_front(U &&value) {
        auto new_node = new Node<T>(std::forward<U>(value));
        new_node->next = m_head;
        m_head = new_node;
        ++m_size;
    }

    void pop_front() {
        if (m_head) {
            auto temp = m_head;
            m_head = m_head->next;
            delete temp;
            --m_size;
        }
    }

    template<typename... Args>
    void emplace_front(Args &&... args) {
        auto new_node = new Node<T>(std::forward<Args>(args)...);
        new_node->next = m_head;
        m_head = new_node;
        ++m_size;
    }

    void insert_after(iterator pos, const_reference value) {
        if (!pos.node()) throw std::out_of_range("Iterator out of range");

        auto new_node = new Node<T>(value);
        new_node->next = pos.node()->next;
        pos.node()->next = new_node;
        ++m_size;
    }

    void insert_after(iterator pos, const size_type count, const_reference value) {
        if (!pos.node()) throw std::out_of_range("Iterator out of range");

        for (size_type i = 0; i < count; ++i) {
            auto new_node = new Node<T>(value);
            new_node->next = pos.node()->next;
            pos.node()->next = new_node;
            pos = iterator(new_node);
        }
        m_size += count;
    }

    void insert_after(iterator pos, std::initializer_list<T> i_list) {
        if (!pos.node()) throw std::out_of_range("Iterator out of range");

        const size_type count = i_list.size();
        for (size_type i = 0; i < count; ++i) {
            auto new_node = new Node<T>(i_list.begin()[i]);
            new_node->next = pos.node()->next;
            pos.node()->next = new_node;
            pos = iterator(new_node);
        }
        m_size += count;
    }

    template<typename InputIt, typename = std::enable_if_t<it::is_iterator<InputIt>::value> >
    void insert_after(iterator pos, InputIt first, InputIt last) {
        if (!pos.node()) throw std::out_of_range("Iterator out of range");

        auto current = pos.node();
        for (auto it = first; it != last; ++it) {
            auto new_node = new Node<T>(*it);
            new_node->next = current->next;
            current->next = new_node;
            current = new_node;
            ++m_size;
        }
    }

    void resize(const size_type count, value_type value = value_type()) {
        if (count == m_size) return;

        while (m_size < count) {
            push_back(value);
        }

        while (m_size > count) {
            pop_back();
        }
    }

    void assign(const size_type n, const_reference value) {
        clear_data();
        for (size_type i = 0; i < n; ++i) {
            push_front(value);
        }
    }

    void assign(std::initializer_list<T> i_list) {
        clear_data();
        for (auto i: i_list) push_back(i);
    }

    template<typename InputIt, typename = std::enable_if_t<it::is_iterator<InputIt>::value> >
    void assign(InputIt first, InputIt last) {
        clear_data();
        for (auto it = first; it != last; ++it) {
            push_back(*it);
        }
    }

    // Iterators
    iterator begin() noexcept {
        return iterator(m_head);
    }

    const_iterator begin() const noexcept {
        return const_iterator(m_head);
    }

    const_iterator cbegin() const noexcept {
        return const_iterator(m_head);
    }

    iterator end() noexcept {
        return iterator(nullptr);
    }

    const_iterator end() const noexcept {
        return const_iterator(nullptr);
    }

    const_iterator cend() const noexcept {
        return const_iterator(nullptr);
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

    void push_back(const_reference value) {
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

    void pop_back() {
        if (!m_head) return;

        if (!m_head->next) {
            delete m_head;
            m_head = nullptr;
            m_size = 0;
            return;
        }

        auto temp = m_head;
        while (temp->next->next) {
            temp = temp->next;
        }
        delete temp->next;
        temp->next = nullptr;
        --m_size;
    }
};
