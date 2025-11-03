#pragma once

#include <string>
#include <memory>

class AVLTree {
private:
    struct Node {
        int value;
        int height;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;
        
        explicit Node(int val) : value(val), height(1), left(nullptr), right(nullptr) {}
    };
    
    std::unique_ptr<Node> root;
    
    int getHeight(const Node* node) const;
    int getBalance(const Node* node) const;
    std::unique_ptr<Node> rotateLeft(std::unique_ptr<Node> node);
    std::unique_ptr<Node> rotateRight(std::unique_ptr<Node> node);
    std::unique_ptr<Node> insertNode(std::unique_ptr<Node> node, int value);
    std::unique_ptr<Node> deleteNode(std::unique_ptr<Node> node, int value);
    Node* minValueNode(Node* node) const;
    void serializeHelper(const Node* node, std::string& result) const;
    std::unique_ptr<Node> deserializeHelper(const std::string& data, size_t& index) const;
    
public:
    AVLTree() : root(nullptr) {}
    
    void insert(int value);
    void remove(int value);
    std::string serialize() const;
    static AVLTree deserialize(const std::string& str);
};
