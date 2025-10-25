#pragma once

template<typename T>
struct Node {
    explicit Node(const T &value) : value(value), next(nullptr) {}

    T value;
    Node<T> *next;
};