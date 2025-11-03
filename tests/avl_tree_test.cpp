#include <catch2/catch_test_macros.hpp>
#include "../src/avl_tree.hpp"

TEST_CASE("AVL tree insert single value") {
    AVLTree tree;
    tree.insert(10);
    REQUIRE(tree.serialize() == "10");
}

TEST_CASE("AVL tree insert multiple values without rotation") {
    AVLTree tree;
    tree.insert(10);
    tree.insert(5);
    tree.insert(15);
    REQUIRE(tree.serialize() == "10,5,15");
}

TEST_CASE("AVL tree insert with left-left rotation") {
    AVLTree tree;
    tree.insert(30);
    tree.insert(20);
    tree.insert(10);
    REQUIRE(tree.serialize() == "20,10,30");
}

TEST_CASE("AVL tree insert with right-right rotation") {
    AVLTree tree;
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);
    REQUIRE(tree.serialize() == "20,10,30");
}

TEST_CASE("AVL tree insert with left-right rotation") {
    AVLTree tree;
    tree.insert(30);
    tree.insert(10);
    tree.insert(20);
    REQUIRE(tree.serialize() == "20,10,30");
}

TEST_CASE("AVL tree insert with right-left rotation") {
    AVLTree tree;
    tree.insert(10);
    tree.insert(30);
    tree.insert(20);
    REQUIRE(tree.serialize() == "20,10,30");
}

TEST_CASE("AVL tree insert complex sequence") {
    AVLTree tree;
    tree.insert(50);
    tree.insert(25);
    tree.insert(75);
    tree.insert(10);
    tree.insert(30);
    tree.insert(60);
    tree.insert(80);
    tree.insert(5);
    tree.insert(15);
    
    std::string result = tree.serialize();
    REQUIRE(!result.empty());
}

TEST_CASE("AVL tree delete leaf node") {
    AVLTree tree;
    tree.insert(10);
    tree.insert(5);
    tree.insert(15);
    tree.remove(5);
    REQUIRE(tree.serialize() == "10,null,15");
}

TEST_CASE("AVL tree delete node with one child") {
    AVLTree tree;
    tree.insert(10);
    tree.insert(5);
    tree.insert(15);
    tree.insert(12);
    tree.remove(15);
    REQUIRE(tree.serialize() == "10,5,12");
}

TEST_CASE("AVL tree delete node with two children") {
    AVLTree tree;
    tree.insert(10);
    tree.insert(5);
    tree.insert(15);
    tree.insert(12);
    tree.insert(20);
    tree.remove(15);
    REQUIRE(tree.serialize() == "10,5,20,null,null,12");
}

TEST_CASE("AVL tree delete root node") {
    AVLTree tree;
    tree.insert(10);
    tree.insert(5);
    tree.insert(15);
    tree.remove(10);
    REQUIRE(tree.serialize() == "15,5");
}

TEST_CASE("AVL tree delete with rebalancing") {
    AVLTree tree;
    tree.insert(50);
    tree.insert(25);
    tree.insert(75);
    tree.insert(10);
    tree.insert(30);
    tree.insert(60);
    tree.insert(80);
    tree.insert(5);
    tree.insert(15);
    
    tree.remove(80);
    tree.remove(75);
    
    std::string result = tree.serialize();
    REQUIRE(!result.empty());
}

TEST_CASE("AVL tree serialize empty tree") {
    AVLTree tree;
    REQUIRE(tree.serialize() == "");
}

TEST_CASE("AVL tree serialize single node") {
    AVLTree tree;
    tree.insert(42);
    REQUIRE(tree.serialize() == "42");
}

TEST_CASE("AVL tree serialize complex tree") {
    AVLTree tree;
    tree.insert(10);
    tree.insert(5);
    tree.insert(15);
    tree.insert(2);
    tree.insert(7);
    tree.insert(12);
    tree.insert(20);
    
    std::string result = tree.serialize();
    REQUIRE(!result.empty());
    REQUIRE(result.find("10") != std::string::npos);
}

TEST_CASE("AVL tree deserialize empty string") {
    AVLTree tree = AVLTree::deserialize("");
    REQUIRE(tree.serialize() == "");
}

TEST_CASE("AVL tree deserialize single node") {
    AVLTree tree = AVLTree::deserialize("42");
    REQUIRE(tree.serialize() == "42");
}

TEST_CASE("AVL tree deserialize simple tree") {
    AVLTree tree = AVLTree::deserialize("10,5,15");
    REQUIRE(tree.serialize() == "10,5,15");
}

TEST_CASE("AVL tree deserialize with null nodes") {
    AVLTree tree = AVLTree::deserialize("10,5,15,null,null,12");
    REQUIRE(tree.serialize() == "10,5,15,null,null,12");
}

TEST_CASE("AVL tree serialize-deserialize roundtrip") {
    AVLTree original;
    original.insert(50);
    original.insert(25);
    original.insert(75);
    original.insert(10);
    original.insert(30);
    original.insert(60);
    original.insert(80);
    
    std::string serialized = original.serialize();
    AVLTree deserialized = AVLTree::deserialize(serialized);
    
    REQUIRE(deserialized.serialize() == serialized);
}

TEST_CASE("AVL tree insert duplicate values") {
    AVLTree tree;
    tree.insert(10);
    tree.insert(10);
    tree.insert(10);
    REQUIRE(tree.serialize() == "10");
}

TEST_CASE("AVL tree delete non-existent value") {
    AVLTree tree;
    tree.insert(10);
    tree.insert(5);
    tree.insert(15);
    tree.remove(100);
    REQUIRE(tree.serialize() == "10,5,15");
}

TEST_CASE("AVL tree operations sequence") {
    AVLTree tree;
    
    tree.insert(100);
    tree.insert(50);
    tree.insert(150);
    tree.insert(25);
    tree.insert(75);
    tree.insert(125);
    tree.insert(175);
    
    tree.remove(25);
    tree.insert(30);
    tree.remove(150);
    
    std::string result = tree.serialize();
    REQUIRE(!result.empty());
}
