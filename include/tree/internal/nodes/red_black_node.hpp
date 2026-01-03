#pragma once

enum class Color { RED, BLACK };

template<typename T>
struct RBNode {

    // Constructor for NIL node
    RBNode()
        : color(Color::BLACK),
          parent(this),
          left(this),
          right(this) {}

    // Constructor for regular nodes
    explicit RBNode(const T &val, RBNode *nil)
        : value(val),
          color(Color::RED),
          parent(nil),
          left(nil),
          right(nil) {}

    T value;
    Color color;
    RBNode *parent;
    RBNode *left;
    RBNode *right;
};
