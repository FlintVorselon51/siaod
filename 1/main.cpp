#include <iostream>
#include <fstream>
#include <bitset>
#include <random>
#include <algorithm>

using namespace std;

void task_1_a();
void task_1_b();
void task_1_c();
void task_2_a();
void task_2_b();
void task_2_c();
void task_3();
void generate_file_with_numbers(int maximum_number, int amount_of_numbers);
int input_int_in_range(int a, int b);
unsigned input_unsigned_in_range(unsigned a, unsigned b);


int main() {
    cout << "1. Запустить задание 1.а" << endl;
    cout << "2. Запустить задание 1.б" << endl;
    cout << "3. Запустить задание 1.в" << endl;
    cout << "4. Запустить задание 2.а" << endl;
    cout << "5. Запустить задание 2.б" << endl;
    cout << "6. Запустить задание 2.в" << endl;
    cout << "7. Запустить задание 3" << endl;
    cout << "8. Перегенерировать файл со случайными неповторяющимися числами" << endl;
    cout << "9. Выход" << endl;
    int choice;
    do {
        choice = input_int_in_range(1, 9);
        if (choice == 1)
            task_1_a();
        else if (choice == 2)
            task_1_b();
        else if (choice == 3)
            task_1_c();
        else if (choice == 4)
            task_2_a();
        else if (choice == 5)
            task_2_b();
        else if (choice == 6)
            task_2_c();
        else if (choice == 7)
            task_3();
        else if (choice == 8) {
            cout << "Введите максимальное число в файле." << endl;
            int n = input_int_in_range(1, 100000000);
            cout << "Введите количество чисел в файле." << endl;
            int m = input_int_in_range(1, n + 1);
            generate_file_with_numbers(n, m);
        }
    } while (choice != 9);
    return 0;
}

void task_1_a() {
    cout << "Задание 1.а." << endl;
    int tmp;
    cout << "Введите число x." << endl;
    tmp = input_int_in_range(0, 255);
    auto x = (unsigned char) tmp;
    cout << "Начальное число: " << +x << '.' << endl;
    cout << "Двоичная запись: " << bitset<8>(x) << '.' << endl;
    x &= ~(1<<4); // Замена 5-го бита на 0.
    cout << "Число после замены 5-го бита на 0: " << +x << '.' << endl;
    cout << "Двоичная запись: " << bitset<8>(x) << '.' << endl;
    cout << endl;
}

void task_1_b() {
    cout << "Задание 1.б." << endl;
    int tmp;
    cout << "Введите число x." << endl;
    tmp = input_int_in_range(0, 255);
    auto x = (unsigned char) tmp;
    cout << "Начальное число: " << +x << '.' << endl;
    cout << "Двоичная запись: " << bitset<8>(x) << '.' << endl;
    x |= 1<<6; // Замена 7-го бита на 1.
    cout << "Число после замены 7-го бита на 1: " << +x << '.' << endl;
    cout << "Двоичная запись: " << bitset<8>(x) << '.' << endl;
    cout << endl;
}

void task_1_c() {
    cout << "Задание 1.в." << endl;
    unsigned x;
    cout << "Введите число x." << endl;
    x = input_unsigned_in_range(0, unsigned(pow(2, 32)));
    const int n = sizeof(int) * 8;  // Количество разрядов в числе типа int
    unsigned mask = (1 << n - 1);  // 1 в старшем бите 32-разярдной сетки
    cout << "Начальный вид сетки: " << bitset<n>(mask) << endl;
    cout << "Результат: ";
    for (int i = 1; i <= n; ++i) {
        cout << ((x & mask) >> (n - i));
        mask = mask >> 1;  // Смещение 1 в маске на разярд вправо
    }
    cout << endl;
}

void task_2_a() {
    cout << "Задание 2.а" << endl;
    int n;
    cout << "Введите количество чисел: " << endl;
    n = input_int_in_range(1, 8);
    unsigned char x = 0;
    unsigned char tmp;
    for (int i = 0; i < n; ++i) {
        cout << "Введите число." << endl;
        tmp = (unsigned char) input_int_in_range(0, 7);
        x |= (1 << (7 - tmp));
    }
    cout << "Полученный битовый массив: " << bitset<8>(x) << endl;
    cout << "Исходные числа в отсортированном виде: ";
    for (int i = 0; i < 8; ++i) {
        if (x & (1 << (7 - i))) {
            cout << i << " ";
        }
    }
    cout << endl;
}

