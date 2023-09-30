#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <set>
#include <chrono>

using namespace std;

const int phone_number_size = 11;
const int address_min_size = 20;
const int address_max_size = 30;
const string txt_filename = "file.txt";
const string bin_filename = "file.bin";

random_device dev;
mt19937 rng(dev());


struct phone_owner {
    string phone_number;
    string address;
};

string generate_random_phone_number();
string generate_random_address();
void generate_files(int n);
vector<string> form_queries(int number_of_queries, int number_of_existing_entries);
void test_linear_search();
long long linear_search(ifstream & fin, const string & query);
void test_bin_search();
long long bin_search(const string & query, const vector<pair<string, long long>> & table, const vector<int> & lookup_table);
int input_int_in_range(int a, int b);

int main() {
    cout << "1. Сгенерировать файлы" << endl;
    cout << "2. Запустить тестирование линейного поиска" << endl;
    cout << "3. Запустить тестирование бинарного поиска" << endl;
    cout << "4. Выход" << endl;
    int choice;
    do {
        choice = input_int_in_range(1, 4);
        if (choice == 1) {
            cout << "Введите количество записей." << endl;
            int n = input_int_in_range(1, 10000);
            generate_files(n);
            cout << "Файлы успешно сгенерировались." << endl;
        }
        else if (choice == 2) {
            test_linear_search();
        }
        else if (choice == 3) {
            test_bin_search();
        }
    } while (choice != 4);
    return 0;
}



string generate_random_phone_number() {
    static const char nums[] = "0123456789";
    string phone_number;
    phone_number.reserve();

    uniform_int_distribution<mt19937::result_type> distribution(0, sizeof(nums) - 2);


    for (int i = 0; i < phone_number_size; ++i) {
        phone_number += nums[distribution(rng)];
    }

    return phone_number;
}

