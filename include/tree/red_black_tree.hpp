#pragma once

#include <iostream>
#include "internal/nodes/red_black_node.hpp"

template<typename T>
class Red_black_tree {
public:
    using value_type = T;
    using pointer = T *;
    using const_pointer = const T *;
    using reference = T &;
    using const_reference = const T &;
    using size_type = size_t;

    Red_black_tree() {
        NIL = new RBNode<value_type>;
        m_root = NIL;
        m_size = 0;
    }

    ~Red_black_tree() {
        clear_data(m_root);
        delete NIL;
    }

    void insert(const_reference value) {
        auto new_node = new RBNode<value_type>(value, NIL);
        auto parent = NIL;
        auto current = m_root;

        while (current != NIL) {
            parent = current;
            if (new_node->value < current->value) {
                current = current->left;
            } else {
                current = current->right;
            }
        }

        new_node->parent = parent;
        if (parent == NIL) {
            m_root = new_node; // tree was empty
        } else if (new_node->value < parent->value) {
            parent->left = new_node;
        } else {
            parent->right = new_node;
        }

        ++m_size;
        insert_fixup(new_node);
    }

    void insert_fixup(RBNode<value_type>* node) {
        while (node->parent->color == Color::RED) {
            if (node->parent == node->parent->parent->left) {
                auto uncle = node->parent->parent->right;
                if (uncle->color == Color::RED) {
                    // Case 1: Uncle red
                    node->parent->color = Color::BLACK;
                    uncle->color = Color::BLACK;
                    node->parent->parent->color = Color::RED;
                    node = node->parent->parent; // check if the tree is still valid
                } else {
                    if (node == node->parent->right) {
                        // Case 2: Triangle
                        node = node->parent;
                        rotate_left(node);
                    }
                    // Case 3: line
                    node->parent->color = Color::BLACK;
                    node->parent->parent->color = Color::RED;
                    rotate_right(node->parent->parent);
                }
            } else {
                // Mirror case: parent is right child
                auto uncle = node->parent->parent->left;
                if (uncle->color == Color::RED) {
                    // Case 1: Uncle red
                    node->parent->color = Color::BLACK;
                    uncle->color = Color::BLACK;
                    node->parent->parent->color = Color::RED;
                    node = node->parent->parent;
                } else {
                    if (node == node->parent->left) {
                        // Case 2: Triangle
                        node = node->parent;
                        rotate_right(node);
                    }
                    // Case 3: line
                    node->parent->color = Color::BLACK;
                    node->parent->parent->color = Color::RED;
                    rotate_left(node->parent->parent);
                }
            }
        }
        m_root->color = Color::BLACK; // root must be black
    }

    RBNode<value_type>* rotate_left(RBNode<value_type>* x) {}
    RBNode<value_type>* rotate_right(RBNode<value_type>* y) {}

private:
    RBNode<value_type>* NIL;
    RBNode<value_type>* m_root;
    size_type m_size;

    void clear_data(RBNode<value_type>* node) {
        if (node != NIL) {
            clear_data(node->left);
            clear_data(node->right);
            delete node;
        }
    }
};