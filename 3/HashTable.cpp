//
// Created by  Dmitry Arapov on 23.09.2023.
//

#include "HashTable.h"


size_t hash_function(const string & s, size_t table_size, const size_t key) {
    size_t hash_result = 0;
    for (int i = 0; s[i] != s.size(); ++i)
        hash_result = (key * hash_result + s[i]) % table_size;
    hash_result = (hash_result * 2 + 1) % table_size;
    return hash_result;
}

size_t h(const string & s, size_t table_size) {
    return hash_function(s, table_size, table_size - 1);
}

size_t g(const string & s, size_t table_size) {
    return hash_function(s, table_size, table_size + 1);
}


