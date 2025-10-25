#pragma once

template<typename T>
struct node {
    explicit node(const T &value) : value(value), next(nullptr) {}

    T value;
    node<T> *next;
};