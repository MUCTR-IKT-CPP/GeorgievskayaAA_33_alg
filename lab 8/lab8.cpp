#include <iostream>
#include <vector>
#include <cmath>
#include <functional>
#include <cstdlib>  // Для rand()
#include <ctime>    // Для time
#include <chrono>   // Для std::chrono
#include <fstream>  // Для std::ofstream

class BinaryHeap {
private:
    std::vector<int> heap;

    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (heap[parent] <= heap[index]) {
                break;
            }
            std::swap(heap[parent], heap[index]);
            index = parent;
        }
    }

    void heapifyDown(int index) {
        int size = heap.size();
        while (2 * index + 1 < size) {
            int leftChild = 2 * index + 1;
            int rightChild = 2 * index + 2;
            int smallest = index;

            if (leftChild < size && heap[leftChild] < heap[smallest]) {
                smallest = leftChild;
            }
            if (rightChild < size && heap[rightChild] < heap[smallest]) {
                smallest = rightChild;
            }

            if (smallest == index) {
                break;
            }

            std::swap(heap[index], heap[smallest]);
            index = smallest;
        }
    }

public:
    void insert(int value) {
        heap.push_back(value);
        heapifyUp(heap.size() - 1);
    }

    int extractMin() {
        if (heap.empty()) {
            throw std::out_of_range("Heap is empty");
        }

        int minValue = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        heapifyDown(0);

        return minValue;
    }

    int getMin() {
        if (heap.empty()) {
            throw std::out_of_range("Heap is empty");
        }

        return heap[0];
    }

    bool isEmpty() {
        return heap.empty();
    }
};

class BinomialHeap {
private:
    struct Node {
        int value;
        int degree;
        Node* parent;
        Node* child;
        Node* sibling;

        Node(int value) : value(value), degree(0), parent(nullptr), child(nullptr), sibling(nullptr) {}
    };

    Node* mergeTrees(Node* tree1, Node* tree2) {
        if (tree1->value > tree2->value) {
            std::swap(tree1, tree2);
        }
        tree2->parent = tree1;
        tree2->sibling = tree1->child;
        tree1->child = tree2;
        tree1->degree++;
        return tree1;
    }

    Node* unionHeaps(Node* h1, Node* h2) {
        if (!h1) return h2;
        if (!h2) return h1;

        Node* newHeap = nullptr;
        Node** tail = &newHeap;

        while (h1 && h2) {
            if (h1->degree < h2->degree) {
                *tail = h1;
                h1 = h1->sibling;
            }
            else if (h1->degree > h2->degree) {
                *tail = h2;
                h2 = h2->sibling;
            }
            else {
                Node* merged = mergeTrees(h1, h2);
                *tail = merged;
                h1 = h1->sibling;
                h2 = h2->sibling;
            }
            tail = &(*tail)->sibling;
        }

        if (h1) *tail = h1;
        if (h2) *tail = h2;

        return newHeap;
    }

    Node* extractMinNode(Node* head, Node*& minNode) {
        if (!head) return nullptr;
        if (!minNode || head->value < minNode->value) {
            minNode = head;
        }
        return extractMinNode(head->sibling, minNode);
    }

public:
    Node* head;

    BinomialHeap() : head(nullptr) {}

    void insert(int value) {
        Node* newNode = new Node(value);
        head = unionHeaps(head, newNode);
    }

    int extractMin() {
        if (!head) throw std::out_of_range("Heap is empty");

        Node* minNode = nullptr;
        head = extractMinNode(head, minNode);

        Node* newHead = nullptr;
        if (minNode->child) {
            Node* child = minNode->child;
            while (child) {
                Node* nextChild = child->sibling;
                child->sibling = newHead;
                child->parent = nullptr;
                newHead = child;
                child = nextChild;
            }
        }

        head = unionHeaps(head, newHead);
        return minNode->value;
    }

    int getMin() {
        if (!head) throw std::out_of_range("Heap is empty");

        Node* minNode = nullptr;
        extractMinNode(head, minNode);

        return minNode->value;
    }

    bool isEmpty() {
        return head == nullptr;
    }
};

long long measureAvgTime(std::function<void()> operation, int count) {
    long long totalTime = 0;
    for (int i = 0; i < count; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        operation();  // Выполнение операции
        auto end = std::chrono::high_resolution_clock::now();
        long long duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        totalTime += duration;
    }
    return totalTime / count;
}

long long measureMaxTime(std::function<void()> operation, int count) {
    long long maxTime = 0;
    for (int i = 0; i < count; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        operation();  // Выполнение операции
        auto end = std::chrono::high_resolution_clock::now();
        long long duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        maxTime = std::max(maxTime, duration);
    }
    return maxTime;
}

