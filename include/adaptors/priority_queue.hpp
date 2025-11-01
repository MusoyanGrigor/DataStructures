#pragma once

#include "sequence/vector.hpp"

template<typename T, typename Container = Vector<T>, typename Compare = std::less<T>>
class Priority_queue {
public:
    // Constructors
    Priority_queue() = default;
    Priority_queue(const Priority_queue&) = default;
    Priority_queue(Priority_queue&&) noexcept = default;

    // Assignment operator
    Priority_queue& operator=(const Priority_queue&) = default;
    Priority_queue& operator=(Priority_queue&&) noexcept = default;

    // Destructor
    ~Priority_queue() = default;

    // Capacity
    bool empty() const noexcept { return m_container.empty(); }
    size_t size() const noexcept { return m_container.size(); }

    // Element access
    const T& top() const noexcept { return m_container.front(); }

    // Modifiers
    template<typename U>
    void push(U&& value) {
        auto it = m_container.begin();
        while (it != m_container.end() && !m_compare(value, *it)) {
            ++it;
        }
        m_container.insert(it, std::forward<U>(value));
    }

    void pop() noexcept {
        m_container.erase(m_container.begin());
    }

    void clear() noexcept { m_container.clear(); }

private:
    Container m_container;
    Compare m_compare;
};