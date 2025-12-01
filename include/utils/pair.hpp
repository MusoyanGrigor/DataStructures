#pragma once
#include <utility>

template <typename T1, typename T2>
class Pair {
public:
    Pair() : m_first(), m_second() {}

    Pair(const T1 &value1, const T2 &value2)
        : m_first(value1), m_second(value2) {}

    Pair(T1&& value1, T2&& value2)
        : m_first(std::move(value1)), m_second(std::move(value2)) {}

    T1& first() { return m_first; }
    const T1& first() const { return m_first; }

    T2& second() { return m_second; }
    const T2& second() const { return m_second; }

private:
    T1 m_first;
    T2 m_second;
};