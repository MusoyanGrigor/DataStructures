#pragma once

template<typename T>
struct TNode {
    explicit TNode(const T &val) : value(val) {}

    T value;
    TNode *left = nullptr;
    TNode *right = nullptr;
};