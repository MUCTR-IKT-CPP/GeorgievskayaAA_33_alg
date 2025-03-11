#include <iostream>
#include <vector>
#include <queue>
#include <cstdlib>
#include <algorithm>
#include <chrono>

// я устав...
using namespace std;
using namespace chrono;

class RandomGraph {
private:
    int numVertices; // Количество вершин
    int numEdges;    // Количество рёбер
    bool directed;   // Направленность графа
    vector<vector<int>> adjMatrix;    // Матрица смежности
    vector<vector<int>> adjList;      // Список смежности
    vector<pair<int, int>> edges;     // Список рёбер

public:
    // Конструктор с параметрами генерации графа
    RandomGraph(int minVertices, int maxVertices, int minEdges, int maxEdges,
                int maxEdgesPerVertex, bool isDirected) {
        srand(time(0));

        numVertices = rand() % (maxVertices - minVertices + 1) + minVertices;
        numEdges = rand() % (maxEdges - minEdges + 1) + minEdges;
        directed = isDirected;

        // Инициализация структуры данных
        adjMatrix.resize(numVertices, vector<int>(numVertices, 0));
        adjList.resize(numVertices);

        generateGraph(maxEdgesPerVertex);
    }

    // Генерация графа
    void generateGraph(int maxEdgesPerVertex) {
        int edgeCount = 0;

        while (edgeCount < numEdges) {
            int u = rand() % numVertices;
            int v = rand() % numVertices;
            
            // Проверка на существование ребра (чтобы не дублировать рёбра)
            if (u != v && adjMatrix[u][v] == 0) {
                adjMatrix[u][v] = 1;
                adjList[u].push_back(v);
                edges.push_back({u, v});

                // Если граф направленный, то добавляем ребро только в одну сторону
                if (!directed) {
                    adjMatrix[v][u] = 1;
                    adjList[v].push_back(u);
                    edges.push_back({v, u});
                }

                edgeCount++;
            }
        }
    }

    // Метод для поиска кратчайшего пути с помощью поиска в ширину (BFS)
    bool bfs(int start, int end) {
        vector<bool> visited(numVertices, false);
        vector<int> parent(numVertices, -1);
        queue<int> q;

        visited[start] = true;
        q.push(start);

        while (!q.empty()) {
            int vertex = q.front();
            q.pop();

            for (int neighbor : adjList[vertex]) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    parent[neighbor] = vertex;
                    q.push(neighbor);

                    if (neighbor == end) {
                        // Восстановление пути
                        vector<int> path;
                        for (int v = end; v != -1; v = parent[v]) {
                            path.push_back(v);
                        }
                        reverse(path.begin(), path.end());
                        cout << "Path (BFS): ";
                        for (int v : path) {
                            cout << v << " ";
                        }
                        cout << endl;
                        return true;
                    }
                }
            }
        }

        return false;
    }

    // Метод для поиска кратчайшего пути с помощью поиска в глубину (DFS)
    bool dfs(int start, int end, vector<bool>& visited, vector<int>& path) {
        visited[start] = true;
        path.push_back(start);

        if (start == end) {
            cout << "Path (DFS): ";
            for (int v : path) {
                cout << v << " ";
            }
            cout << endl;
            return true;
        }

        for (int neighbor : adjList[start]) {
            if (!visited[neighbor]) {
                if (dfs(neighbor, end, visited, path)) {
                    return true;
                }
            }
        }

        path.pop_back();
        return false;
    }

    // Метод для вызова DFS
    bool dfsWrapper(int start, int end) {
        vector<bool> visited(numVertices, false);
        vector<int> path;
        return dfs(start, end, visited, path);
    }

    // Методы для получения представлений графа
    vector<vector<int>> getAdjMatrix() {
        return adjMatrix;
    }

    vector<vector<int>> getAdjList() {
        return adjList;
    }

    vector<pair<int, int>> getEdges() {
        return edges;
    }

    // Метод для отображения графа
    void displayGraph() {
        cout << "Adjacency Matrix: \n";
        for (auto& row : adjMatrix) {
            for (int val : row) {
                cout << val << " ";
            }
            cout << endl;
        }

        cout << "Adjacency List: \n";
        for (int i = 0; i < adjList.size(); ++i) {
            cout << i << ": ";
            for (int v : adjList[i]) {
                cout << v << " ";
            }
            cout << endl;
        }

        cout << "Edges: \n";
        for (auto& edge : edges) {
            cout << edge.first << " -> " << edge.second << endl;
        }
    }
};

// Функция для проведения тестирования
void testGraphs(int minVertices, int maxVertices, int minEdges, int maxEdges,
                int maxEdgesPerVertex, bool directed) {
    for (int i = 0; i < 10; ++i) {
        // Количество вершин и рёбер для текущего графа
        int vertices = minVertices + i;
        int edges = minEdges + i;

        // Генерация графа
        RandomGraph graph(vertices, vertices + 1, edges, edges + 1, maxEdgesPerVertex, directed);
        cout << "Generated graph with " << vertices << " vertices and " << edges << " edges.\n";
        graph.displayGraph();

        // Выбор случайных вершин для поиска пути
        int start = rand() % vertices;
        int end = rand() % vertices;
        cout << "Searching for path from " << start << " to " << end << "...\n";

        // Замер времени для поиска с использованием BFS
        auto start_time = high_resolution_clock::now();
        if (!graph.bfs(start, end)) {
            cout << "No path found using BFS." << endl;
        }
        auto end_time = high_resolution_clock::now();
        auto duration = duration_cast<nanoseconds>(end_time - start_time);
        cout << "BFS Time: " << duration.count() << " ns\n";

        // Замер времени для поиска с использованием DFS
        start_time = high_resolution_clock::now();
        if (!graph.dfsWrapper(start, end)) {
            cout << "No path found using DFS." << endl;
        }
        end_time = high_resolution_clock::now();
        duration = duration_cast<nanoseconds>(end_time - start_time);
        cout << "DFS Time: " << duration.count() << " ns\n";
    }
}

int main() {
    // Параметры генерации графов
    int minVertices = 5;
    int maxVertices = 15;
    int minEdges = 5;
    int maxEdges = 20;
    int maxEdgesPerVertex = 3;
    bool directed = false;

    testGraphs(minVertices, maxVertices, minEdges, maxEdges, maxEdgesPerVertex, directed);

    return 0;
}
