#pragma once

#include <iostream>

#include "tree/internal/node.hpp"

template<typename T>
class BinaryTree {
public:
    BinaryTree() : m_root(nullptr), m_size(0) {}

    BinaryTree(const BinaryTree& other) : BinaryTree() {
        m_root = copy_node(other.m_root);
        m_size = other.m_size;
    }

    BinaryTree(BinaryTree&& other) noexcept : m_root(other.m_root), m_size(other.m_size) {
        other.m_root = nullptr;
        other.m_size = 0;
    }

    ~BinaryTree() {
        clean_data(m_root);
    }

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

    void clear() {
        clean_data(m_root);
        m_root = nullptr;
        m_size = 0;
    }

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

    const T& max() {
        if (!m_root) throw std::out_of_range("Tree is empty");
        TNode<T>* current = m_root;
        while (current->right) {
            current = current->right;
        }
        return current->value;
    }

    const T& min() {
        if (!m_root) throw std::out_of_range("Tree is empty");
        TNode<T>* current = m_root;
        while (current->left) {
            current = current->left;
        }
        return current->value;
    }

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

    void clean_data(TNode<T>* node) {
        if (!node) return;
        clean_data(node->left);
        clean_data(node->right);
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

    auto copy_node(TNode<T>* node)  {
        if (!node) return nullptr;
        auto new_node = new TNode<T>(node->value);
        new_node->left = copy_node(node->left);
        new_node->right = copy_node(node->right);
        return new_node;
    }
};