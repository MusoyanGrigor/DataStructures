#pragma once

#include <iostream>

#include "internal/nodes/red_black_node.hpp"
#include "adaptors/queue.hpp"

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

    void remove_node(const_reference value) {
        auto z = m_root;

        // Find node
        while (z != NIL && z->value != value) {
            if (value < z->value) {
                z = z->left;
            } else {
                z = z->right;
            }
        }

        if (z == NIL) {
            return; // value not found
        }

        auto y = z;
        auto y_original_color = y->color;
        RBNode<value_type> *x;

        if (z->left == NIL) {
            x = z->right;
            transplant(z, z->right);
        } else if (z->right == NIL) {
            x = z->left;
            transplant(z, z->left);
        } else {
            y = minimum(z->right); // successor
            y_original_color = y->color;
            x = y->right;

            if (y->parent == z) {
                x->parent = y;
            } else {
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }

            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }

        delete z;
        --m_size;

        if (y_original_color == Color::BLACK) {
            delete_fixup(x);
        }
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
    RBNode<value_type> *NIL;
    RBNode<value_type> *m_root;
    size_type m_size;

    void clear_data(RBNode<value_type> *node) {
        if (node != NIL) {
            clear_data(node->left);
            clear_data(node->right);
            delete node;
        }
    }

    void inorder_helper(RBNode<value_type> *node) const {
        if (!node) return;
        inorder_helper(node->left);
        std::cout << node->value << " ";
        inorder_helper(node->right);
    }

    void preorder_helper(RBNode<value_type> *node) const {
        if (!node) return;
        std::cout << node->value << " ";
        preorder_helper(node->left);
        preorder_helper(node->right);
    }

    void postorder_helper(RBNode<value_type> *node) const {
        if (!node) return;
        postorder_helper(node->left);
        postorder_helper(node->right);
        std::cout << node->value << " ";
    }

    void level_order_helper(RBNode<value_type> *node) const {
        if (!node) return;

        Queue<RBNode<value_type> *> q;
        q.push(node);
        while (!q.empty()) {
            auto current = q.front();
            q.pop();
            std::cout << current->value << " ";

            if (current->left) q.push(current->left);
            if (current->right) q.push(current->right);
        }
    }

    void insert_fixup(RBNode<value_type> *node) {
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

    void delete_fixup(RBNode<value_type> *x) {
        while (x != m_root && x->color == Color::BLACK) {
            if (x == x->parent->left) {
                auto sibling = x->parent->right;

                if (sibling->color == Color::RED) {
                    sibling->color = Color::BLACK;
                    x->parent->color = Color::RED;
                    rotate_left(x->parent);
                    sibling = x->parent->right;
                }

                if (sibling->left->color == Color::BLACK &&
                    sibling->right->color == Color::BLACK) {
                    sibling->color = Color::RED;
                    x = x->parent;
                } else {
                    if (sibling->right->color == Color::BLACK) {
                        sibling->left->color = Color::BLACK;
                        sibling->color = Color::RED;
                        rotate_right(sibling);
                        sibling = x->parent->right;
                    }

                    sibling->color = x->parent->color;
                    x->parent->color = Color::BLACK;
                    sibling->right->color = Color::BLACK;
                    rotate_left(x->parent);
                    x = m_root;
                }
            } else {
                // mirror case
                auto w = x->parent->left;

                if (w->color == Color::RED) {
                    w->color = Color::BLACK;
                    x->parent->color = Color::RED;
                    rotate_right(x->parent);
                    w = x->parent->left;
                }

                if (w->right->color == Color::BLACK &&
                    w->left->color == Color::BLACK) {
                    w->color = Color::RED;
                    x = x->parent;
                } else {
                    if (w->left->color == Color::BLACK) {
                        w->right->color = Color::BLACK;
                        w->color = Color::RED;
                        rotate_left(w);
                        w = x->parent->left;
                    }

                    w->color = x->parent->color;
                    x->parent->color = Color::BLACK;
                    w->left->color = Color::BLACK;
                    rotate_right(x->parent);
                    x = m_root;
                }
            }
        }
        x->color = Color::BLACK;
    }

    void transplant(RBNode<value_type> *u, RBNode<value_type> *v) {
        if (u->parent == NIL) {
            m_root = v;
        } else if (u == u->parent->left) {
            u->parent->left = v;
        } else {
            u->parent->right = v;
        }

        if (v != NIL) {
            v->parent = u->parent;
        }
    }

    RBNode<value_type>* minimum(RBNode<value_type>* node) const {
        while (node->left != NIL) {
            node = node->left;
        }
        return node;
    }

    RBNode<value_type> *rotate_left(RBNode<value_type> *x) {
        auto y = x->right;
        x->right = y->left;

        if (y->left != NIL) {
            y->left->parent = x;
        }

        y->parent = x->parent; // Link x's parent to y

        if (x->parent == NIL) {
            m_root = y; // x was root, now y becomes root
        } else if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }

        y->left = x;
        x->parent = y;

        return y;
    }

    RBNode<value_type> *rotate_right(RBNode<value_type> *y) {
        auto x = y->left;
        y->left = x->right;

        if (x->right != NIL) {
            x->right->parent = y;
        }

        x->parent = y->parent; // Link y's parent to x

        if (y->parent == NIL) {
            m_root = x; // y was root, now x becomes root
        } else if (y == y->parent->left) {
            y->parent->left = x;
        } else {
            y->parent->right = x;
        }

        x->right = y;
        y->parent = x;

        return x;
    }
};
