#pragma once

#include <iostream>
#include "internal/node.hpp"

template <typename T>
class AVL_tree {
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = size_t;

    AVL_tree() : m_root(nullptr), m_size(0) {}

private:
    AVLNode<value_type> m_root;
    size_type m_size;
};