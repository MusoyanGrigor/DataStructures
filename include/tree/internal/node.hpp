#pragma once

template<typename T>
struct TNode {
    explicit TNode(const T& val) : value(val) {}

    T value;
    TNode* left = nullptr;
    TNode* right = nullptr;
};

template <typename T>
struct AVLNode {
    explicit AVLNode(const T& val) : value(val) {}

    T value;
    AVLNode* left = nullptr;
    AVLNode* right = nullptr;
    int height = 1;
};