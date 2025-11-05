#pragma once

#include <iostream>

#include "tree/internal/node.hpp"

template<typename T>
class Binary_tree {
public:
    // Constructors
    Binary_tree() : m_root(nullptr), m_size(0) {}

    Binary_tree(const Binary_tree& other) : Binary_tree() {
        m_root = copy_nodes(other.m_root);
        m_size = other.m_size;
    }

    Binary_tree(Binary_tree&& other) noexcept : m_root(other.m_root), m_size(other.m_size) {
        other.m_root = nullptr;
        other.m_size = 0;
    }

    // Assignment operator
    Binary_tree& operator=(const Binary_tree& other) {
        if (this != &other) {
            clear_data();
            m_root = copy_nodes(other.m_root);
            m_size = other.m_size;
        }
        return *this;
    }

    Binary_tree& operator=(Binary_tree&& other) noexcept {
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
    ~Binary_tree() {
        clear_data(m_root);
    }

    // Modifiers
    void insert(const T& value) {
        auto new_node = new TNode<T>(value);
        if (!m_root) {
            m_root = new_node;
            ++m_size;
            return;
        }

        TNode<T>* parent = nullptr;
        TNode<T>* current = m_root;

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

    auto remove(const T& value) {
        return remove_node(m_root, value);
    }

    void clear() {
        clear_data(m_root);
        m_root = nullptr;
        m_size = 0;
    }


    // Observers
    [[nodiscard]] std::size_t size() const {
        return m_size;
    }

    [[nodiscard]] bool empty() const {
        return m_size == 0;
    }

    bool contains(const T& value) const {
        TNode<T>* current = m_root;
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

    const T& max() const {
        if (!m_root) throw std::out_of_range("Tree is empty");
        TNode<T>* current = m_root;
        while (current->right) {
            current = current->right;
        }
        return current->value;
    }

    const T& min() const {
        if (!m_root) throw std::out_of_range("Tree is empty");
        TNode<T>* current = m_root;
        while (current->left) {
            current = current->left;
        }
        return current->value;
    }

    [[nodiscard]] std::size_t height() const {
        return height_helper(m_root);
    }

    [[nodiscard]] std::size_t leaf_count() const {
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

private:
    TNode<T>* m_root;
    std::size_t m_size;

    void clear_data(TNode<T>* node) {
        if (!node) return;
        clear_data(node->left);
        clear_data(node->right);
        delete node;
    }

    void inorder_helper(TNode<T>* node) const{
        if (!node) return;
        inorder_helper(node->left);
        std::cout << node->value << " ";
        inorder_helper(node->right);
    }

    void preorder_helper(TNode<T>* node) const {
        if (!node) return;
        std::cout << node->value << " ";
        preorder_helper(node->left);
        preorder_helper(node->right);
    }

    void postorder_helper(TNode<T>* node) const {
        if (!node) return;
        postorder_helper(node->left);
        postorder_helper(node->right);
        std::cout << node->value << " ";
    }

    std::size_t height_helper(TNode<T>* node) const {
        if (!node) return 0;
        const std::size_t left_height = height_helper(node->left);
        const std::size_t right_height = height_helper(node->right);
        return std::max(left_height, right_height) + 1;
    }

    std::size_t leaf_count_helper(TNode<T>* node) const {
        if (!node) return 0;
        if (!node->left && !node->right) return 1;
        return leaf_count_helper(node->left) + leaf_count_helper(node->right);
    }

    auto copy_nodes(TNode<T>* node)  {
        if (!node) return nullptr;
        auto new_node = new TNode<T>(node->value);
        new_node->left = copy_nodes(node->left);
        new_node->right = copy_nodes(node->right);
        return new_node;
    }

    TNode<T>* remove_node(TNode<T>* node, const T& value) {
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

            TNode<T>* minNode = node->right;
            while (minNode->left) minNode = minNode->left;
            node->value = minNode->value;
            node->right = remove_node(node->right, node->value);
        }
        return node;
    }
};