#pragma once

#include <iostream>

#include "tree/internal/node.hpp"
#include "adaptors/queue.hpp"

template<typename T>
class Binary_search_tree {
public:
    using value_type = T;
    using pointer = T *;
    using const_pointer = const T *;
    using reference = T &;
    using const_reference = const T &;
    using size_type = size_t;

    // Constructors
    Binary_search_tree() : m_root(nullptr), m_size(0) {
    }

    Binary_search_tree(const Binary_search_tree &other) : Binary_search_tree() {
        m_root = copy_nodes(other.m_root);
        m_size = other.m_size;
    }

    Binary_search_tree(Binary_search_tree &&other) noexcept : m_root(other.m_root), m_size(other.m_size) {
        other.m_root = nullptr;
        other.m_size = 0;
    }

    // Assignment operator
    Binary_search_tree &operator=(const Binary_search_tree &other) {
        if (this != &other) {
            clear_data();
            m_root = copy_nodes(other.m_root);
            m_size = other.m_size;
        }
        return *this;
    }

    Binary_search_tree &operator=(Binary_search_tree &&other) noexcept {
        if (this != &other) {
            clear_data();
            m_root = other.m_root;
            m_size = other.m_size;
            other.m_root = nullptr;
            other.m_size = 0;
        }
        return *this;
    }

    // Destructor
    ~Binary_search_tree() {
        clear_data(m_root);
    }

    // Modifiers
    void insert(const_reference value) {
        auto new_node = new TNode<value_type>(value);
        if (!m_root) {
            m_root = new_node;
            ++m_size;
            return;
        }

        TNode<value_type> *parent = nullptr;
        TNode<value_type> *current = m_root;

        while (current) {
            parent = current;
            if (value < current->value) {
                current = current->left;
            } else {
                current = current->right;
            }
        }

        if (value < parent->value) {
            parent->left = new_node;
        } else {
            parent->right = new_node;
        }
        ++m_size;
    }

    auto remove(const_reference value) {
        return remove_node(m_root, value);
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

    bool contains(const_reference value) const {
        TNode<value_type> *current = m_root;
        while (current) {
            if (current->value == value) return true;
            if (value < current->value) {
                current = current->left;
            } else {
                current = current->right;
            }
        }
        return false;
    }

    const_reference max() const {
        if (!m_root) throw std::out_of_range("Tree is empty");
        TNode<value_type> *current = m_root;
        while (current->right) {
            current = current->right;
        }
        return current->value;
    }

    const_reference min() const {
        if (!m_root) throw std::out_of_range("Tree is empty");
        TNode<value_type> *current = m_root;
        while (current->left) {
            current = current->left;
        }
        return current->value;
    }

    [[nodiscard]] size_type height() const {
        return height_helper(m_root);
    }

    [[nodiscard]] size_type leaf_count() const {
        return leaf_count_helper(m_root);
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
        const size_type left_height = height_helper(node->left);
        const size_type right_height = height_helper(node->right);
        return std::max(left_height, right_height) + 1;
    }

    size_type leaf_count_helper(TNode<value_type> *node) const {
        if (!node) return 0;
        if (!node->left && !node->right) return 1;
        return leaf_count_helper(node->left) + leaf_count_helper(node->right);
    }

    auto copy_nodes(TNode<value_type> *node) {
        if (!node) return nullptr;
        auto new_node = new TNode<value_type>(node->value);
        new_node->left = copy_nodes(node->left);
        new_node->right = copy_nodes(node->right);
        return new_node;
    }

    TNode<value_type> *remove_node(TNode<value_type> *node, const_reference value) {
        if (!node) return nullptr;

        if (value < node->value) {
            node->left = remove_node(node->left, value);
        } else if (value > node->value) {
            node->right = remove_node(node->right, value);
        } else {
            if (!node->left) {
                auto rightChild = node->right;
                delete node;
                --m_size;
                return rightChild;
            }
            if (!node->right) {
                auto leftChild = node->left;
                delete node;
                --m_size;
                return leftChild;
            }

            TNode<value_type> *minNode = node->right;
            while (minNode->left) minNode = minNode->left;
            node->value = minNode->value;
            node->right = remove_node(node->right, node->value);
        }
        return node;
    }
};
