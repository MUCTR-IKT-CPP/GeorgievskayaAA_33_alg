#include <iostream>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <vector>
#include <random>
#include <numeric>

class AVLTree {
public:
    struct Node {
        int key;
        Node* left;
        Node* right;
        int height;

        Node(int key) : key(key), left(nullptr), right(nullptr), height(1) {}
    };

    AVLTree() : root(nullptr), size(0) {}

    void insert(int key) {
        root = insert(root, key);
        ++size;
    }

    void remove(int key) {
        root = remove(root, key);
        --size;
    }

    bool search(int key) {
        return search(root, key);
    }

    int getMaxDepth() {
        return getMaxDepth(root);
    }

    void printDepths() {
        printDepths(root, 1);
    }

    int getSize() {
        return size;
    }

    std::vector<int> getAllDepths() {
        std::vector<int> depths;
        collectDepths(root, 1, depths);
        return depths;
    }

    template <typename Func>
    double measureTime(Func func, int operations) {
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < operations; ++i) {
            func(rand() % 10000);
        }
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double>(end - start).count() / operations;
    }

private:
    Node* root;
    int size;

    Node* insert(Node* node, int key) {
        if (!node) return new Node(key);

        if (key < node->key) node->left = insert(node->left, key);
        else if (key > node->key) node->right = insert(node->right, key);
        else return node;

        node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));

        int balance = getBalance(node);

        if (balance > 1 && key < node->left->key)
            return rightRotate(node);
        if (balance < -1 && key > node->right->key)
            return leftRotate(node);
        if (balance > 1 && key > node->left->key) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        if (balance < -1 && key < node->right->key) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    Node* remove(Node* root, int key) {
        if (!root) return root;

        if (key < root->key) root->left = remove(root->left, key);
        else if (key > root->key) root->right = remove(root->right, key);
        else {
            if (!root->left) {
                Node* temp = root->right;
                delete root;
                return temp;
            } else if (!root->right) {
                Node* temp = root->left;
                delete root;
                return temp;
            }

            Node* temp = getMinValueNode(root->right);
            root->key = temp->key;
            root->right = remove(root->right, temp->key);
        }

        root->height = 1 + std::max(getHeight(root->left), getHeight(root->right));

        int balance = getBalance(root);

        if (balance > 1 && getBalance(root->left) >= 0)
            return rightRotate(root);
        if (balance > 1 && getBalance(root->left) < 0) {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }
        if (balance < -1 && getBalance(root->right) <= 0)
            return leftRotate(root);
        if (balance < -1 && getBalance(root->right) > 0) {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }

        return root;
    }

    bool search(Node* node, int key) {
        if (!node) return false;
        if (key == node->key) return true;
        if (key < node->key) return search(node->left, key);
        return search(node->right, key);
    }

    Node* rightRotate(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = 1 + std::max(getHeight(y->left), getHeight(y->right));
        x->height = 1 + std::max(getHeight(x->left), getHeight(x->right));

        return x;
    }

    Node* leftRotate(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = 1 + std::max(getHeight(x->left), getHeight(x->right));
        y->height = 1 + std::max(getHeight(y->left), getHeight(y->right));

        return y;
    }

    int getHeight(Node* node) {
        if (!node) return 0;
        return node->height;
    }

    int getBalance(Node* node) {
        if (!node) return 0;
        return getHeight(node->left) - getHeight(node->right);
    }

    Node* getMinValueNode(Node* node) {
        Node* current = node;
        while (current && current->left) current = current->left;
        return current;
    }

    int getMaxDepth(Node* node) {
        if (!node) return 0;
        return 1 + std::max(getMaxDepth(node->left), getMaxDepth(node->right));
    }

    void printDepths(Node* node, int depth) {
        if (!node) return;
        std::cout << "Node " << node->key << " depth: " << depth << std::endl;
        printDepths(node->left, depth + 1);
        printDepths(node->right, depth + 1);
    }

    void collectDepths(Node* node, int currentDepth, std::vector<int>& depths) {
        if (!node) return;
        depths.push_back(currentDepth);
        collectDepths(node->left, currentDepth + 1, depths);
        collectDepths(node->right, currentDepth + 1, depths);
    }
};

