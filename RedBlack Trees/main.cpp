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
    bool color; // RED or BLACK
    Node* left;
    Node* right;
    Node* parent;

    Node(string data) : data(data), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
};

class RedBlackTree {
private:
    Node* root;

    // Left rotate at node x
    void rotateLeft(Node*& root, Node*& x) {
        Node* y = x->right;
        x->right = y->left;
        if (y->left != nullptr)
            y->left->parent = x;

        y->parent = x->parent;
        if (x->parent == nullptr)
            root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;

        y->left = x;
        x->parent = y;
    }

    // Right rotate at node y
    void rotateRight(Node*& root, Node*& y) {
        Node* x = y->left;
        y->left = x->right;
        if (x->right != nullptr)
            x->right->parent = y;

        x->parent = y->parent;
        if (y->parent == nullptr)
            root = x;
        else if (y == y->parent->left)
            y->parent->left = x;
        else
            y->parent->right = x;

        x->right = y;
        y->parent = x;
    }

    // Fix Red-Black Tree after insertion
    void fixViolation(Node*& root, Node*& z) {
        while (z != root && z->parent->color == RED) {
            Node* parent = z->parent;
            Node* grandparent = parent->parent;

            if (parent == grandparent->left) {
                Node* uncle = grandparent->right;

                if (uncle != nullptr && uncle->color == RED) {
                    // Case 1: Uncle is red - recolor
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    grandparent->color = RED;
                    z = grandparent;
                } else {
                    if (z == parent->right) {
                        // Case 2: z is right child - rotate left
                        z = parent;
                        rotateLeft(root, z);
                    }
                    // Case 3: z is left child - rotate right
                    parent = z->parent;
                    grandparent = parent->parent;
                    parent->color = BLACK;
                    grandparent->color = RED;
                    rotateRight(root, grandparent);
                }
            } else {
                // Mirror case: parent is right child
                Node* uncle = grandparent->left;

                if (uncle != nullptr && uncle->color == RED) {
                    // Case 1: Uncle is red - recolor
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    grandparent->color = RED;
                    z = grandparent;
                } else {
                    if (z == parent->left) {
                        // Case 2: z is left child - rotate right
                        z = parent;
                        rotateRight(root, z);
                    }
                    // Case 3: z is right child - rotate left
                    parent = z->parent;
                    grandparent = parent->parent;
                    parent->color = BLACK;
                    grandparent->color = RED;
                    rotateLeft(root, grandparent);
                }
            }
        }

        root->color = BLACK;
    }

    // BST insert helper
    Node* BSTInsert(Node* root, Node* z) {
        if (root == nullptr)
            return z;

        if (strcasecmp(z->data.c_str(), root->data.c_str()) < 0) {
            root->left = BSTInsert(root->left, z);
            root->left->parent = root;
        } else if (strcasecmp(z->data.c_str(), root->data.c_str()) > 0) {
            root->right = BSTInsert(root->right, z);
            root->right->parent = root;
        }

        return root;
    }

    // Inorder traversal
    void inorder(Node* node) {
        if (node == nullptr) return;
        inorder(node->left);
        cout << node->data << " ";
        inorder(node->right);
    }

    int getHeight(Node* node) {
        if (node == nullptr) return 0;
        return 1 + max(getHeight(node->left), getHeight(node->right));
    }

    int getBlackHeight(Node* node) {
        if (node == nullptr) return 0;
        int left = getBlackHeight(node->left);
        return left + (node->color == BLACK ? 1 : 0);
    }

    int countNodes(Node* node) {
        if (node == nullptr) return 0;
        return 1 + countNodes(node->left) + countNodes(node->right);
    }

    Node* searchNode(Node* node, const string& key) {
        if (node == nullptr || strcasecmp(node->data.c_str(), key.c_str()) == 0)
            return node;

        if (strcasecmp(key.c_str(), node->data.c_str()) < 0)
            return searchNode(node->left, key);
        else
            return searchNode(node->right, key);
    }

public:
    RedBlackTree() : root(nullptr) {}

    void insert(const string &data) {
        Node* z = new Node(data);
        root = BSTInsert(root, z);
        fixViolation(root, z);
    }

    bool search(const string& key) {
        return searchNode(root, key) != nullptr;
    }

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