void measurePerformance(BinaryHeap& binaryHeap, BinomialHeap& binomialHeap, int N, std::ofstream& outFile) {
    // Случайные числа для тестов
    std::vector<int> randomNumbers(N);
    for (int i = 0; i < N; ++i) {
        randomNumbers[i] = rand() % 1000;
    }

    // Тест 1: 1000 раз найти минимум
    outFile << "Измерение для поиска минимума (1000 операций):\n";
    outFile << "Среднее время для поиска минимума для бинарной кучи: " 
            << measureAvgTime([&]() {
                if (!binaryHeap.isEmpty()) {
                    binaryHeap.getMin();
                }
            }, 1000) << " наносекунд\n";
    outFile << "Среднее время для поиска минимума для биномиальной кучи: " 
            << measureAvgTime([&]() {
                if (!binomialHeap.isEmpty()) {
                    binomialHeap.getMin();
                }
            }, 1000) << " наносекунд\n";

    // Тест 2: 1000 раз удалить минимум
    outFile << "Измерение для удаления минимума (1000 операций):\n";
    outFile << "Среднее время для удаления минимума для бинарной кучи: " 
            << measureAvgTime([&]() {
                if (!binaryHeap.isEmpty()) {
                    binaryHeap.extractMin();
                }
            }, 1000) << " наносекунд\n";
    outFile << "Среднее время для удаления минимума для биномиальной кучи: " 
            << measureAvgTime([&]() {
                if (!binomialHeap.isEmpty()) {
                    binomialHeap.extractMin();
                }
            }, 1000) << " наносекунд\n";

    // Тест 3: 1000 раз добавить новый элемент
    outFile << "Измерение для добавления элемента (1000 операций):\n";
    outFile << "Среднее время для добавления элемента в бинарную кучу: " 
            << measureAvgTime([&]() {
                binaryHeap.insert(randomNumbers[rand() % N]);
            }, 1000) << " наносекунд\n";
    outFile << "Среднее время для добавления элемента в биномиальную кучу: " 
            << measureAvgTime([&]() {
                binomialHeap.insert(randomNumbers[rand() % N]);
            }, 1000) << " наносекунд\n";

    // Максимальное время для 10, 25, 50 и 100 операций
    std::vector<int> operations = {10, 25, 50, 100};

    // Максимальное время для поиска минимума
    for (int opCount : operations) {
        outFile << "Максимальное время для " << opCount << " операций поиска минимума для бинарной кучи: " 
                << measureMaxTime([&]() {
                    if (!binaryHeap.isEmpty()) {
                        binaryHeap.getMin();
                    }
                }, opCount) << " наносекунд\n";
        outFile << "Максимальное время для " << opCount << " операций поиска минимума для биномиальной кучи: " 
                << measureMaxTime([&]() {
                    if (!binomialHeap.isEmpty()) {
                        binomialHeap.getMin();
                    }
                }, opCount) << " наносекунд\n";
    }

    // Максимальное время для удаления минимума
    for (int opCount : operations) {
        outFile << "Максимальное время для " << opCount << " операций удаления минимума для бинарной кучи: " 
                << measureMaxTime([&]() {
                    if (!binaryHeap.isEmpty()) {
                        binaryHeap.extractMin();
                    }
                }, opCount) << " наносекунд\n";
        outFile << "Максимальное время для " << opCount << " операций удаления минимума для биномиальной кучи: " 
                << measureMaxTime([&]() {
                    if (!binomialHeap.isEmpty()) {
                        binomialHeap.extractMin();
                    }
                }, opCount) << " наносекунд\n";
    }

    // Максимальное время для добавления элемента
    for (int opCount : operations) {
        outFile << "Максимальное время для " << opCount << " операций добавления элемента в бинарную кучу: " 
                << measureMaxTime([&]() {
                    binaryHeap.insert(randomNumbers[rand() % N]);
                }, opCount) << " наносекунд\n";
        outFile << "Максимальное время для " << opCount << " операций добавления элемента в биномиальную кучу: " 
                << measureMaxTime([&]() {
                    binomialHeap.insert(randomNumbers[rand() % N]);
                }, opCount) << " наносекунд\n";
    }
}

int main() {
    srand(time(0));

    // Открываем файл для записи результатов
    std::ofstream outFile("heap_performance_results.txt");

    if (!outFile) {
        std::cerr << "Не удалось открыть файл для записи!" << std::endl;
        return 1;
    }

    for (int i = 3; i <= 7; ++i) {
        int N = pow(10, i);  // N = 10^i
        BinaryHeap binaryHeap;
        BinomialHeap binomialHeap;

        // Заполнение кучами
        for (int j = 0; j < N; ++j) {
            int randomValue = rand() % 1000;  // Случайное значение от 0 до 999
            binaryHeap.insert(randomValue);
            binomialHeap.insert(randomValue);
        }

        // Измерение производительности и запись в файл
        outFile << "Тест для N = " << N << ":\n";
        measurePerformance(binaryHeap, binomialHeap, N, outFile);
        outFile << std::endl;
    }

    outFile.close();  // Закрываем файл

    std::cout << "Результаты записаны в файл heap_performance_results.txt\n";

    return 0;
}
