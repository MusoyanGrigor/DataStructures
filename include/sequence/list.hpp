#pragma once
#include <algorithm>
#include <cstddef>
#include <limits>

#include "../iterator/iterator_utils.hpp"
#include "internal/node.hpp"

template<typename T>
class List {
public:
    using value_type = T;
    using pointer = T *;
    using const_pointer = const T *;
    using reference = T &;
    using const_reference = const T &;
    using size_type = std::size_t;
    using iterator = Bidirectional_iterator<T>;
    using const_iterator = Bidirectional_iterator<const T>;
    using reverse_iterator = Reverse_bidirectional_iterator<T>;
    using const_reverse_iterator = Reverse_bidirectional_iterator<const T>;

    // Constructors
    List() : m_head(nullptr), m_tail(nullptr), m_size(0) {
    }

    List(const List &other) : List() {
        auto temp = other.m_head;
        while (temp) {
            push_back(temp->value);
            temp = temp->next;
        }
    }

    List(List &&other) noexcept : m_head(other.m_head), m_tail(other.m_tail), m_size(other.m_size) {
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
        for (auto &value: i_list) {
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

    // Assignment operator
    List &operator=(const List &other) {
        if (this != &other) {
            clear_data();
            auto temp = other.m_head;
            while (temp) {
                push_back(temp->value);
                temp = temp->next;
            }
        }
        return *this;
    }

    List &operator=(List &&other) noexcept {
        if (this != &other) {
            clear_data();
            m_head = other.m_head;
            m_tail = other.m_tail;
            m_size = other.m_size;
            other.m_head = nullptr;
            other.m_tail = nullptr;
            other.m_size = 0;
        }
        return *this;
    }

    List &operator=(std::initializer_list<value_type> i_list) {
        clear_data();
        for (auto &value: i_list) {
            push_back(value);
        }
        return *this;
    }

    // Destructor
    ~List() { clear_data(); }

    // Element access
    reference front() {
        if (!m_head) throw std::out_of_range("List is empty");
        return m_head->value;
    }

    const_reference front() const {
        if (!m_head) throw std::out_of_range("List is empty");
        return m_head->value;
    }

    reference back() {
        if (!m_head) throw std::out_of_range("List is empty");
        return m_tail->value;
    }

    const_reference back() const {
        if (!m_head) throw std::out_of_range("List is empty");
        return m_tail->value;
    }

    // Size
    [[nodiscard]] size_type size() const noexcept {
        return m_size;
    }

    [[nodiscard]] size_type max_size() const noexcept {
        return std::numeric_limits<size_type>::max();
    }

    [[nodiscard]] bool empty() const noexcept {
        return m_size == 0;
    }

    // Modifiers
    template<typename U>
    void push_front(U &&value) {
        auto new_node = new DNode<value_type>(std::forward<U>(value));
        if (!m_head) {
            m_head = m_tail = new_node;
        } else {
            new_node->next = m_head;
            m_head->prev = new_node;
            m_head = new_node;
        }
        ++m_size;
    }

    template<typename U>
    void push_back(U &&value) {
        auto new_node = new DNode<value_type>(std::forward<U>(value));
        if (!m_head) {
            m_head = m_tail = new_node;
        } else {
            m_tail->next = new_node;
            new_node->prev = m_tail;
            m_tail = new_node;
        }
        ++m_size;
    }

    void pop_front() {
        if (!m_head) throw std::out_of_range("List is empty");
        auto temp = m_head;
        m_head = temp->next;

        if (m_head) m_head->prev = nullptr;
        else m_tail = nullptr;

        delete temp;
        --m_size;
    }

    void pop_back() {
        if (!m_head) throw std::out_of_range("List is empty");
        auto temp = m_tail;
        m_tail = temp->prev;

        if (m_tail) m_tail->next = nullptr;
        else m_head = nullptr;

        delete temp;
        --m_size;
    }

    template<typename U>
    void insert(iterator pos,U&&  value) {
        if (pos == end()) push_back(std::forward<U>(value));
        else if (pos == begin()) push_front(std::forward<U>(value));
        else {
            auto new_node = new DNode<value_type>(std::forward<U>(value));
            DNode<value_type> *current = pos.node();

            new_node->next = current;
            new_node->prev = current->prev;

            if (current->prev) current->prev->next = new_node;
            current->prev = new_node;
            ++m_size;
        }
    }

    void insert(iterator pos, const size_type count, const_reference value) {
        if (pos == end()) {
            for (size_type i = 0; i < count; ++i) push_back(value);
            return;
        }
        if (pos == begin()) {
            for (size_type i = 0; i < count; ++i) push_front(value);
            return;
        }

        auto current = pos.node();
        DNode<value_type> *prev_node = current->prev;

        for (size_type i = 0; i < count; ++i) {
            auto new_node = new DNode<value_type>(value);
            new_node->prev = prev_node;
            if (prev_node) prev_node->next = new_node;
            prev_node = new_node; // move prev_node forward
            ++m_size;
        }

        // Connect the last inserted node to current
        prev_node->next = current;
        current->prev = prev_node;
    }

    void insert(iterator pos, std::initializer_list<value_type> i_list) {
        if (pos == end()) {
            for (auto &value: i_list) push_back(value);
            return;
        }
        if (pos == begin()) {
            for (auto &value: i_list) push_front(value);
            return;
        }

        auto current = pos.node();
        DNode<value_type> *prev_node = current->prev;

        for (auto &value: i_list) {
            auto new_node = new DNode<value_type>(value);
            new_node->prev = prev_node;
            if (prev_node) prev_node->next = new_node;
            prev_node = new_node;
            ++m_size;
        }

        prev_node->next = current;
        current->prev = prev_node;
    }

    template<typename InputIt, typename = std::enable_if_t<it::is_iterator<InputIt>::value> >
    void insert(iterator pos, InputIt first, InputIt last) {
        if (pos == end()) {
            for (auto it = first; it != last; ++it) push_back(*it);
            return;
        }
        if (pos == begin()) {
            for (auto it = first; it != last; ++it) push_front(*it);
            return;
        }

        auto current = pos.node();
        DNode<value_type> *prev_node = current->prev;

        for (auto it = first; it != last; ++it) {
            auto new_node = new DNode<value_type>(*it);
            new_node->prev = prev_node;
            if (prev_node) prev_node->next = new_node;
            prev_node = new_node;
            ++m_size;
        }

        prev_node->next = current;
        current->prev = prev_node;
    }


    template<typename U = value_type>
    void resize(const size_type count, U&& value = U()) {
        while (m_size < count)
            push_back(std::forward<U>(value));
        while (m_size > count)
            pop_back();
    }

    void clear() {
        clear_data();
    }

    void swap(List& other) noexcept {
        std::swap(m_head, other.m_head);
        std::swap(m_tail, other.m_tail);
        std::swap(m_size, other.m_size);
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

    reverse_iterator rbegin() noexcept {
        return reverse_iterator(m_tail);
    }

    reverse_iterator rend() noexcept {
        return reverse_iterator(nullptr);
    }

    const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator(m_tail);
    }

    const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator(nullptr);
    }

    const_reverse_iterator crbegin() const noexcept {
        return const_reverse_iterator(m_tail);
    }

    const_reverse_iterator crend() const noexcept {
        return const_reverse_iterator(nullptr);
    }

    // Relational operators
    auto operator<=>(const List &other) const {
        return std::lexicographical_compare_three_way(begin(), end(), other.begin(), other.end());
    }

    bool operator==(const List &other) const {
        return (*this <=> other) == 0;
    }

    bool operator!=(const List &other) const {
        return !(*this == other);
    }

private:
    DNode<value_type> *m_head;
    DNode<value_type> *m_tail;
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


template<typename T>
void swap(List<T>& lhs, List<T>& rhs) noexcept {
    lhs.swap(rhs);
}