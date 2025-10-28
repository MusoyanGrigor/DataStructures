#pragma once

template<typename T>
struct Node {
    Node() = default;

    explicit Node(const T& val) : value(val) {}
    explicit Node(T&& val) : value(std::move(val)) {}

    template<typename... Args>
    explicit Node(Args&&... args) : value(std::forward<Args>(args)...) {}

    T value;
    Node* next = nullptr;
};

template<typename T>
struct DNode {
    DNode() = default;

    explicit DNode(const T& val) : value(val) {}
    explicit DNode(T&& val) : value(std::move(val)) {}

    template<typename... Args>
    explicit DNode(Args&&... args) : value(std::forward<Args>(args)...) {}

    T value;
    DNode* next = nullptr;
    DNode* prev = nullptr;
};
