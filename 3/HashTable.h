#ifndef SIAOD33_HASHTABLE_H
#define SIAOD33_HASHTABLE_H
#include <iostream>

using namespace std;

struct student {
    string record_book_number;
    string group;
    string name;
};

struct node {
    student s;
    bool is_deleted;
};

size_t hash_function(const string & s, size_t table_size, const size_t key);

size_t h(const string & s, size_t table_size);

size_t g(const string & s, size_t table_size);


class HashTable {
private:
    const double ratio = 0.75;
    size_t buffer_size;
    size_t size;
    size_t size_all_non_nullptr;
    node ** arr;
public:
    HashTable() {
        buffer_size = 8;
        size = 0;
        size_all_non_nullptr = 0;
        arr = new node * [buffer_size];
        for (int i = 0; i < buffer_size; ++i)
            arr[i] = nullptr;
    }
    ~HashTable() {
        for (int i= 0; i < buffer_size; ++i)
            if (arr[i])
                delete arr[i];
        delete[] arr;
    }
    void resize() {
        size_t past_buffer_size = buffer_size;
        buffer_size *= 2;
        size_all_non_nullptr = 0;
        size = 0;
        node ** arr_2 = new node * [buffer_size];
        for (int i = 0; i < buffer_size; ++i)
            arr_2[i] = nullptr;
        swap(arr, arr_2);
        for (int i = 0; i < past_buffer_size; ++i) {
            if (arr_2[i] && !arr_2[i]->is_deleted) {
                student tmp;
                tmp.name = arr_2[i]->s.name;
                tmp.group = arr_2[i]->s.group;
                tmp.record_book_number = arr_2[i]->s.record_book_number;
                add(tmp);
            }
        }
        for (int i = 0; i < past_buffer_size; ++i) {
            if (arr_2[i])
                delete arr_2[i];
        }
        delete[] arr_2;
    }
    student find(const string & key) {
        size_t h1 = h(key, buffer_size);
        size_t h2 = g(key, buffer_size);
        size_t i = 0;
        while (arr[h1] != nullptr && i < buffer_size) {
            if (arr[h1]->s.record_book_number == key && !arr[h1]->is_deleted)
                return arr[h1]->s;
            h1 = (h1 + h2) % buffer_size;
            ++i;
        }
        return {};
    }
    bool remove(const string & key) {
        size_t h1 = h(key, buffer_size);
        size_t h2 = g(key, buffer_size);
        size_t i = 0;
        while (arr[h1] != nullptr && i < buffer_size) {
            if (arr[h1]->s.record_book_number == key && !arr[h1]->is_deleted) {
                arr[h1]->is_deleted = true;
                --size;
                return true;
            }
            h1 = (h1 + h2) % buffer_size;
            ++i;
        }
        return false;
    }
    bool add(const student & s) {
        if (size + 1 > int(ratio * double(buffer_size)) or size_all_non_nullptr > 2 * size)
            resize();
        size_t h1 = h(s.record_book_number, buffer_size);
        size_t h2 = g(s.record_book_number, buffer_size);
        size_t i = 0;
        int first_deleted = -1;
        while (arr[h1] != nullptr && i < buffer_size) {
            if (arr[h1]->s.record_book_number == s.record_book_number && !arr[h1]->is_deleted)
                return false;
            if (arr[h1]->is_deleted && first_deleted == -1)
                first_deleted = int(h1);
            h1 = (h1 + h2) % buffer_size;
            ++i;
        }
        if (first_deleted == -1) {
            arr[h1] = new node();
            arr[h1]->s.name = s.name;
            arr[h1]->s.group = s.group;
            arr[h1]->s.record_book_number = s.record_book_number;
            arr[h1]->is_deleted = false;
        } else {
            arr[first_deleted]->s.name = s.name;
            arr[first_deleted]->s.group = s.group;
            arr[first_deleted]->s.record_book_number = s.record_book_number;
            arr[h1]->is_deleted = false;
        }
        ++size;
        return true;
    }
};



#endif //SIAOD33_HASHTABLE_H
