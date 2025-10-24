#pragma once

#include "iterator_tags.hpp"
#include "iterator_base.hpp"

// Input iterator
template<typename T>
class Input_iterator : public Iterator<input_iterator_tag, T> {
public:
    explicit Input_iterator(T *p = nullptr) : m_ptr(p) {
    }

    T &operator*() const {
        return *m_ptr;
    }

    Input_iterator &operator++() {
        ++m_ptr;
        return *this;
    }

    Input_iterator operator++(int) {
        Input_iterator tmp = *this;
        ++(*this);
        return tmp;
    }

    bool operator==(const Input_iterator &other) const {
        return m_ptr == other.m_ptr;
    }

    bool operator!=(const Input_iterator &other) const {
        return m_ptr != other.m_ptr;
    }

private:
    T *m_ptr;
};

// Output iterator
template<typename T>
class Output_iterator : public Iterator<output_iterator_tag, T> {
public:
    explicit Output_iterator(T *p = nullptr) : m_ptr(p) {
    }

    Output_iterator &operator=(const T &value) {
        *m_ptr = value;
        return *this;
    }

    T &operator*() const {
        return *m_ptr;
    }

    Output_iterator &operator++() {
        ++m_ptr;
        return *this;
    }

    Output_iterator operator++(int) {
        Output_iterator tmp = *this;
        ++(*this);
        return tmp;
    }

private:
    T *m_ptr;
};

// Forward iterator
template<typename T>
class Forward_iterator : public Iterator<forward_iterator_tag, T> {
public:
    explicit Forward_iterator(T *p = nullptr) : m_ptr(p) {
    }

    T &operator*() const {
        return *m_ptr;
    }

    Forward_iterator &operator++() {
        ++m_ptr;
        return *this;
    }

    Forward_iterator operator++(int) {
        Forward_iterator tmp = *this;
        ++(*this);
        return tmp;
    }

    bool operator==(const Forward_iterator &other) const {
        return m_ptr == other.m_ptr;
    }

    bool operator!=(const Forward_iterator &other) const {
        return m_ptr != other.m_ptr;
    }

private:
    T *m_ptr;
};

// Bidirectional iterator
template<typename T>
class Bidirectional_iterator : public Iterator<bidirectional_iterator_tag, T> {
public:
    explicit Bidirectional_iterator(T *p = nullptr) : m_ptr(p) {
    }

    T &operator*() const {
        return *m_ptr;
    }

    Bidirectional_iterator &operator++() {
        ++m_ptr;
        return *this;
    }

    Bidirectional_iterator operator++(int) {
        Bidirectional_iterator tmp = *this;
        ++(*this);
        return tmp;
    }

    Bidirectional_iterator &operator--() {
        --m_ptr;
        return *this;
    }

    Bidirectional_iterator operator--(int) {
        Bidirectional_iterator tmp = *this;
        --(*this);
        return tmp;
    }

    bool operator==(const Bidirectional_iterator &other) const {
        return m_ptr == other.m_ptr;
    }

    bool operator!=(const Bidirectional_iterator &other) const {
        return m_ptr != other.m_ptr;
    }

private:
    T *m_ptr;
};

// Random access iterator
template<typename T>
class Random_access_iterator : public Iterator<random_access_iterator_tag, T> {
public:
    explicit Random_access_iterator(T *p = nullptr) : m_ptr(p) {
    }

    T &operator*() const {
        return *m_ptr;
    }

    Random_access_iterator &operator++() {
        ++m_ptr;
        return *this;
    }

    Random_access_iterator operator++(int) {
        Random_access_iterator tmp = *this;
        ++(*this);
        return tmp;
    }

    Random_access_iterator &operator--() {
        --m_ptr;
        return *this;
    }

    Random_access_iterator operator--(int) {
        Random_access_iterator tmp = *this;
        --(*this);
        return tmp;
    }

    Random_access_iterator operator+(std::ptrdiff_t n) const {
        return Random_access_iterator(m_ptr + n);
    }

    Random_access_iterator operator-(std::ptrdiff_t n) const {
        return Random_access_iterator(m_ptr - n);
    }

