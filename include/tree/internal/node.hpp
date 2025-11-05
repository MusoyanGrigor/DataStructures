#pragma once

template<typename T>
struct TNode {
    explicit TNode(const T& val) : value(val) {}

    T value;
    TNode* next = nullptr;
    TNode* prev = nullptr;
};