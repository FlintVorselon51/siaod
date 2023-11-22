#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

struct Node {
    char symbol;
    float probability;
    string code;
};

vector<Node> createNodes(string input) {
    map <char, int> charCount;
    for (char c : input) {
        charCount[c]++;
    }

    vector<Node> nodes;
    for (auto pair : charCount) {
        float probability = static_cast<float>(pair.second) / input.size();
        nodes.push_back({pair.first, probability, ""});
    }

    std::sort(nodes.begin(), nodes.end(), [](Node a, Node b) {
        return a.probability > b.probability;
    });

    return nodes;
}


void shannonFano(vector<Node>& nodes, int start, int end) {
    if (start == end) {
        return;
    }

    float totalProb = 0;
    for (int i = start; i <= end; i++) {
        totalProb += nodes[i].probability;
    }

    float currProb = 0;
    int splitIndex = start;
    float currDiff = abs(2 * currProb - totalProb);
    for (int i = start; i <= end; i++) {
        float diff = abs(2 * (currProb + nodes[i].probability) - totalProb);
        if (diff < currDiff) {
            currDiff = diff;
            splitIndex = i;
        }
        currProb += nodes[i].probability;
    }

    for (int i = start; i <= end; i++) {
        if (i <= splitIndex) {
            nodes[i].code += "0";
        } else {
            nodes[i].code += "1";
        }
    }

    shannonFano(nodes, start, splitIndex);
    shannonFano(nodes, splitIndex + 1, end);
}

string encodeShannonFano(string input, vector<Node> nodes) {
    map<char, string> codeMap;
    for (Node node : nodes) {
        codeMap[node.symbol] = node.code;
    }

    string encodedString;
    for (char c : input) {
        encodedString += codeMap[c];
    }

    return encodedString;
}


string decodeShannonFano(string input, vector<Node> nodes) {
    map<string, char> codeMap;
    for (Node node : nodes) {
        string tmp{node.symbol};
        cout << node.code << " " << tmp << "\n";
        codeMap[node.code] = node.symbol;
    }

    string result = "";
    string currentString = "";
    for (char ch : input) {
        currentString += ch;
        if (codeMap.find(currentString) != codeMap.end()) {
            result += codeMap[currentString];
            currentString = "";
        }
    }
    return result;
}

struct Token77 {
    int offset;
    int length;
    char nextChar;
};

vector<Token77> compress77(string input) {
    vector<Token77> tokens;
    int windowSize = 12;
    int lookAheadBufferSize = 12;

    int currentIndex = 0;
    while (currentIndex < input.length()) {
        int matchLength = 0;
        int matchIndex = 0;
        char nextChar = input[currentIndex + matchLength];

        for (int i = max(0, currentIndex - windowSize); i < currentIndex; ++i) {
            int length = 0;
            while (input[i + length] == input[currentIndex + length] && length < lookAheadBufferSize) {
                length++;
            }
            if (length > matchLength) {
                matchIndex = i;
                matchLength = length;
                nextChar = input[currentIndex + matchLength];
            }
        }

        Token77 token;
        token.offset = currentIndex - matchIndex;
        token.length = matchLength;
        token.nextChar = nextChar;

        tokens.push_back(token);
        currentIndex += (matchLength + 1);
    }
    return tokens;
}


string decompress77(vector<Token77> tokens) {
    string decompressedString;
    for (Token77 token : tokens) {
        if (token.length == 0) {
            decompressedString += token.nextChar;
        } else {
            int startIndex = decompressedString.length() - token.offset;
            for (int i = 0; i < token.length; ++i) {
                decompressedString += decompressedString[startIndex + i];
            }
            decompressedString += token.nextChar;
        }
    }
    return decompressedString;
}

struct Token78 {
    int index;
    string symbol;
    char sym;

    Token78(int index, string symbol) : index(index), symbol(symbol), sym(NULL) {}
    Token78(int index, char symbol) : index(index), symbol(""), sym(symbol) {}
};

bool find(string str, vector<Token78*> nodes, int& n) {
    for (int i = 0; i < nodes.size(); i++)
        if (str == nodes[i]->symbol) {
            n = nodes[i]->index;
            return true;
        }

    return false;
}

vector<Token78*> compress78(string phrase) {

    string str = "";
    vector <Token78*> dictionary;
    vector <Token78*> nodes;
    int n = 0;

    for (int i = 0; i <= phrase.size(); i++) {
        str += phrase[i];
        if (!find(str, dictionary, n)) {
            dictionary.push_back(new Token78(dictionary.size() + 1, str));
            nodes.push_back(new Token78(n, phrase[i]));
            n = 0;
            str = "";
        }
    }

    return nodes;

}

string decompress78(vector<Token78*> encodedData) {
    vector<string> dictionary;
    dictionary.push_back("");
    string decodedString = "";

    for (Token78* token: encodedData) {
        string sequence = dictionary[token->index] + token->sym;
        decodedString += sequence;
        dictionary.push_back(sequence);
    }
    return decodedString;
}

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
    setlocale(LC_ALL, "Russian");
    // Эне-бене, рики-таки, Буль-буль-буль, Караки-шмаки Эус-деус-краснодеус бац
//    0100101010010000101
//  лорлоралоранранлоран
    cout << "1. Запустить кодирование LZ77" << endl;
    cout << "2. Запустить кодирование LZ78" << endl;
    cout << "3. Запустить кодирование фано" << endl;
    cout << "4. Выход" << endl;
    size_t choice;
    string s;
    do {
        choice = input_int_in_range(1, 4);
        if (choice == 1) {
            cout << "Введите строку:" << endl;
            cin >> s;
            vector<Token77> tokens = compress77(s);
            cout << "Результат декодирования: " << decompress77(tokens) << endl;
        } else if (choice == 2) {
            cout << "Введите строку:" << endl;
            cin >> s;
            vector<Token78*> tokens = compress78(s);
            cout << "Результат декодирования: " << decompress78(tokens) << endl;
        } else if (choice == 3) {
            cout << "Введите строку:" << endl;
            cin.ignore();
            getline(cin, s);
            vector<Node> nodes = createNodes(s);
            shannonFano(nodes, 0, nodes.size());
            string encoded = encodeShannonFano(s, nodes);
            cout << "Закодированная строка: " << encoded << endl;
            cout << endl;
            string decoded = decodeShannonFano(encoded, nodes);
            cout << "Декодированная строка: " << decoded << endl;
        }
    } while (choice != 2);

    return 0;
}
