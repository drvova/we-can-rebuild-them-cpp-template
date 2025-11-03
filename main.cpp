#include <iostream>
#include "src/avl_tree.hpp"

int main() {
    AVLTree tree;
    
    std::cout << "AVL Tree Demo\n";
    std::cout << "=============\n\n";
    
    std::cout << "Inserting values: 50, 25, 75, 10, 30, 60, 80\n";
    tree.insert(50);
    tree.insert(25);
    tree.insert(75);
    tree.insert(10);
    tree.insert(30);
    tree.insert(60);
    tree.insert(80);
    
    std::cout << "Serialized tree: " << tree.serialize() << "\n\n";
    
    std::cout << "Deleting value: 25\n";
    tree.remove(25);
    std::cout << "Serialized tree: " << tree.serialize() << "\n\n";
    
    std::string serialized = tree.serialize();
    std::cout << "Creating new tree from serialized data...\n";
    AVLTree newTree = AVLTree::deserialize(serialized);
    std::cout << "New tree serialized: " << newTree.serialize() << "\n\n";
    
    std::cout << "Inserting value 35 into new tree\n";
    newTree.insert(35);
    std::cout << "New tree serialized: " << newTree.serialize() << "\n";
    
    return 0;
}
