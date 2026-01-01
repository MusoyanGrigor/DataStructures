#pragma once

#include <iostream>

#include "internal/nodes/avl_node.hpp"
#include "adaptors/queue.hpp"

template<typename T>
class AVL_tree {
public:
    using value_type = T;
    using pointer = T *;
    using const_pointer = const T *;
    using reference = T &;
    using const_reference = const T &;
    using size_type = size_t;

    // Constructors
    AVL_tree() : m_root(nullptr), m_size(0) {
    }

    AVL_tree(const AVL_tree &other) : AVL_tree() {
        m_root = copy_nodes(other.m_root);
        m_size = other.m_size;
    }

    AVL_tree(AVL_tree &&other) noexcept : m_root(other.m_root), m_size(other.m_size) {
        other.m_root = nullptr;
        other.m_size = 0;
    }

    // Assignment operator
    AVL_tree &operator=(const AVL_tree &other) {
        if (this != &other) {
            clear_data();
            m_root = copy_nodes(other.m_root);
            m_size = other.m_size;
        }
        return *this;
    }

    AVL_tree &operator=(AVL_tree &&other) noexcept {
        if (this != &other) {
            m_root = other.m_root;
            m_size = other.m_size;
            other.m_root = nullptr;
            other.m_size = 0;
        }
        return *this;
    }

    // Destructor
    ~AVL_tree() {
        clear_data(m_root);
    }

    // Modifiers
    void insert(const_reference value) {
        m_root = insert_node(m_root, value);
    }

    void remove(const_reference value) {
        m_root = remove_node(m_root, value);
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
        auto current = m_root;
        while (current) {
            if (value == current->value) return true;
            current = value < current->value ? current->left : current->right;
        }
        return false;
    }

    const_reference max() const {
        if (!m_root) throw std::out_of_range("Tree is empty");
        AVLNode<value_type> *current = m_root;
        while (current->right) {
            current = current->right;
        }
        return current->value;
    }

    const_reference min() const {
        if (!m_root) throw std::out_of_range("Tree is empty");
        AVLNode<value_type> *current = m_root;
        while (current->left) {
            current = current->left;
        }
        return current->value;
    }

    size_type height() const {
        return get_height(m_root);
    }

    size_type leaf_count() const {
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
    AVLNode<value_type> *m_root;
    size_type m_size;

    void clear_data(AVLNode<value_type> *node) {
        if (!node) return;
        clear_data(node->left);
        clear_data(node->right);
        delete node;
    }

    void inorder_helper(AVLNode<value_type> *node) const {
        if (!node) return;
        inorder_helper(node->left);
        std::cout << node->value << " ";
        inorder_helper(node->right);
    }

    void preorder_helper(AVLNode<value_type> *node) const {
        if (!node) return;
        std::cout << node->value << " ";
        preorder_helper(node->left);
        preorder_helper(node->right);
    }

    void postorder_helper(AVLNode<value_type> *node) const {
        if (!node) return;
        postorder_helper(node->left);
        postorder_helper(node->right);
        std::cout << node->value << " ";
    }

    void level_order_helper(AVLNode<value_type> *node) const {
        if (!node) return;

        Queue<AVLNode<value_type> *> q;
        q.push(node);
        while (!q.empty()) {
            auto current = q.front();
            q.pop();
            std::cout << current->value << " ";

            if (current->left) q.push(current->left);
            if (current->right) q.push(current->right);
        }
    }

    AVLNode<value_type> *copy_nodes(AVLNode<value_type> *node) {
        if (!node) return nullptr;

        auto new_node = new AVLNode<value_type>(node->value);
        new_node->height = node->height;
        new_node->left = copy_nodes(node->left);
        new_node->right = copy_nodes(node->right);
        return new_node;
    }

    AVLNode<value_type> *insert_node(AVLNode<value_type> *node, const_reference value) {
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
        if (balance < -1 && value > node->right->value) {
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

    AVLNode<value_type> *remove_node(AVLNode<value_type> *node, const_reference value) {
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

            AVLNode<value_type> *minNode = node->right;
            while (minNode->left) minNode = minNode->left;
            node->value = minNode->value;
            node->right = remove_node(node->right, minNode->value);
        }

        node->height = 1 + std::max(get_height(node->left), get_height(node->right));

        const int balance = get_balance(node);

        // LL case
        if (balance > 1 && get_balance(node->left) >= 0)
            return rotate_right(node);

        // LR case
        if (balance > 1 && get_balance(node->left) < 0) {
            node->left = rotate_left(node->left);
            return rotate_right(node);
        }

        // RR case
        if (balance < -1 && get_balance(node->right) <= 0)
            return rotate_left(node);

        // RL case
        if (balance < -1 && get_balance(node->right) > 0) {
            node->right = rotate_right(node->right);
            return rotate_left(node);
        }

        return node;
    }

    int get_height(AVLNode<value_type> *node) const {
        return node ? node->height : 0;
    }

    int get_balance(AVLNode<value_type> *node) const {
        return node ? get_height(node->left) - get_height(node->right) : 0;
    }

    size_type leaf_count_helper(AVLNode<value_type> *node) const {
        if (!node) return 0;
        if (!node->left && !node->right) return 1;
        return leaf_count_helper(node->left) + leaf_count_helper(node->right);
    }

    AVLNode<value_type> *rotate_left(AVLNode<value_type> *x) {
        auto y = x->right;
        auto T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = 1 + std::max(get_height(x->left), get_height(x->right));
        y->height = 1 + std::max(get_height(y->left), get_height(y->right));

        return y;
    }

    AVLNode<value_type> *rotate_right(AVLNode<value_type> *y) {
        AVLNode<value_type> *x = y->left;
        AVLNode<value_type> *T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = 1 + std::max(get_height(y->left), get_height(y->right));
        x->height = 1 + std::max(get_height(x->left), get_height(x->right));

        return x;
    }
};
