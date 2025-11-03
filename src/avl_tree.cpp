#include "avl_tree.hpp"
#include <algorithm>
#include <sstream>
#include <vector>
#include <queue>

int AVLTree::getHeight(const Node* node) const {
    return node ? node->height : 0;
}

int AVLTree::getBalance(const Node* node) const {
    return node ? getHeight(node->left.get()) - getHeight(node->right.get()) : 0;
}

std::unique_ptr<AVLTree::Node> AVLTree::rotateLeft(std::unique_ptr<Node> x) {
    std::unique_ptr<Node> y = std::move(x->right);
    x->right = std::move(y->left);
    x->height = 1 + std::max(getHeight(x->left.get()), getHeight(x->right.get()));
    y->left = std::move(x);
    y->height = 1 + std::max(getHeight(y->left.get()), getHeight(y->right.get()));
    return y;
}

std::unique_ptr<AVLTree::Node> AVLTree::rotateRight(std::unique_ptr<Node> y) {
    std::unique_ptr<Node> x = std::move(y->left);
    y->left = std::move(x->right);
    y->height = 1 + std::max(getHeight(y->left.get()), getHeight(y->right.get()));
    x->right = std::move(y);
    x->height = 1 + std::max(getHeight(x->left.get()), getHeight(x->right.get()));
    return x;
}

std::unique_ptr<AVLTree::Node> AVLTree::insertNode(std::unique_ptr<Node> node, int value) {
    if (!node) {
        return std::make_unique<Node>(value);
    }
    
    if (value < node->value) {
        node->left = insertNode(std::move(node->left), value);
    } else if (value > node->value) {
        node->right = insertNode(std::move(node->right), value);
    } else {
        return node;
    }
    
    node->height = 1 + std::max(getHeight(node->left.get()), getHeight(node->right.get()));
    int balance = getBalance(node.get());
    
    if (balance > 1 && value < node->left->value) {
        return rotateRight(std::move(node));
    }
    
    if (balance < -1 && value > node->right->value) {
        return rotateLeft(std::move(node));
    }
    
    if (balance > 1 && value > node->left->value) {
        node->left = rotateLeft(std::move(node->left));
        return rotateRight(std::move(node));
    }
    
    if (balance < -1 && value < node->right->value) {
        node->right = rotateRight(std::move(node->right));
        return rotateLeft(std::move(node));
    }
    
    return node;
}

AVLTree::Node* AVLTree::minValueNode(Node* node) const {
    Node* current = node;
    while (current && current->left) {
        current = current->left.get();
    }
    return current;
}

std::unique_ptr<AVLTree::Node> AVLTree::deleteNode(std::unique_ptr<Node> node, int value) {
    if (!node) {
        return nullptr;
    }
    
    if (value < node->value) {
        node->left = deleteNode(std::move(node->left), value);
    } else if (value > node->value) {
        node->right = deleteNode(std::move(node->right), value);
    } else {
        if (!node->left || !node->right) {
            return node->left ? std::move(node->left) : std::move(node->right);
        }
        
        Node* temp = minValueNode(node->right.get());
        node->value = temp->value;
        node->right = deleteNode(std::move(node->right), temp->value);
    }
    
    node->height = 1 + std::max(getHeight(node->left.get()), getHeight(node->right.get()));
    int balance = getBalance(node.get());
    
    if (balance > 1 && getBalance(node->left.get()) >= 0) {
        return rotateRight(std::move(node));
    }
    
    if (balance > 1 && getBalance(node->left.get()) < 0) {
        node->left = rotateLeft(std::move(node->left));
        return rotateRight(std::move(node));
    }
    
    if (balance < -1 && getBalance(node->right.get()) <= 0) {
        return rotateLeft(std::move(node));
    }
    
    if (balance < -1 && getBalance(node->right.get()) > 0) {
        node->right = rotateRight(std::move(node->right));
        return rotateLeft(std::move(node));
    }
    
    return node;
}

void AVLTree::insert(int value) {
    root = insertNode(std::move(root), value);
}

void AVLTree::remove(int value) {
    root = deleteNode(std::move(root), value);
}

void AVLTree::serializeHelper(const Node* node, std::string& result) const {
    std::queue<const Node*> q;
    q.push(node);
    
    while (!q.empty()) {
        const Node* current = q.front();
        q.pop();
        
        if (current) {
            if (!result.empty()) result += ",";
            result += std::to_string(current->value);
            q.push(current->left.get());
            q.push(current->right.get());
        } else {
            if (!result.empty()) result += ",";
            result += "null";
        }
    }
    
    while (result.size() >= 5 && result.substr(result.size() - 5) == ",null") {
        result = result.substr(0, result.size() - 5);
    }
}

std::string AVLTree::serialize() const {
    if (!root) return "";
    
    std::string result;
    serializeHelper(root.get(), result);
    return result;
}

std::unique_ptr<AVLTree::Node> AVLTree::deserializeHelper(const std::string& data, size_t& index) const {
    if (index >= data.size()) return nullptr;
    
    size_t comma = data.find(',', index);
    std::string token = (comma == std::string::npos) ? data.substr(index) : data.substr(index, comma - index);
    index = (comma == std::string::npos) ? data.size() : comma + 1;
    
    if (token == "null") return nullptr;
    
    return std::make_unique<Node>(std::stoi(token));
}

AVLTree AVLTree::deserialize(const std::string& str) {
    AVLTree tree;
    if (str.empty()) return tree;
    
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    
    while (std::getline(ss, token, ',')) {
        tokens.push_back(token);
    }
    
    if (tokens.empty() || tokens[0] == "null") return tree;
    
    tree.root = std::make_unique<Node>(std::stoi(tokens[0]));
    std::queue<Node*> q;
    q.push(tree.root.get());
    
    size_t i = 1;
    while (!q.empty() && i < tokens.size()) {
        Node* current = q.front();
        q.pop();
        
        if (i < tokens.size() && tokens[i] != "null") {
            current->left = std::make_unique<Node>(std::stoi(tokens[i]));
            q.push(current->left.get());
        }
        i++;
        
        if (i < tokens.size() && tokens[i] != "null") {
            current->right = std::make_unique<Node>(std::stoi(tokens[i]));
            q.push(current->right.get());
        }
        i++;
    }
    
    return tree;
}