string generate_random_address() {
    static const char charset[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";

    uniform_int_distribution<mt19937::result_type> size_distribution(address_min_size, address_max_size);
    unsigned int address_size = size_distribution(rng);

    uniform_int_distribution<mt19937::result_type> distribution(0, sizeof(charset) - 2);

    string address;
    address.reserve(address_size);

    for (int i = 0; i < address_size; ++i) {
        address += charset[distribution(rng)];
    }

    return address;
}

void generate_files(int n) {
    string phone_number;
    ofstream fout(txt_filename);
    ofstream fout_bin(bin_filename, ios::binary);
    fout << n << endl;
    set<string> used_phone_numbers;
    fout_bin.write(reinterpret_cast<const char*>(&n), sizeof(n));
    for (int i = 0; i < n; ++i) {
        do
            phone_number = generate_random_phone_number();
        while (used_phone_numbers.find(phone_number) != used_phone_numbers.end());
        used_phone_numbers.insert(phone_number);
        string address = generate_random_address();
        phone_owner new_phone_owner {phone_number, address};

        long new_phone_number_size = (long)phone_number.size();
        long new_address_size = (long)address.size();

        fout << phone_number << " " << address << endl;
        fout_bin.write(reinterpret_cast<const char*>(&new_phone_number_size), sizeof(new_phone_number_size));
        fout_bin.write(&phone_number[0], new_phone_number_size);
        fout_bin.write(reinterpret_cast<const char*>(&new_address_size), sizeof(new_address_size));
        fout_bin.write(&address[0], new_address_size);
    }
    fout.close();
    fout_bin.close();
}

vector<string> form_queries(int number_of_queries, int number_of_existing_entries) {
    ifstream fin(bin_filename, ios::binary);
    int n;
    long size_of_phone_number;
    long size_of_address;
    string phone_number;
    string address;

    vector<string> result;
    vector<string> phone_numbers;

    fin.read(reinterpret_cast<char*>(&n), sizeof(n));
    while (fin.read(reinterpret_cast<char*>(&size_of_phone_number), sizeof(size_of_phone_number))) {
        phone_number.resize(size_of_phone_number);
        fin.read(&phone_number[0], size_of_phone_number);
        fin.read(reinterpret_cast<char*>(&size_of_address), sizeof(size_of_address));
        address.resize(size_of_address);
        fin.read(&address[0], size_of_address);
        phone_numbers.push_back(phone_number);
    }

    shuffle(phone_numbers.begin(), phone_numbers.end(), rng);

    int i = 0;
    for (; i < number_of_existing_entries and i < phone_numbers.size(); ++i)
        result.push_back(phone_numbers[i]);
    for (; i < number_of_queries; ++i)
        result.push_back(generate_random_phone_number());
    return result;
}

void test_linear_search() {

    ifstream fin(bin_filename, ios::binary);
    int n;
    fin.read(reinterpret_cast<char*>(&n), sizeof(n));
    cout << "В файле обнаружено " << n << " записей." << endl;
    cout << "Введите количество запросов поиска." << endl;
    int number_of_queries = input_int_in_range(1, 100000);
    cout << "Введите количество существующих записей в запросах." << endl;
    int number_of_existing_entries = input_int_in_range(0, n);
    vector<string> queries = form_queries(number_of_queries, number_of_existing_entries);

    auto start_time = chrono::steady_clock::now();
    int counter = 0;
    for (const auto & query : queries) {
        long long pos = linear_search(fin, query);
        if (pos != -1)
            ++counter;
    }
    auto execution_time = chrono::steady_clock::now() - start_time;
    cout << "Найдено записей: " << counter << endl;
    cout << "Время выполнения: " << chrono::duration <double, milli> (execution_time).count() << " ms" << endl;
    string query;
    cin.ignore();
    do {
        cout << "Введите строку для поиска: ";
        getline(cin, query);
        if (query.empty())
            break;
        long long pos = linear_search(fin, query);
        if (pos != -1) {
            fin.seekg(pos);
            long tmp_size;
            string tmp;
            fin.read(reinterpret_cast<char*>(&tmp_size), sizeof(tmp_size));
            tmp.resize(tmp_size);
            fin.read(&tmp[0], tmp_size);
            cout << "Запись найдена (" << tmp;
            fin.read(reinterpret_cast<char*>(&tmp_size), sizeof(tmp_size));
            tmp.resize(tmp_size);
            fin.read(&tmp[0], tmp_size);
            cout << ", " << tmp << ") в позиции " << pos << '.' << endl;
        } else {
            cout << "Запись не найдена." << endl;
        }
    } while (!query.empty());
    fin.close();
}

long long linear_search(ifstream & fin, const string & query) {
    int n;
    long size_of_phone_number;
    long size_of_address;
    string phone_number;
    string address;
    fin.clear();
    fin.seekg(0, ifstream::beg);
    fin.read(reinterpret_cast<char*>(&n), sizeof(n));
    long long pos = fin.tellg();
    while (fin.read(reinterpret_cast<char*>(&size_of_phone_number), sizeof(size_of_phone_number))) {
        phone_number.resize(size_of_phone_number);
        fin.read(&phone_number[0], size_of_phone_number);
        fin.read(reinterpret_cast<char*>(&size_of_address), sizeof(size_of_address));
        address.resize(size_of_address);
        fin.read(&address[0], size_of_address);
        if (query == phone_number)
            return pos;
        pos = fin.tellg();
    }
    return -1;
}

void test_bin_search() {
    ifstream fin(bin_filename, ios::binary);
    int n;
    fin.read(reinterpret_cast<char*>(&n), sizeof(n));
    cout << "В файле обнаружено " << n << " записей." << endl;
    cout << "Введите количество запросов поиска." << endl;
    int number_of_queries = input_int_in_range(1, 100000);
    cout << "Введите количество существующих записей в запросах." << endl;
    int number_of_existing_entries = input_int_in_range(0, n);
    vector<string> queries = form_queries(number_of_queries, number_of_existing_entries);

    vector<pair<string, long long>> table;

    long size_of_phone_number;
    long size_of_address;
    string phone_number;
    string address;
    long long current_pos = fin.tellg();
    while (fin.read(reinterpret_cast<char *>(&size_of_phone_number), sizeof(size_of_phone_number))) {
        phone_number.resize(size_of_phone_number);
        fin.read(&phone_number[0], size_of_phone_number);
        table.emplace_back(phone_number, current_pos);
        fin.read(reinterpret_cast<char *>(&size_of_address), sizeof(size_of_address));
        address.resize(size_of_address);
        fin.read(&address[0], size_of_address);
        current_pos = fin.tellg();
    }
    sort(table.begin(), table.end());

    int tmp = (int) log2(n) + 2;
    vector<int> lookup_table(tmp);

    int pow = 1;
    int i = 0;
    do {
        int half = pow;
        pow <<= 1;
        lookup_table[i] = (n + half) / pow;
    } while (lookup_table[i++] != 0);

    auto start_time = chrono::steady_clock::now();
    int counter = 0;
    for (const auto & query : queries) {
        long long pos = bin_search(query, table, lookup_table);
        if (pos != -1)
            ++counter;
    }
    auto execution_time = chrono::steady_clock::now() - start_time;
    cout << "Найдено записей: " << counter << endl;
    cout << "Время выполнения: " << chrono::duration <double, milli> (execution_time).count() << " ms" << endl;
    string query;
    cin.ignore();
    fin.close();
    fin.open(bin_filename, ios::binary);
    do {
        cout << "Введите строку для поиска: ";
        getline(cin, query);
        if (query.empty())
            break;
        long long pos = bin_search(query, table, lookup_table);
        if (pos != -1) {
            fin.seekg(pos);
            long tmp_size;
            string tmp_s;
            fin.read(reinterpret_cast<char*>(&tmp_size), sizeof(tmp_size));
            tmp_s.resize(tmp_size);
            fin.read(&tmp_s[0], tmp_size);
            cout << "Запись найдена (" << tmp_s;
            fin.read(reinterpret_cast<char*>(&tmp_size), sizeof(tmp_size));
            tmp_s.resize(tmp_size);
            fin.read(&tmp_s[0], tmp_size);
            cout << ", " << tmp_s << ") в позиции " << pos << '.' << endl;
        } else {
            cout << "Запись не найдена." << endl;
        }
    } while (!query.empty());
    fin.close();
}

long long bin_search(const string & query, const vector<pair<string, long long>> & table, const vector<int> & lookup_table) {
    int i = lookup_table[0] - 1;
    int d = 0;
    while (true) {
        if (query == table[i].first)
            return table[i].second;
        if (lookup_table[d] == 0)
            return -1;
        if (query < table[i].first)
            i -= lookup_table[++d];
        else
            i += lookup_table[++d];
    }
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
