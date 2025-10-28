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
    Forward_list() : m_dummy(new Node<value_type>()), m_size(0) {}

    Forward_list(const Forward_list &other) : Forward_list() {
        auto tail = m_dummy;
        for (auto current = other.m_dummy->next; current != nullptr; current = current->next) {
            tail->next = new Node<value_type>(current->value);
            tail = tail->next;
            ++m_size;
        }
    }

    Forward_list(Forward_list &&other) noexcept : m_dummy(other.m_dummy), m_size(other.m_size) {
        other.m_dummy = new Node<value_type>();
        other.m_dummy->next = nullptr;
        other.m_size = 0;
    }

    template<typename InputIt, typename = std::enable_if_t<it::is_iterator<InputIt>::value> >
    Forward_list(InputIt first, InputIt last) : Forward_list() {
        auto tail = m_dummy;
        for (auto it = first; it != last; ++it) {
            tail->next = new Node<value_type>(*it);
            tail = tail->next;
            ++m_size;
        }
    }

    Forward_list(std::initializer_list<value_type> i_list) : Forward_list() {
        auto tail = m_dummy;
        for (const auto &value: i_list) {
            tail->next = new Node<value_type>(value);
            tail = tail->next;
            ++m_size;
        }
    }

    explicit Forward_list(const size_type count) : Forward_list() {
        for (size_type i = 0; i < count; ++i) {
            push_front(value_type());
        }
    }

    explicit Forward_list(const size_type count, const_reference value) : Forward_list() {
        for (size_type i = 0; i < count; ++i) push_front(value);
    }

    // Assignment operator
    Forward_list &operator=(const Forward_list &other) {
        if (this != &other) {
            clear_data();
            auto tail = m_dummy;
            for (auto current = other.m_dummy->next; current != nullptr; current = current->next) {
                tail->next = new Node<value_type>(current->value);
                tail = tail->next;
                ++m_size;
            }
        }
        return *this;
    }

    Forward_list &operator=(Forward_list &&other) noexcept {
        if (this != &other) {
            clear_data();
            delete m_dummy;

            m_dummy = other.m_dummy;
            m_size = other.m_size;

            other.m_dummy = new Node<value_type>();
            other.m_size = 0;
        }
        return *this;
    }

    Forward_list &operator=(std::initializer_list<value_type> i_list) {
        clear_data();
        auto tail = m_dummy;
        for (const auto &value: i_list) {
            tail->next = new Node<value_type>(value);
            tail = tail->next;
            ++m_size;
        }
        return *this;
    }

    // Destructor
    ~Forward_list() {
        clear_data();
        delete m_dummy;
        m_dummy = nullptr;
    }

    // Element Access
    value_type &front() {
        if (empty()) throw std::out_of_range("Forward_list is empty");
        return m_dummy->next->value;
    }

    const_reference front() const {
        if (empty()) throw std::out_of_range("Forward_list is empty");
        return m_dummy->next->value;
    }

    // Size
    [[nodiscard]] size_type size() const {
        return m_size;
    }

    [[nodiscard]] size_type max_size() const noexcept {
        return std::numeric_limits<size_type>::max();
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
        auto new_node = new Node<value_type>(std::forward<U>(value));
        new_node->next = m_dummy->next;
        m_dummy->next = new_node;
        ++m_size;
    }

    void pop_front() {
        if (m_dummy->next) {
            auto temp = m_dummy->next;
            m_dummy->next = temp->next;
            delete temp;
            --m_size;
        }
    }

    template<typename... Args>
    void emplace_front(Args &&... args) {
        auto new_node = new Node<value_type>(std::forward<Args>(args)...);
        new_node->next = m_dummy->next;
        m_dummy->next = new_node;
        ++m_size;
    }

    void insert_after(iterator pos, const_reference value) {
        if (!pos.node()) throw std::out_of_range("Iterator out of range");
        auto new_node = new Node<value_type>(value);
        new_node->next = pos.node()->next;
        pos.node()->next = new_node;
        ++m_size;
    }

    void insert_after(iterator pos, const size_type count, const_reference value) {
        if (!pos.node()) throw std::out_of_range("Iterator out of range");

        for (size_type i = 0; i < count; ++i) {
            auto new_node = new Node<value_type>(value);
            new_node->next = pos.node()->next;
            pos.node()->next = new_node;
            pos = iterator(new_node);
        }
        m_size += count;
    }

    void insert_after(iterator pos, std::initializer_list<value_type> i_list) {
        if (!pos.node()) throw std::out_of_range("Iterator out of range");

        const size_type count = i_list.size();
        for (size_type i = 0; i < count; ++i) {
            auto new_node = new Node<value_type>(i_list.begin()[i]);
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
            auto new_node = new Node<value_type>(*it);
            new_node->next = current->next;
            current->next = new_node;
            current = new_node;
            ++m_size;
        }
    }

    void erase_after(iterator pos) {
        if (!pos.node()) throw std::out_of_range("Iterator out of range");
        if (!pos.node()->next) return;

        auto temp = pos.node()->next;
        pos.node()->next = temp->next;
        delete temp;
        --m_size;
    }

    void erase_after(iterator first, iterator last) {
        if (!first.node()) throw std::out_of_range("Iterator out of range");

        auto current = first.node()->next;
        auto stop = last.node();

        while (current != stop) {
            auto temp = current;
            current = current->next;
            delete temp;
            --m_size;
        }

        first.node()->next = stop;
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

    void assign(const size_type count, const_reference value) {
        clear_data();
        for (size_type i = 0; i < count; ++i) {
            push_front(value);
        }
    }

    void assign(std::initializer_list<value_type> i_list) {
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

    void swap(Forward_list& other) noexcept{
        std::swap(m_dummy, other.m_dummy);
        std::swap(m_size, other.m_size);
    }

    // Iterators
    iterator begin() noexcept {
        return iterator(m_dummy->next);
    }

    const_iterator begin() const noexcept {
        return const_iterator(m_dummy->next);
    }

    const_iterator cbegin() const noexcept {
        return const_iterator(m_dummy->next);
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

    iterator before_begin() noexcept {
        return iterator(m_dummy);
    }

    const_iterator before_begin() const noexcept {
        return const_iterator(m_dummy);
    }

    const_iterator cbefore_begin() const noexcept {
        return const_iterator(m_dummy);
    }

    // Relational operators
    auto operator<=>(const Forward_list& other) const {
        return std::lexicographical_compare_three_way(
            begin(), end(),
            other.begin(), other.end()
        );
    }

    bool operator==(const Forward_list & other) const {
        return (*this <=> other) == 0;
    }

    bool operator!=(const Forward_list & other) const {
        return !(*this == other);
    }

private:
    Node<value_type> *m_dummy;
    size_type m_size;

    void clear_data() noexcept {
        auto current = m_dummy->next;
        while (current) {
            auto temp = current;
            current = current->next;
            delete temp;
        }
        m_dummy->next = nullptr;
        m_size = 0;
    }

    void push_back(const_reference value) {
        auto new_node = new Node<value_type>(value);
        auto temp = m_dummy;
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = new_node;
        ++m_size;
    }

    void pop_back() {
        if (!m_dummy->next) return;

        if (!m_dummy->next->next) {
            delete m_dummy->next;
            m_dummy->next = nullptr;
            m_size = 0;
            return;
        }

        auto temp = m_dummy;
        while (temp->next->next) {
            temp = temp->next;
        }

        delete temp->next;
        temp->next = nullptr;
        --m_size;
    }
};

template <typename T>
void Swap(Forward_list<T>& lhs, Forward_list<T>& rhs) {
    lhs.swap(rhs);
}