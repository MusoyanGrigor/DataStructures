#pragma once

#include <iostream>
#include "internal/node.hpp"
#include "adaptors/queue.hpp"

template<typename T>
class Binary_tree {
public:
    using value_type = T;
    using pointer = T *;
    using const_pointer = const T *;
    using reference = T &;
    using const_reference = const T &;
    using size_type = size_t;

    // Constructors
    Binary_tree() : m_root(nullptr), m_size(0) {
    }

    Binary_tree(const Binary_tree &other) : Binary_tree() {
        m_root = copy_nodes(other.m_root);
        m_size = other.m_size;
    }

    Binary_tree(Binary_tree &&other) noexcept : m_root(other.m_root), m_size(other.m_size) {
        other.m_root = nullptr;
        other.m_size = 0;
    }

    // Assignment operators
    Binary_tree &operator=(const Binary_tree &other) {
        if (this != &other) {
            clear();
            m_root = copy_nodes(other.m_root);
            m_size = other.m_size;
        }
        return *this;
    }

    Binary_tree &operator=(Binary_tree &&other) noexcept {
        if (this != &other) {
            clear();
            m_root = other.m_root;
            m_size = other.m_size;
            other.m_root = nullptr;
            other.m_size = 0;
        }
        return *this;
    }

    // Destructor
    ~Binary_tree() { clear(); }

    // Modifiers
    void insert(const_reference value) {
        auto new_node = new TNode<value_type>(value);
        if (!m_root) {
            m_root = new_node;
            ++m_size;
            return;
        }

        Queue<TNode<value_type> *> q;
        q.push(m_root);
        while (!q.empty()) {
            auto current = q.front();
            q.pop();

            if (!current->left) {
                current->left = new_node;
                ++m_size;
                return;
            }
            q.push(current->left);

            if (!current->right) {
                current->right = new_node;
                ++m_size;
                return;
            }
            q.push(current->right);
        }
    }

    bool remove(const_reference value) {
        if (!m_root) return false;

        if (m_root->value == value && m_size == 1) {
            delete m_root;
            m_root = nullptr;
            m_size = 0;
            return true;
        }

        Queue<TNode<value_type> *> q;
        q.push(m_root);

        TNode<value_type> *node_to_delete = nullptr;
        TNode<value_type> *last_node = nullptr;
        TNode<value_type> *last_node_parent = nullptr;

        while (!q.empty()) {
            auto current = q.front();
            q.pop();
            if (current->value == value) node_to_delete = current;

            if (current->left) {
                last_node_parent = current;
                q.push(current->left);
            }
            if (current->right) {
                last_node_parent = current;
                q.push(current->right);
            }

            last_node = current;
        }

        if (!node_to_delete) return false;
        node_to_delete->value = last_node->value;
        if (last_node_parent) {
            if (last_node_parent->right == last_node)
                last_node_parent->right = nullptr;
            else if (last_node_parent->left == last_node)
                last_node_parent->left = nullptr;
        }
        delete last_node;
        --m_size;
        return true;
    }

    void clear() {
        clear_data(m_root);
        m_root = nullptr;
        m_size = 0;
    }

    // Observers
    [[nodiscard]] size_type size() const {
        return m_size;
    }

    [[nodiscard]] bool empty() const {
        return m_size == 0;
    }

    [[nodiscard]] size_type height() const {
        return height_helper(m_root);
    }

    [[nodiscard]] size_type leaf_count() const {
        return leaf_count_helper(m_root);
    }

    bool contains(const_reference value) const {
        if (!m_root) return false;
        Queue<TNode<value_type> *> q;
        q.push(m_root);
        while (!q.empty()) {
            auto current = q.front();
            q.pop();
            if (current->value == value) return true;
            if (current->left) q.push(current->left);
            if (current->right) q.push(current->right);
        }
        return false;
    }

    // Traversals
    void inorder() const {
        inorder_helper(m_root);
        std::cout << std::endl;
    }

    void preorder() const {
        preorder_helper(m_root);
        std::cout << std::endl;
    }

    void postorder() const {
        postorder_helper(m_root);
        std::cout << std::endl;
    }

    void level_order() const {
        level_order_helper(m_root);
        std::cout << std::endl;
    }

private:
    TNode<value_type> *m_root;
    size_type m_size;

    void clear_data(TNode<value_type> *node) {
        if (!node) return;
        clear_data(node->left);
        clear_data(node->right);
        delete node;
    }

    void inorder_helper(TNode<value_type> *node) const {
        if (!node) return;
        inorder_helper(node->left);
        std::cout << node->value << " ";
        inorder_helper(node->right);
    }

    void preorder_helper(TNode<value_type> *node) const {
        if (!node) return;
        std::cout << node->value << " ";
        preorder_helper(node->left);
        preorder_helper(node->right);
    }

    void postorder_helper(TNode<value_type> *node) const {
        if (!node) return;
        postorder_helper(node->left);
        postorder_helper(node->right);
        std::cout << node->value << " ";
    }

    void level_order_helper(TNode<value_type> *node) const {
        if (!node) return;
        Queue<TNode<value_type> *> q;
        q.push(node);
        while (!q.empty()) {
            auto current = q.front();
            q.pop();
            std::cout << current->value << " ";
            if (current->left) q.push(current->left);
            if (current->right) q.push(current->right);
        }
    }

    size_type height_helper(TNode<value_type> *node) const {
        if (!node) return 0;
        return std::max(height_helper(node->left), height_helper(node->right)) + 1;
    }

    size_type leaf_count_helper(TNode<value_type> *node) const {
        if (!node) return 0;
        if (!node->left && !node->right) return 1;
        return leaf_count_helper(node->left) + leaf_count_helper(node->right);
    }

    TNode<value_type> *copy_nodes(TNode<value_type> *node) {
        if (!node) return nullptr;
        auto new_node = new TNode<value_type>(node->value);
        new_node->left = copy_nodes(node->left);
        new_node->right = copy_nodes(node->right);
        return new_node;
    }
};