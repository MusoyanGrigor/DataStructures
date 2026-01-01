#pragma once

template<typename T>
struct AVLNode {
    explicit AVLNode(const T &val) : value(val) {}

    T value;
    AVLNode *left = nullptr;
    AVLNode *right = nullptr;
    int height = 1;
};