    std::ptrdiff_t operator-(const Random_access_iterator &other) const {
        return m_ptr - other.m_ptr;
    }

    T &operator[](std::ptrdiff_t n) const {
        return m_ptr[n];
    }

    bool operator<(const Random_access_iterator &other) const {
        return m_ptr < other.m_ptr;
    }

    bool operator<=(const Random_access_iterator &other) const {
        return m_ptr <= other.m_ptr;
    }

    bool operator>(const Random_access_iterator &other) const {
        return m_ptr > other.m_ptr;
    }

    bool operator>=(const Random_access_iterator &other) const {
        return m_ptr >= other.m_ptr;
    }

    bool operator==(const Random_access_iterator &other) const {
        return m_ptr == other.m_ptr;
    }

    bool operator!=(const Random_access_iterator &other) const {
        return m_ptr != other.m_ptr;
    }

private:
    T *m_ptr;
};

template<typename T>
class Reverse_bidirectional_iterator : public Iterator<bidirectional_iterator_tag, T> {
public:
    explicit Reverse_bidirectional_iterator(T *p = nullptr) : m_ptr(p) {
    }

    T &operator*() const {
        return *(m_ptr - 1);
    }

    Reverse_bidirectional_iterator &operator++() {
        --m_ptr;
        return *this;
    }

    Reverse_bidirectional_iterator operator++(int) {
        Reverse_bidirectional_iterator tmp = *this;
        --(*this);
        return tmp;
    }

    Reverse_bidirectional_iterator &operator--() {
        ++m_ptr;
        return *this;
    }

    Reverse_bidirectional_iterator operator--(int) {
        Reverse_bidirectional_iterator tmp = *this;
        ++(*this);
        return tmp;
    }

    T* operator->() const {
        return m_ptr - 1;
    }

    bool operator==(const Reverse_bidirectional_iterator &other) const {
        return m_ptr == other.m_ptr;
    }

    bool operator!=(const Reverse_bidirectional_iterator &other) const {
        return m_ptr != other.m_ptr;
    }

private:
    T *m_ptr;
};

template <typename T>
class Reverse_random_access_iterator {
public:
    explicit Reverse_random_access_iterator(T *p = nullptr) : m_ptr(p) {}

    T &operator*() const {
        return *(m_ptr - 1);
    }

    Reverse_random_access_iterator &operator++() {
        --m_ptr;
        return *this;
    }

    Reverse_random_access_iterator operator++(int) {
        Reverse_random_access_iterator tmp = *this;
        --(*this);
        return tmp;
    }

    Reverse_random_access_iterator &operator--() {
        ++m_ptr;
        return *this;
    }

    Reverse_random_access_iterator operator--(int) {
        Reverse_random_access_iterator tmp = *this;
        --(*this);
        return tmp;
    }

    Reverse_random_access_iterator operator+(std::ptrdiff_t n) const {
        return Reverse_random_access_iterator(m_ptr - n);
    }

    Reverse_random_access_iterator operator-(std::ptrdiff_t n) const {
        return Reverse_random_access_iterator(m_ptr + n);
    }

    std::ptrdiff_t operator-(const Reverse_random_access_iterator &other) const {
        return other.m_ptr - m_ptr;
    }

    T& operator[](std::ptrdiff_t n) const {
        return *(*this + n);
    }

    T* operator->() const { return m_ptr - 1; }

    bool operator<(const Reverse_random_access_iterator &other) const {
        return m_ptr > other.m_ptr;
    }

    bool operator<=(const Reverse_random_access_iterator &other) const {
        return m_ptr >= other.m_ptr;
    }

    bool operator>(const Reverse_random_access_iterator &other) const {
        return m_ptr < other.m_ptr;
    }

    bool operator>=(const Reverse_random_access_iterator &other) const {
        return m_ptr <= other.m_ptr;
    }

    bool operator==(const Reverse_random_access_iterator &other) const {
        return m_ptr == other.m_ptr;
    }

    bool operator!=(const Reverse_random_access_iterator &other) const {
        return m_ptr != other.m_ptr;
    }


private:
    T* m_ptr;
};