void task_2_b() {
    cout << "Задание 2.б" << endl;
    int n;
    unsigned long long x = 0;
    cout << "Введите количество чисел." << endl;
    n = input_int_in_range(1, 64);
    int tmp;
    for (int i = 0; i < n; ++i) {
        cout << "Введите число." << endl;
        tmp = input_int_in_range(0, 63);
        x |= (1ull << (63 - tmp));
    }
    cout << "Полученный битовый массив: " << bitset<64>(x) << endl;
    cout << "Исходные числа в отсортированном виде: ";
    for (int i = 0; i < 64; ++i) {
        if (x & (1ull << (63 - i))) {
            cout << i << " ";
        }
    }
    cout << endl;
}

void task_2_c() {
    cout << "Задание 2.б" << endl;
    int n;
    int tmp;
    auto * x = new unsigned char[8];

    cout << "Введите количество чисел.";
    n = input_int_in_range(1, 64);

    for (int i = 0; i < n; ++i) {
        cout << "Введите число.";
        tmp = input_int_in_range(0, 63);
        x[tmp >> 3] |= (1 << (7 - tmp & 7));
    }
    cout << "Полученный битовый массив: ";
    for (int i = 0; i < 8; ++i) {
        cout << bitset<8>(x[i]) << ' ';
    }
    cout << endl;
    cout << "Исходные числа в отсортированном виде: ";
    for (int i = 0; i < 64; ++i) {
        if (x[i >> 3] & (1 << (7 - i & 7)))
            cout << i << " ";
    }
    cout << endl;
}

void generate_file_with_numbers(int maximum_number, int amount_of_numbers) {
    vector<int> v;
    v.reserve(maximum_number + 1);
    for (int i = 0; i <= maximum_number; ++i)
        v.push_back(i);
    random_device rd;
    mt19937 generator(rd());
    shuffle(v.begin(), v.end(), generator);
    ofstream f("numbers.txt");
    f << maximum_number << " ";
    for (int i = 0; i < amount_of_numbers; ++i)
        f << v[i] << " ";
    f.close();
}

void task_3() {
    auto start_time = chrono::steady_clock::now();
    int n;
    ifstream f("numbers.txt");
    f >> n;
    int m = (n >> 3);
    auto * x = new unsigned char[m + 1];
    int tmp;
    while (f >> tmp)
        x[tmp >> 3] |= (1 << (7 - tmp & 7));
    f.close();
    ofstream out("sorted_numbers.txt");
    for (int i = 0; i <= n; ++i) {
        if (x[i >> 3] & (1 << (7 - i & 7))) {
            out << i << " ";
        }
    }
    out.close();
    cout << "Задание 3" << endl;
    auto execution_time = chrono::steady_clock::now() - start_time;
    cout << "Время выполнения: " << chrono::duration <double, milli> (execution_time).count() << " ms" << endl;
    int array_size_in_bytes = sizeof(unsigned char) * (m + 1);
    cout << "Размер битового массива: " << array_size_in_bytes << " [bytes] = " << array_size_in_bytes / 1024 << " [KB]" << endl;
}

int input_int_in_range(int a, int b) {
    int number;
    cout << "Введите целое число в диапозоне [" << a << ", " << b << "]: ";
    cin >> number;
    while (cin.fail() or number < a or number > b) {
        cout << "Ошибка! Пожалуйста, введите целое число в диапозоне [" << a << ", " << b << "]: ";
        cin.clear();
        cin.ignore(256,'\n');
        cin >> number;
    }
    return number;
}

unsigned input_unsigned_in_range(unsigned a, unsigned b) {
    unsigned number;
    cout << "Введите целое число в диапозоне [" << a << ", " << b << "]: ";
    cin >> number;
    while (cin.fail() or number < a or number > b) {
        cout << "Ошибка! Пожалуйста, введите целое число в диапозоне [" << a << ", " << b << "]: ";
        cin.clear();
        cin.ignore(256,'\n');
        cin >> number;
    }
    return number;
}

