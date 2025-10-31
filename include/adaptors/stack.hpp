#pragma once

#include <stack>

#include "sequence/deque.hpp"

template<typename T, typename Container = Deque<T> >
class Stack {
public:
    using value_type = Container::value_type;
    using reference = Container::reference;
    using const_reference = Container::const_reference;
    using size_type = Container::size_type;

    Stack() = default;

    explicit Stack(const Container &container) {
        m_container = container;
    }

    Stack(const Stack &other) {
        m_container = other.m_container;
    }

    Stack(Stack &&other) noexcept {
        m_container = std::move(other.m_container);
    }

    Stack &operator=(const Stack &other) {
        if (this != &other) {
            m_container = other.m_container;
        }
        return *this;
    }

    Stack &operator=(Stack &&other) noexcept {
        if (this != &other) {
            m_container = std::move(other.m_container);
        }
        return *this;
    }

    ~Stack() = default;

    reference top() {
        return m_container.back();
    }

    const_reference top() const {
        return m_container.back();
    }

    bool empty() const {
        return m_container.empty();
    }

    size_type size() const {
        return m_container.size();
    }

    template<typename U>
    void push(U &&value) {
        m_container.push_back(std::forward<U>(value));
    }

    void pop() {
        m_container.pop_back();
    }

    template<typename... Args>
    void emplace(Args &&... args) {
        m_container.emplace_back(std::forward<Args>(args)...);
    }

    void swap(Stack &other) noexcept {
        std::swap(m_container, other.m_container);
    }

private:
    Container m_container;
};

template<typename T>
void swap(Stack<T> &lhs, Stack<T> &rhs) noexcept {
    lhs.swap(rhs);
}