#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <cstring>

using namespace std;

const string DICTIONARY_FILE = "../Dictionary.txt";
enum Color { RED, BLACK };

struct Node {
    string data;
    bool color;
    Node *left, *right, *parent;

    Node(string data): data(data), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
};

class RedBlackTree {
private:
    Node* root;

    void rotateLeft(Node*& root, Node*& pt) {
        Node* pt_right = pt->right;
        pt->right = pt_right->left;
        if (pt->right != nullptr)
            pt->right->parent = pt;

        pt_right->parent = pt->parent;
        if (pt->parent == nullptr)
            root = pt_right;
        else if (pt == pt->parent->left)
            pt->parent->left = pt_right;
        else
            pt->parent->right = pt_right;

        pt_right->left = pt;
        pt->parent = pt_right;
    }

    void rotateRight(Node*& root, Node*& pt) {
        Node* pt_left = pt->left;
        pt->left = pt_left->right;
        if (pt->left != nullptr)
            pt->left->parent = pt;

        pt_left->parent = pt->parent;
        if (pt->parent == nullptr)
            root = pt_left;
        else if (pt == pt->parent->left)
            pt->parent->left = pt_left;
        else
            pt->parent->right = pt_left;

        pt_left->right = pt;
        pt->parent = pt_left;
    }

    void fixViolation(Node*& root, Node*& pt) {
        Node* parent_pt = nullptr;
        Node* grand_parent_pt = nullptr;

        while ((pt != root) && (pt->color != BLACK) && (pt->parent->color == RED)) {
            parent_pt = pt->parent;
            grand_parent_pt = pt->parent->parent;

            if (parent_pt == grand_parent_pt->left) {
                Node* uncle_pt = grand_parent_pt->right;

                if (uncle_pt != nullptr && uncle_pt->color == RED) {
                    grand_parent_pt->color = RED;
                    parent_pt->color = BLACK;
                    uncle_pt->color = BLACK;
                    pt = grand_parent_pt;
                } else {
                    if (pt == parent_pt->right) {
                        rotateLeft(root, parent_pt);
                        pt = parent_pt;
                        parent_pt = pt->parent;
                    }
                    rotateRight(root, grand_parent_pt);
                    swap(parent_pt->color, grand_parent_pt->color);
                    pt = parent_pt;
                }
            } else {
                Node* uncle_pt = grand_parent_pt->left;

                if ((uncle_pt != nullptr) && (uncle_pt->color == RED)) {
                    grand_parent_pt->color = RED;
                    parent_pt->color = BLACK;
                    uncle_pt->color = BLACK;
                    pt = grand_parent_pt;
                } else {
                    if (pt == parent_pt->left) {
                        rotateRight(root, parent_pt);
                        pt = parent_pt;
                        parent_pt = pt->parent;
                    }
                    rotateLeft(root, grand_parent_pt);
                    swap(parent_pt->color, grand_parent_pt->color);
                    pt = parent_pt;
                }
            }
        }
        root->color = BLACK;
    }

    void inorder(Node* root) {
        if (root == nullptr) return;
        inorder(root->left);
        cout << root->data << " ";
        inorder(root->right);
    }

    Node* BSTInsert(Node* root, Node* pt) {
        if (root == nullptr)
            return pt;

        if (strcasecmp(pt->data.c_str(), root->data.c_str()) < 0) {
            root->left = BSTInsert(root->left, pt);
            root->left->parent = root;
        } else if (strcasecmp(pt->data.c_str(), root->data.c_str()) > 0) {
            root->right = BSTInsert(root->right, pt);
            root->right->parent = root;
        }

        return root;
    }

    int getHeight(Node* node) {
        if (node == nullptr) return 0;
        return 1 + max(getHeight(node->left), getHeight(node->right));
    }

    int getBlackHeight(Node* node) {
        if (node == nullptr) return 0;
        int leftHeight = getBlackHeight(node->left);
        return leftHeight + (node->color == BLACK ? 1 : 0);
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
        Node* pt = new Node(data);
        root = BSTInsert(root, pt);
        fixViolation(root, pt);
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

RedBlackTree loadDictionary(const string &filename);
void insertWord(RedBlackTree &tree, const string &word);
void lookupWord(RedBlackTree &tree, const string &word);

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

RedBlackTree loadDictionary(const string &filename) {
    RedBlackTree tree;
    ifstream infile(filename);
    string line;
    if (!infile) {
        cerr << "Error: Could not open file '" << filename << "'" << endl;
        return tree;
    }

    while (getline(infile, line)) {
        if (!line.empty()) {
            if (!tree.search(line))
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
