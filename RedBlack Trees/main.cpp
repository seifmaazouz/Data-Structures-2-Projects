#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
using namespace std;

const string DICTIONARY_FILE = "../Dictionary.txt";
enum Color { RED, BLACK };

// Node structure
struct Node {
    string data;
    bool color;
    Node* left;
    Node* right;
    Node* parent;

    Node(string data, Node* nil) : data(data), color(RED), left(nil), right(nil), parent(nil) {}
};

class RedBlackTree {
private:
    Node* root;
    Node* nil; // Shared sentinel node for all null pointers

    // Rotate left at node
    void rotateLeft(Node*& root, Node*& z) {
        Node* rightChild = z->right;
        z->right = rightChild->left;

        if (rightChild->left != nil)
            rightChild->left->parent = z;

        rightChild->parent = z->parent;

        if (z->parent == nil)
            root = rightChild;
        else if (z == z->parent->left)
            z->parent->left = rightChild;
        else
            z->parent->right = rightChild;

        rightChild->left = z;
        z->parent = rightChild;
    }

    // Rotate right at node
    void rotateRight(Node*& root, Node*& z) {
        Node* leftChild = z->left;
        z->left = leftChild->right;

        if (leftChild->right != nil)
            leftChild->right->parent = z;

        leftChild->parent = z->parent;

        if (z->parent == nil)
            root = leftChild;
        else if (z == z->parent->left)
            z->parent->left = leftChild;
        else
            z->parent->right = leftChild;

        leftChild->right = z;
        z->parent = leftChild;
    }

    // Fix Red-Black Tree after insertion
    void fixViolation(Node*& root, Node*& z) {
        while (z->parent->color == RED) {
            Node* parent = z->parent;
            Node* grandparent = parent->parent;

            if (parent == grandparent->left) {
                Node* uncle = grandparent->right;

                if (uncle->color == RED) {
                    // Case 1: Uncle is red → recolor and move up
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    grandparent->color = RED;
                    z = grandparent;
                } else {
                    if (z == parent->right) {
                        // Case 2: node is right child → rotate to become left child
                        z = parent;
                        rotateLeft(root, z);
                    }
                    // Case 3: node is left child → rotate right and recolor
                    parent = z->parent;
                    grandparent = parent->parent;
                    parent->color = BLACK;
                    grandparent->color = RED;
                    rotateRight(root, grandparent);
                }
            } else {
                // Mirror case (parent is right child of grandparent)
                Node* uncle = grandparent->left;

                if (uncle->color == RED) {
                    // Case 1
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    grandparent->color = RED;
                    z = grandparent;
                } else {
                    if (z == parent->left) {
                        // Case 2
                        z = parent;
                        rotateRight(root, z);
                    }
                    // Case 3
                    parent = z->parent;
                    grandparent = parent->parent;
                    parent->color = BLACK;
                    grandparent->color = RED;
                    rotateLeft(root, grandparent);
                }
            }
        }
        root->color = BLACK; // Root must always be black
    }

    // Standard BST insert
    Node* BSTInsert(Node* current, Node* newNode) {
        if (current == nil)
            return newNode;

        if (strcasecmp(newNode->data.c_str(), current->data.c_str()) < 0) {
            current->left = BSTInsert(current->left, newNode);
            current->left->parent = current;
        } else if (strcasecmp(newNode->data.c_str(), current->data.c_str()) > 0) {
            current->right = BSTInsert(current->right, newNode);
            current->right->parent = current;
        }

        return current;
    }

    // Inorder traversal to print sorted words
    void inorder(Node* node) {
        if (node == nil) return;
        inorder(node->left);
        cout << node->data << " ";
        inorder(node->right);
    }

    // Get total height of tree
    int getHeight(Node* node) {
        if (node == nil) return 0;
        return 1 + max(getHeight(node->left), getHeight(node->right));
    }

    // Get black height (number of black nodes along a path)
    int getBlackHeight(Node* node) {
        if (node == nil) return 0;
        int left = getBlackHeight(node->left);
        return left + (node->color == BLACK ? 1 : 0);
    }

    // Count total nodes
    int countNodes(Node* node) {
        if (node == nil) return 0;
        return 1 + countNodes(node->left) + countNodes(node->right);
    }

    // Search for a word
    Node* searchNode(Node* node, const string& key) {
        if (node == nil || strcasecmp(node->data.c_str(), key.c_str()) == 0)
            return node;

        if (strcasecmp(key.c_str(), node->data.c_str()) < 0)
            return searchNode(node->left, key);
        else
            return searchNode(node->right, key);
    }

public:
    RedBlackTree() {
        // Initialize nil node
        nil = new Node("", nullptr);
        nil->color = BLACK;
        nil->left = nil->right = nil->parent = nil;

        root = nil;
    }

    // Insert a new word
    void insert(const string &data) {
        Node* newNode = new Node(data, nil);
        root = BSTInsert(root, newNode);
        fixViolation(root, newNode);
    }

    // Search for a word
    bool search(const string& key) {
        return searchNode(root, key) != nil;
    }

    // Print tree properties
    void printTreeHeight() {
        cout << "Tree Height: " << getHeight(root) << endl;
    }

    void printBlackHeight() {
        cout << "Black Height: " << getBlackHeight(root) << endl;
    }

    void printTreeSize() {
        cout << "Tree Size: " << countNodes(root) << endl;
    }

    void printInorder() {
        cout << "Inorder Traversal: ";
        inorder(root);
        cout << endl;
    }
};

// Load dictionary from file
RedBlackTree loadDictionary(const string &filename) {
    RedBlackTree tree;
    ifstream infile(filename);
    string line;

    if (!infile) {
        cerr << "Error: Could not open file '" << filename << "'" << endl;
        return tree;
    }

    while (getline(infile, line)) {
        if (!line.empty() && !tree.search(line)) {
            tree.insert(line);
        }
    }

    infile.close();
    cout << "Dictionary loaded successfully!\n" << endl;
    tree.printTreeSize();
    tree.printTreeHeight();
    tree.printBlackHeight();
    return tree;
}

void insertWord(RedBlackTree &tree, const string &word) {
    if (tree.search(word)) {
        cout << "ERROR: Word already in the dictionary!" << endl;
        return;
    }

    tree.insert(word);

    ofstream outfile(DICTIONARY_FILE, ios::app);
    if (outfile.is_open()) {
        outfile << "\n" << word;
        cout << "Word inserted successfully!" << endl;
        outfile.close();
    } else {
        cout << "ERROR: Could not open file for writing!" << endl;
    }

    tree.printTreeSize();
    tree.printTreeHeight();
    tree.printBlackHeight();
}

void lookupWord(RedBlackTree &tree, const string &word) {
    if (tree.search(word)) {
        cout << "Word found!" << endl;
    } else {
        cout << "Word not found!" << endl;
    }
}

int main() {
    RedBlackTree tree = loadDictionary(DICTIONARY_FILE);

    while (true) {
        cout << "\nChoose an option (1, 2, 3)" << endl;
        cout << "1. Insert a word" << endl;
        cout << "2. Lookup a word" << endl;
        cout << "3. Exit" << endl;

        cout << "Enter your choice: ";
        int choice;
        cin >> choice;

        switch (choice) {
            case 1: {
                cout << "Enter the word to insert: ";
                string word;
                cin >> word;
                insertWord(tree, word);
                break;
            }
            case 2: {
                cout << "Enter the word to lookup: ";
                string word;
                cin >> word;
                lookupWord(tree, word);
                break;
            }
            case 3:
                cout << "Exiting..." << endl;
                return 0;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    }
}