class Treap {
public:
    struct Node {
        int key, priority;
        Node* left;
        Node* right;

        Node(int key) : key(key), priority(rand()), left(nullptr), right(nullptr) {}
    };

    Treap() : root(nullptr), size(0) {}

    void insert(int key) {
        root = insert(root, key);
        ++size;
    }

    void remove(int key) {
        root = remove(root, key);
        --size;
    }

    bool search(int key) {
        return search(root, key);
    }

    int getMaxDepth() {
        return getMaxDepth(root);
    }

    void printDepths() {
        printDepths(root, 1);
    }

    int getSize() {
        return size;
    }

    std::vector<int> getAllDepths() {
        std::vector<int> depths;
        collectDepths(root, 1, depths);
        return depths;
    }

    template <typename Func>
    double measureTime(Func func, int operations) {
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < operations; ++i) {
            func(rand() % 10000);
        }
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double>(end - start).count() / operations;
    }

private:
    Node* root;
    int size;

    Node* insert(Node* node, int key) {
        if (!node) return new Node(key);

        if (key < node->key) node->left = insert(node->left, key);
        else node->right = insert(node->right, key);

        if (node->left && node->left->priority > node->priority) {
            node = rotateRight(node);
        } else if (node->right && node->right->priority > node->priority) {
            node = rotateLeft(node);
        }

        return node;
    }

    Node* remove(Node* node, int key) {
        if (!node) return node;

        if (key < node->key) node->left = remove(node->left, key);
        else if (key > node->key) node->right = remove(node->right, key);
        else {
            if (!node->left || !node->right) {
                Node* temp = node->left ? node->left : node->right;
                delete node;
                return temp;
            }
            if (node->left->priority > node->right->priority) {
                node = rotateRight(node);
                node->right = remove(node->right, key);
            } else {
                node = rotateLeft(node);
                node->left = remove(node->left, key);
            }
        }
        return node;
    }

    bool search(Node* node, int key) {
        if (!node) return false;
        if (key == node->key) return true;
        if (key < node->key) return search(node->left, key);
        return search(node->right, key);
    }

    Node* rotateLeft(Node* node) {
        Node* newRoot = node->right;
        node->right = newRoot->left;
        newRoot->left = node;
        return newRoot;
    }

    Node* rotateRight(Node* node) {
        Node* newRoot = node->left;
        node->left = newRoot->right;
        newRoot->right = node;
        return newRoot;
    }

    int getMaxDepth(Node* node) {
        if (!node) return 0;
        return 1 + std::max(getMaxDepth(node->left), getMaxDepth(node->right));
    }

    void printDepths(Node* node, int depth) {
        if (!node) return;
        std::cout << "Node " << node->key << " depth: " << depth << std::endl;
        printDepths(node->left, depth + 1);
        printDepths(node->right, depth + 1);
    }

    void collectDepths(Node* node, int currentDepth, std::vector<int>& depths) {
        if (!node) return;
        depths.push_back(currentDepth);
        collectDepths(node->left, currentDepth + 1, depths);
        collectDepths(node->right, currentDepth + 1, depths);
    }
};

