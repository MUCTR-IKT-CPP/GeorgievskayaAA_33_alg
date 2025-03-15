#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <climits>
#include <chrono>
#include <fstream>
#include <queue>

using namespace std;

// Массив с возможными значениями связей для каждой вершины
vector<int> possibleConnections = {3, 4, 10, 20};

// Функция для проверки связности графа с использованием BFS
bool isConnected(const vector<vector<int>>& graph, int N) {
    vector<bool> visited(N, false);
    queue<int> q;
    q.push(0);
    visited[0] = true;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v = 0; v < N; ++v) {
            if (graph[u][v] != 0 && !visited[v]) {
                visited[v] = true;
                q.push(v);
            }
        }
    }

    // Если все вершины были посещены, то граф связан
    for (bool v : visited) {
        if (!v) return false;
    }

    return true;
}

// Функция для добавления ребер, чтобы граф стал связным
void ensureConnectedGraph(vector<vector<int>>& graph, int N) {
    // Если граф не связан, добавляем дополнительные ребра
    if (!isConnected(graph, N)) {
        for (int i = 0; i < N; ++i) {
            for (int j = i + 1; j < N; ++j) {
                if (graph[i][j] == 0) {
                    int weight = rand() % 20 + 1;  // случайный вес ребра от 1 до 20
                    graph[i][j] = weight;
                    graph[j][i] = weight;
                }
            }
        }
    }
}

// Функция для создания графа с N вершинами
vector<vector<int>> generateGraph(int N) {
    vector<vector<int>> graph(N, vector<int>(N, 0));

    // Сначала создаем случайные ребра для связности
    for (int i = 0; i < N; ++i) {
        // Для каждой вершины выбираем случайное количество связей из массива possibleConnections
        int numConnections = possibleConnections[rand() % possibleConnections.size()];

        // Для отслеживания уже связанных вершин
        vector<int> connected(N, 0);

        // Убедимся, что вершина не соединяется с самой собой
        if (numConnections >= N - 1) numConnections = N - 1;  // Не можем соединить с количеством вершин больше, чем есть

        for (int j = 0; j < numConnections; ++j) {
            int randVertex = rand() % N;
            if (randVertex != i && !connected[randVertex]) {
                connected[randVertex] = 1;
                int weight = rand() % 20 + 1;  // случайный вес ребра от 1 до 20
                graph[i][randVertex] = weight;
                graph[randVertex][i] = weight;  // граф неориентированный
            }
        }
    }

    // Теперь проверим, является ли граф связным, и если нет, добавим ребра для связности
    ensureConnectedGraph(graph, N);

    return graph;
}

// Алгоритм Прима для поиска минимального остовного дерева
int primAlgorithm(const vector<vector<int>>& graph) {
    int N = graph.size();
    vector<bool> inMST(N, false);  // Вершины в минимальном остовном дереве
    vector<int> key(N, INT_MAX);    // Ключи для вершин
    key[0] = 0;
    int totalWeight = 0;

    for (int count = 0; count < N; ++count) {
        // Выбираем вершину с минимальным ключом, которая еще не включена в MST
        int u = -1;
        for (int i = 0; i < N; ++i) {
            if (!inMST[i] && (u == -1 || key[i] < key[u])) {
                u = i;
            }
        }

        inMST[u] = true;
        totalWeight += key[u];

        // Обновляем ключи для соседей выбранной вершины
        for (int v = 0; v < N; ++v) {
            if (graph[u][v] != 0 && !inMST[v] && graph[u][v] < key[v]) {
                key[v] = graph[u][v];
            }
        }
    }

    return totalWeight;
}

// Функция для вывода матрицы смежности
void printAdjacencyMatrix(const vector<vector<int>>& graph, int N) {
    cout << "Матрица смежности для графа с " << N << " вершинами:\n";
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cout << graph[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}

int main() {
    srand(time(0));  // Инициализация генератора случайных чисел

    vector<int> vertexCounts = {10, 20, 50, 100};  // Количество вершин
    ofstream outputFile("results.txt");  // Открытие файла для записи

    // Заголовок для файла
    outputFile << "Vertices|TRY|TIME(ns)|Weight\n";

    // Тестируем для разных размеров графа
    for (int N : vertexCounts) {
        // Проводим серию из 5-10 тестов
        int numTests = 10;  // Вы можете увеличить до 10 для более точных результатов
        for (int test = 1; test <= numTests; ++test) {
            // Создаем новый граф для каждого теста
            vector<vector<int>> graph = generateGraph(N);

            // Выводим матрицу смежности в консоль
            printAdjacencyMatrix(graph, N);

            // Запуск алгоритма Прима и замер времени
            auto start = chrono::high_resolution_clock::now();
            int totalWeight = primAlgorithm(graph);
            auto end = chrono::high_resolution_clock::now();

            // Замер времени в наносекундах
            chrono::duration<double> duration = end - start;
            auto nanoseconds = chrono::duration_cast<chrono::nanoseconds>(duration).count();

            // Записываем результаты в файл
            outputFile << N << "|" << test << "|" << nanoseconds << "|" << totalWeight << "\n";
        }
    }

    outputFile.close();  // Закрытие файла

    cout << "Результаты записаны в файл results.txt.\n";

    return 0;
}
