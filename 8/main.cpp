#include <iostream>

using namespace std;

int get_length_of_longest_palindrome_subsequence(const string & s);
int get_length_of_longest_common_subsequence(const string & text1, const string & text2);

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
    cout << "1. Найти длину самой длиной подпоследовательности-палиндрома у строки." << endl;
    cout << "2. Выход" << endl;
    size_t choice;
    string s;
    do {
        choice = input_int_in_range(1, 2);
        if (choice == 1) {
            cout << "Введите строку:" << endl;
            cin >> s;
            cout << "Результат: " << get_length_of_longest_palindrome_subsequence(s) << endl;
        }
    } while (choice != 2);

    return 0;
}

int get_length_of_longest_palindrome_subsequence(const string & s) {
    string reversed(s);
    std::reverse(reversed.begin(), reversed.end());
    return get_length_of_longest_common_subsequence(s, reversed);
}

int get_length_of_longest_common_subsequence(const string & text1, const string & text2) {
    int result = 0;
    vector<vector<int>> v;
    for (int i = 0; i < text1.size(); ++i) {
        v.emplace_back();
        for (int j = 0; j < text2.size(); ++j) {
            if (text1[i] != text2[j]) {
                if (i == 0 && j == 0) {
                    v[i].push_back(0);
                } else if (i == 0) {
                    v[i].push_back(v[i][j - 1]);
                } else if (j == 0) {
                    v[i].push_back(v[i - 1][j]);
                } else {
                    v[i].push_back(max(v[i - 1][j], v[i][j - 1]));
                }
            } else {
                if (i == 0 || j == 0) {
                    v[i].push_back(1);
                    if (result == 0) {
                        result = 1;
                    }
                } else {
                    int value = v[i - 1][j - 1] + 1;
                    v[i].push_back(value);
                    result = max(result, value);
                }
            }
        }
    }
    return result;
}
