#pragma once

#include "sequence/deque.hpp"

template<typename T, typename Container = Deque<T>>
class Queue {
public:
    using value_type = Container::value_type;
    using reference = Container::reference;
    using const_reference = Container::const_reference;
    using size_type = Container::size_type;

    Queue() = default;

    explicit Queue(const Container& container) {
        m_container = container;
    }

    Queue(const Queue& other) {
        m_container = other.m_container;
    }

    Queue(Queue&& other) noexcept {
        m_container = std::move(other.m_container);
    }

    Queue& operator=(const Queue& other) {
        if (this != &other) {
            m_container = other.m_container;
        }
        return *this;
    }

    Queue& operator=(Queue&& other) noexcept {
        if (this != &other) {
            m_container = std::move(other.m_container);
        }
        return *this;
    }

    ~Queue() = default;

    reference front() {
        return m_container.front();
    }

    const_reference front() const {
        return m_container.front();
    }

    reference back() {
        return m_container.back();
    }

    const_reference back() const {
        return m_container.back();
    }

    bool empty() const {
        return m_container.empty();
    }

    size_type size() const {
        return m_container.size();
    }

    template<typename U>
    void push(U&& value) {
        m_container.push_back(std::forward<U>(value));
    }

    void pop() {
        m_container.pop_front();
    }

    template<typename... Args>
    void emplace(Args&&... args) {
        return m_container.emplace_back(std::forward<Args>(args)...);
    }

    void swap(Queue& other) noexcept {
        std::swap(m_container, other.m_container);
    }

private:
    Container m_container;
};

template<typename T>
void swap(Queue<T> &lhs, Queue<T> &rhs) noexcept {
    lhs.swap(rhs);
}