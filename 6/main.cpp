#include <iostream>
#include <map>
#include <set>

using namespace std;

// key - source point.
// value - vector of pairs.
// pair.first - target point.
// pair.second - path cost.
map<int, vector<pair<int, int>>> graph;
int source = 1;
int target = 6;
set<int> visited_set;
vector<int> visited_vector;


pair<int, vector<int>> dfs(int node, int current_path_cost);

int input_int_in_range(int a, int b) {
    int number;
    cout << "Введите натуральное число в диапозоне [" << a << ", " << b << "]: ";
    cin >> number;
    while (cin.fail() or number < a or number > b) {
        cout << "Ошибка! Пожалуйста, введите целое число в диапозоне [" << a << ", " << b << "]: ";
        cin.clear();
        cin.ignore();
        cin >> number;
    }
    return number;
}


int main() {
    graph[1] = {{3, 4}, {2, 8}};
    graph[2] = {{4, 6}, {5, 3}};
    graph[3] = {{2, 3}, {4, 2}, {6, 10}};
    graph[4] = {{5, 3}, {6, 1}};
    graph[5] = {{6, 4}};
    cout << "1. Задать граф вручную." << endl;
    cout << "2. Задать стартовую точку." << endl;
    cout << "3. Задать конечную точку." << endl;
    cout << "4. Посчитать кратчайший путь" << endl;
    cout << "5. Выход" << endl;
    size_t choice;
    do {
        choice = input_int_in_range(1, 5);
        if (choice == 1) {
            graph.clear();
            cout << "Введите количество рёбер" << endl;
            int number_of_edges = input_int_in_range(1, 1000);
            for (int i = 0; i < number_of_edges; ++i) {
                cout << "Введите стартовую вершину ребра" << endl;
                int start_vertex = input_int_in_range(1, 1000);
                cout << "Введите целевую вершину ребра" << endl;
                int end_vertex = input_int_in_range(1, 1000);
                cout << "Введите длину ребра" << endl;
                int cost = input_int_in_range(1, 10000000);
                if (graph.find(start_vertex) != graph.end()) {
                    graph[start_vertex].emplace_back(end_vertex, cost);
                } else {
                    graph[start_vertex] = {{end_vertex, cost}};
                }
            }
        } else if (choice == 2) {
            source = input_int_in_range(1, 1000);
        } else if (choice == 3) {
            target = input_int_in_range(1, 1000);
        } else if (choice == 4) {
            if (source == target) {
                cout << "Стартовая точка равна конечной" << endl;
                continue;
            }
            auto result = dfs(source, 0);
            if (result.first == -1) {
                cout << "Путь из " << source << " в " << target << " не найден" << endl;
                continue;
            }
            cout << "Минимальная длина пути: " << result.first << endl;
            cout << "Путь: " << source << " -> ";
            for (int i = 0; i < result.second.size() - 1; ++i) {
                cout << result.second[i] << " -> ";
            }
            cout << result.second.back() << endl;
        }
    } while (choice != 5);

    return 0;
}

pair<int, vector<int>> dfs(int node, int current_path_cost) {
    if (node == target) {
        return {current_path_cost, visited_vector};
    }
    int result_cost = -1;
    vector<int> result_path;
    for (auto neighbor : graph[node]) {
        if (visited_set.find(neighbor.first) != visited_set.end())
            continue;
        visited_set.insert(neighbor.first);
        visited_vector.push_back(neighbor.first);
        auto x = dfs(neighbor.first, current_path_cost + neighbor.second);
        visited_set.erase(neighbor.first);
        visited_vector.pop_back();
        if (x.first != -1 && (result_cost == -1 || x.first < result_cost)) {
            result_cost = x.first;
            result_path = x.second;
        }
    }
    return {result_cost, result_path};
}
