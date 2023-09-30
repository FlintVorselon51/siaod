#include <iostream>
#include <random>
#include <regex>

using namespace std;

random_device dev;
mt19937 rng(dev());

string longest_word(const string & sentence);
int kmp(const string & p, const string & t);
vector<size_t> generate_z(const string & s);
vector<size_t> generate_prefix(const string & s);
string generate_random_sentence(size_t number_of_words, size_t min_size, size_t max_size);
string generate_random_string(size_t size);
size_t input_size_t_in_range(size_t a, size_t b);
string trim(const string & str);

int main() {
    string x = "abacaba";

    cout << "1. [случайное предложение] Найти самое длинное слово, первая и последняя буква которого одинаковы." << endl;
    cout << "2. [ввод] Найти самое длинное слово, первая и последняя буква которого одинаковы." << endl;
    cout << "3. [случайные строки] алгоритм Кнута-Мориса-Пратта" << endl;
    cout << "4. [ввод] алгоритм Кнута-Мориса-Пратта" << endl;
    cout << "5. Выход" << endl;
    size_t choice;
    do {
        choice = input_size_t_in_range(1, 5);
        if (choice == 1) {
            size_t number_of_words;
            size_t min_size;
            size_t max_size;
            cout << "Введите количество слов в строке." << endl;
            number_of_words = input_size_t_in_range(1, 10000);
            cout << "Введите минимальный размер слова." << endl;
            min_size = input_size_t_in_range(1, 10000);
            cout << "Введите максимальный размер слова." << endl;
            max_size = input_size_t_in_range(min_size, 10000);
            string s = generate_random_sentence(number_of_words, min_size, max_size);
            cout << "Предложение:" << endl << s << endl;
            string result = longest_word(s);
            if (!result.empty()) {
                cout << "Самое длинное слово, которое начинается и заканчивается на один и тот же символ:" << endl;
                cout << result << endl;
            } else
                cout << "Слово, которое начинается и заканчивается на один и тот же символ, не найдено" << endl;
        }
        else if (choice == 2) {
            string s;
            cout << "Введите предложение:" << endl;
            cin.clear();
            cin.ignore();
            getline(cin, s);
            s = trim(s);
            cout << "Предложение:" << endl << s << endl;
            string result = longest_word(s);
            if (!result.empty()) {
                cout << "Самое длинное слово, которое начинается и заканчивается на один и тот же символ:" << endl;
                cout << result << endl;
            } else
                cout << "Слово, которое начинается и заканчивается на один и тот же символ, не найдено" << endl;
        } else if (choice == 3) {
            cout << "Введите длину строки" << endl;
            size_t t_size = input_size_t_in_range(1, 100000);
            string t = generate_random_string(t_size);
            cout << "Введите длину шаблона" << endl;
            size_t p_size = input_size_t_in_range(1, 100);
            string p = generate_random_string(p_size);
            int result = kmp(p, t);
            cout << "Строка:" << endl << t << endl;
            cout << "Шаблон:" << endl << p << endl;
            if (result == -1)
                cout << "Совпадение с шаблоном не найдено." << endl;
            else
                cout << "Последний раз шаблон встречается на индексе " << result << endl;
        } else if (choice == 4) {
            string t;
            cout << "Введите строку:" << endl;
            cin.clear();
            cin.ignore();
            getline(cin, t);
            t = trim(t);
            string p;
            cout << "Введите шаблон:" << endl;
            getline(cin, p);
            p = trim(p);
            cout << "Строка:" << endl << t << endl;
            cout << "Шаблон:" << endl << p << endl;
            int result = kmp(p, t);
            if (result == -1)
                cout << "Совпадение с шаблоном не найдено." << endl;
            else
                cout << "Последний раз шаблон встречается на индексе " << result << endl;
        }
    } while (choice != 5);
    return 0;
}

string longest_word(const string & sentence) {
    string result;
    size_t word_start = 0;
    for (size_t i = 1; i < sentence.size(); ++i) {
        if (sentence[i + 1] == ' ' or i + 1 >= sentence.size()) {
            if (sentence[i] == sentence[word_start] and i - word_start + 1 > result.size())
                result = sentence.substr(word_start, i - word_start + 1);
            word_start = i + 2;
        }
    }
    return result;
}

int kmp(const string & p, const string & t) {
    int result = -1;
    vector<size_t> prefix = generate_prefix(p + "#" + t);
    for (size_t i = 0; i < t.length(); ++i)
        if (prefix[p.length() + i + 1] == p.length())
            result = int(i - p.length() + 1);
    return result;
}

vector<size_t> generate_z(const string & s) {
    const size_t n = s.length();
    size_t l = 0;
    size_t r = 0;
    vector<size_t> z;
    z.resize(n);
    z[0] = 0;
    for (size_t i = 1; i < n; ++i) {
        z[i] = 0;
        if (r >= i)
            z[i] = std::min(z[i - l], r - i + 1);
        while (z[i] + i < n && s[z[i]] == s[z[i] + i])
            ++z[i];
        if (i + z[i] - 1 > r) {
            l = i;
            r = i + z[i] - 1;
        }
    }
    return z;
}

vector<size_t> generate_prefix(const string & s) {
    const size_t n = s.length();
    vector<size_t> p;
    p.resize(n);
    for (size_t i = 1; i < n; ++i) {
        size_t k = p[i - 1];
        while (k > 0 and s[i] != s[k])
            k = p[k - 1];
        if (s[i] == s[k])
            ++k;
        p[i] = k;
    }
    return p;
}

string generate_random_sentence(size_t number_of_words, size_t min_size, size_t max_size) {
    static const char charset[] = "abcdefghijklmnopqrstuvwxyz";

    uniform_int_distribution<mt19937::result_type> size_distribution(min_size, max_size);
    uniform_int_distribution<mt19937::result_type> distribution(0, sizeof(charset) - 2);

    string result;

    for (size_t i = 0; i < number_of_words; ++i) {
        size_t size = size_distribution(rng);
        for (size_t j = 0; j < size; ++j)
            result += charset[distribution(rng)];
        if (i != number_of_words - 1)
            result += ' ';
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

size_t input_size_t_in_range(size_t a, size_t b) {
    size_t number;
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

string trim(const string & str) {
    static regex regular("^\\s+(.*?)\\s+$");
    static smatch result;

    if (regex_match(str, result, regular))
        return result[1];
    return str;
}

