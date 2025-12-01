#pragma once
#include <utility>

template <typename T1, typename T2>
class Pair {
public:
    using first_type = T1;
    using second_type = T2;

    Pair() : m_first(), m_second() {}

    template <typename U1, typename U2>
    Pair(U1 &&value1, U2 &&value2) : m_first(std::forward<U1>(value1)), m_second(std::forward<U2>(value2)) {}

    Pair(T1&& value1, T2&& value2)
        : m_first(std::move(value1)), m_second(std::move(value2)) {}

    Pair& operator=(const Pair &other) {
        if (this != &other) {
            m_first = other.m_first;
            m_second = other.m_second;
        }
        return *this;
    }

    Pair& operator=(Pair &&other) noexcept {
        if (this != &other) {
            m_first = std::move(other.m_first);
            m_second = std::move(other.m_second);
        }
        return *this;
    }

    T1& first() { return m_first; }
    const T1& first() const { return m_first; }

    T2& second() { return m_second; }
    const T2& second() const { return m_second; }

    void swap(Pair &other) noexcept {
        using std::swap;
        swap(m_first, other.m_first);
        swap(m_second, other.m_second);
    }

    auto operator<=>(const Pair&) const = default;

private:
    T1 m_first;
    T2 m_second;
};

template <typename T1, typename T2>
void swap(Pair<T1, T2> &p1, Pair<T1, T2> &p2) noexcept {
    p1.swap(p2);
}