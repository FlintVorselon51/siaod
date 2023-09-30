#include <iostream>
#include <random>
#include "HashTable.h"

using namespace std;

random_device dev;
mt19937 rng(dev());

string generate_random_numeric_string(size_t size);
string generate_random_string(size_t size);
int input_int_in_range(int a, int b);

int main() {
    HashTable hash_table;
    cout << "1. Добавить"
            " случайных студентов в хеш таблицу" << endl;
    cout << "2. Найти студента по таблице" << endl;
    cout << "3. Удалить студента из таблицы" << endl;
    cout << "4. Выход" << endl;
    int choice;
    do {
        choice = input_int_in_range(1, 4);
        if (choice == 1) {
            cout << "Введите количество студентов." << endl;
            int n = input_int_in_range(1, 20);
            for (int i = 0; i < n; ++i) {
                student s = {generate_random_numeric_string(10), generate_random_numeric_string(4),
                             generate_random_string(20)};
                hash_table.add(s);
                cout << "Добавлен студент: " << s.record_book_number << ' ' << s.group << ' ' << s.name << endl;
            }
        }
        else if (choice == 2) {
            string key;
            cout << "Введите номер зачетной книжки студента: ";
            cin >> key;
            student s = hash_table.find(key);
             if (!s.name.empty() and !s.group.empty() and !s.record_book_number.empty())
                 cout << "Найден студент: " << s.record_book_number << ' ' << s.group << ' ' << s.name << endl;
             else
                 cout << "Студент с таким студенческим номером не найден" << endl;
        }
        else if (choice == 3) {
            string key;
            cout << "Введите номер зачетной книжки студента: ";
            cin >> key;
            if (hash_table.remove(key)) {
                cout << "Студент успешно удален" << endl;
            } else {
                cout << "Студент не найден" << endl;
            }
        }
    } while (choice != 4);
    return 0;
}

string generate_random_numeric_string(size_t size) {
    static const char nums[] = "0123456789";
    string result;
    result.reserve(size);

    uniform_int_distribution<mt19937::result_type> distribution(0, sizeof(nums) - 2);

    for (int i = 0; i < size; ++i) {
        result += nums[distribution(rng)];
    }

    return result;
}

string generate_random_string(size_t size) {
    static const char charset[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";

    uniform_int_distribution<mt19937::result_type> distribution(0, sizeof(charset) - 2);

    string result;
    result.reserve(size);

    for (int i = 0; i < size; ++i) {
        result += charset[distribution(rng)];
    }

    return result;
}

int input_int_in_range(int a, int b) {
    int number;
    cout << "Введите целое число в диапозоне [" << a << ", " << b << "]: ";
    cin >> number;
    while (cin.fail() or number < a or number > b) {
        cout << "Ошибка! Пожалуйста, введите целое число в диапозоне [" << a << ", " << b << "]: ";
        cin.clear();
        cin.ignore();
        cin >> number;
    }
    return number;
}