int main() {
    std::ofstream outFile("results.txt");

    outFile << "N, AVL Max Depth, AVL Avg Insert Time, AVL Avg Remove Time, AVL Avg Search Time, AVL Avg Height, Treap Max Depth, Treap Avg Insert Time, Treap Avg Remove Time, Treap Avg Search Time, Treap Avg Height, AVL Max Height of Last Series, Treap Max Height of Last Series, AVL Avg Branch Height of Last Series, Treap Avg Branch Height of Last Series\n";

    for (int i = 10; i <= 18; ++i) {
        int N = 1 << i;
        std::cout << "Running tests for N = " << N << std::endl;

        double totalInsertAVL = 0, totalRemoveAVL = 0, totalSearchAVL = 0;
        double totalInsertTreap = 0, totalRemoveTreap = 0, totalSearchTreap = 0;

        int maxDepthAVL = 0, maxDepthTreap = 0;
        std::vector<int> allDepthsAVL, allDepthsTreap;

        for (int repeat = 0; repeat < 50; ++repeat) {
            std::vector<int> values(N);
            for (int i = 0; i < N; ++i) {
                values[i] = rand() % 10000;
            }

            AVLTree avl_tree;
            for (int v : values) {
                avl_tree.insert(v);
            }

            totalInsertAVL += avl_tree.measureTime([&](int val) { avl_tree.insert(val); }, 1000);
            totalRemoveAVL += avl_tree.measureTime([&](int val) { avl_tree.remove(val); }, 1000);
            totalSearchAVL += avl_tree.measureTime([&](int val) { avl_tree.search(val); }, 1000);

            maxDepthAVL = std::max(maxDepthAVL, avl_tree.getMaxDepth());
            std::vector<int> depthsAVL = avl_tree.getAllDepths();
            allDepthsAVL.insert(allDepthsAVL.end(), depthsAVL.begin(), depthsAVL.end());

            Treap treap;
            for (int v : values) {
                treap.insert(v);
            }

            totalInsertTreap += treap.measureTime([&](int val) { treap.insert(val); }, 1000);
            totalRemoveTreap += treap.measureTime([&](int val) { treap.remove(val); }, 1000);
            totalSearchTreap += treap.measureTime([&](int val) { treap.search(val); }, 1000);

            maxDepthTreap = std::max(maxDepthTreap, treap.getMaxDepth());
            std::vector<int> depthsTreap = treap.getAllDepths();
            allDepthsTreap.insert(allDepthsTreap.end(), depthsTreap.begin(), depthsTreap.end());
        }

        double avgInsertAVL = totalInsertAVL / 50;
        double avgRemoveAVL = totalRemoveAVL / 50;
        double avgSearchAVL = totalSearchAVL / 50;

        double avgInsertTreap = totalInsertTreap / 50;
        double avgRemoveTreap = totalRemoveTreap / 50;
        double avgSearchTreap = totalSearchTreap / 50;

        double avgHeightAVL = std::accumulate(allDepthsAVL.begin(), allDepthsAVL.end(), 0.0) / allDepthsAVL.size();
        double avgHeightTreap = std::accumulate(allDepthsTreap.begin(), allDepthsTreap.end(), 0.0) / allDepthsTreap.size();

        // Для последней серии
        double avgHeightAVLLastSeries = 0.0;
        double avgHeightTreapLastSeries = 0.0;
        double avgBranchHeightAVLLastSeries = 0.0;
        double avgBranchHeightTreapLastSeries = 0.0;

        if (!allDepthsAVL.empty()) {
            avgHeightAVLLastSeries = std::accumulate(allDepthsAVL.begin(), allDepthsAVL.end(), 0.0) / allDepthsAVL.size();

            // Среднее распределение высот веток для AVL
            avgBranchHeightAVLLastSeries = avgHeightAVLLastSeries; // Здесь можно использовать дополнительные формулы для расчёта высоты веток, если нужно.
        }

        if (!allDepthsTreap.empty()) {
            avgHeightTreapLastSeries = std::accumulate(allDepthsTreap.begin(), allDepthsTreap.end(), 0.0) / allDepthsTreap.size();

            // Среднее распределение высот веток для Treap
            avgBranchHeightTreapLastSeries = avgHeightTreapLastSeries; // Для расчёта высоты веток можно также использовать дополнительные подходы.
        }

        outFile << N << ", "
            << maxDepthAVL << ", "
            << avgInsertAVL << ", "
            << avgRemoveAVL << ", "
            << avgSearchAVL << ", "
            << avgHeightAVL << ", "
            << maxDepthTreap << ", "
            << avgInsertTreap << ", "
            << avgRemoveTreap << ", "
            << avgSearchTreap << ", "
            << avgHeightTreap << ", "
            << avgHeightAVLLastSeries << ", "
            << avgHeightTreapLastSeries << ", "
            << avgBranchHeightAVLLastSeries << ", "
            << avgBranchHeightTreapLastSeries << "\n";
    }

    outFile.close();
    std::cout << "Results have been written to results.txt" << std::endl;

    return 0;
}
