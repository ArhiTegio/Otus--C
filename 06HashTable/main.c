#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct row {
    char *key;
    int value;
    int is_deleted;
    int next_addres;
};

struct hash_table {
    int* hash_array;
    struct row* data_array;
    int count_hash_table;
    int count_data_table;
    int position_hash_table;
    int position_data_table;
};

int hash_func(char *value, int hash_table_length) {
    int sum = 0;
    int length = strlen(value);
    for(int i = 0; i < length ; ++i){
        sum += (int)(value[i]);
    }
    return fmod(sum, hash_table_length);
}

struct hash_table create_hash_table(int size)
{
    int *arr_hash = malloc(size * sizeof(int));
    struct row *arr_data =  malloc(size * sizeof(struct row*));
    struct hash_table hash = {};

    hash.data_array = arr_data;
    hash.hash_array = arr_hash;
    hash.count_data_table = size;
    hash.count_hash_table = size;
    hash.position_data_table = 0;
    hash.position_hash_table = 0;

    for(int i = 0; i < size; ++i)
    {
        hash.hash_array[i] = -1;
    }

    return hash;
}

struct row get_last_row(struct hash_table hash, int index)
{
    struct row r = hash.data_array[index];
    int position = 0;
    do
    {
        if(r.next_addres >= 0){
            r = hash.data_array[r.next_addres];
            position = position + 1;
        }
        else{
            break;
        }
    }
    while(1);

    if(hash.position_hash_table < index){
        hash.position_hash_table = index;
    }

    return r;
}

struct hash_table add(struct hash_table hash, char *key, int value)
{
    int position = hash_func(key, hash.count_data_table);
    if(hash.position_data_table < hash.count_data_table)
    {
        struct row r = {NULL, -1, -1, -1};
        r.key = key;
        r.value = value;
        r.is_deleted = 0;
        r.next_addres = -1;

        if(hash.hash_array[position] == -1)
        {
            hash.hash_array[position] = hash.position_data_table;
            hash.data_array[hash.position_data_table] = r;
        }
        else {
            struct row r_old = get_last_row(hash, position);
            r_old.next_addres = hash.position_data_table;
            hash.data_array[hash.position_data_table] = r;
        }
        hash.position_data_table = hash.position_data_table + 1;
    }
    else
    {
        //¬ыполнить расширение масства данных если hash->position_hash_table больше определенного значени€.
        printf("Hash table overflowing.");
    }
    return hash;
}


struct hash_table delete_(struct hash_table hash, char* key){
    int position = hash_func(key, hash.count_data_table);
    struct row r = hash.data_array[hash.hash_array[position]];

    do {
        if(strcmp(r.key, key) == 0){
            r.is_deleted = 1;
            break;
        }
        else {
            if(r.next_addres >= 0) {
                r = hash.data_array[r.next_addres];
            }
            else{
                break;
            }
        }
    }
    while(1);

    return hash;
}

int get_(struct hash_table hash, char *key){
    int position = hash_func(key, hash.count_data_table);
    int index = hash.hash_array[position];
    struct row r = hash.data_array[index];

    do {
        if(strcmp(r.key, key) == 0){
            break;
        }
        else {
            if(r.next_addres >= 0) {
                r = hash.data_array[r.next_addres];
            }
            else{
                break;
            }
        }
    }
    while(1);
    return r.value;
}



int main()
{
    struct hash_table hash = create_hash_table(256);

    hash = add(hash, "test1", 12345);
    hash = add(hash, "test2", 123456);
    hash = add(hash, "test3", 1234567);

    printf("test1 = %d test2 = %d test3 %d\n", get_(hash, "test1"), get_(hash, "test2"), get_(hash, "test3"));

    printf("%d\n", hash.count_data_table);
    printf("Hello world!\n");
    return 0;
}
