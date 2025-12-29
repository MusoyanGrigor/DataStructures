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

    void insert(const_reference value) {
        m_root = insert_node(m_root, value);
    }

private:
    AVLNode<value_type>* m_root;
    size_type m_size;

    AVLNode<value_type>* insert_node(AVLNode<value_type>* node, const_reference value) {
        if (!node) {
            ++m_size;
            return new AVLNode<value_type>(value);
        }

        if (value < node->value) {
            node->left = insert_node(node->left, value);
        } else if (value > node->value) {
            node->right = insert_node(node->right, value);
        } else {
            return node; // duplicates are not allowed
        }

        node->height = 1 + std::max(get_height(node->left), get_height(node->right));

        const int balance = get_balance(node);

        // LL case
        if (balance > 1 && value < node->left->value) {
            return rotate_right(node);
        }

        // RR case
        if (balance < - 1 && value > node->right->value) {
            return rotate_left(node);
        }

        // LR case
        if (balance > 1 && value > node->left->value) {
            node->left = rotate_left(node->left);
            return rotate_right(node);
        }

        // RL case
        if (balance < -1 && value < node->right->value) {
            node->right = rotate_right(node);
            return rotate_left(node);
        }

        return node;
    }

    int get_height(AVLNode<value_type>* node) const {
        return node ? node->height : 0;
    }

    int get_balance(AVLNode<value_type>* node) const {
        return node ? get_height(node->left) - get_height(node->right) : 0;
    }

    AVLNode<value_type>* rotate_left(AVLNode<value_type>* x) {
        auto y = x->right;
        auto T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = 1 + std::max(get_height(x->left), get_height(x->right));
        y->height = 1 + std::max(get_height(y->left), get_height(y->right));

        return y;
    }

    AVLNode<value_type>* rotate_right(AVLNode<value_type>* y) {
        AVLNode<value_type>* x = y->left;
        AVLNode<value_type>* T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = 1 + std::max(get_height(y->left), get_height(y->right));
        x->height = 1 + std::max(get_height(x->left), get_height(x->right));

        return x;
    }

};