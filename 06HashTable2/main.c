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

struct row get_last_row(struct hash_table *hash, int index)
{
    struct row r = hash->data_array[index];
    int position = 0;
    do
    {
        if(r.next_addres >= 0){
            r = hash->data_array[r.next_addres];
            position = position + 1;
        }
        else{
            break;
        }
    }
    while(1);

    if(hash->position_hash_table < index){
        hash->position_hash_table = index;
    }

    return r;
}

struct hash_table add(struct hash_table *hash, char *key, int value)
{
    int position = hash_func(key, hash->count_data_table);
    if(hash->position_data_table < hash->count_data_table)
    {
        struct row r = {NULL, -1, -1, -1};
        r.key = key;
        r.value = value;
        r.is_deleted = 0;
        r.next_addres = -1;

        if(hash->hash_array[position] == -1)
        {
            hash->hash_array[position] = hash->position_data_table;
            hash->data_array[hash->position_data_table] = r;
        }
        else {
            struct row r_old = get_last_row(hash, position);
            r_old.next_addres = hash->position_data_table;
            hash->data_array[hash->position_data_table] = r;
        }
        hash->position_data_table = hash->position_data_table + 1;
    }
    else
    {
        //Выполнить расширение масства данных если hash->position_hash_table больше определенного значения.
        printf("Hash table overflowing.");
    }
    return *hash;
}


struct hash_table delete_(struct hash_table *hash, char* key){
    int position = hash_func(key, hash->count_data_table);
    struct row r = hash->data_array[hash->hash_array[position]];

    do {
        if(strcmp(r.key, key) == 0){
            r.is_deleted = 1;
            break;
        }
        else {
            if(r.next_addres >= 0) {
                hash->data_array[r.next_addres].is_deleted = 1;
            }
            else {
                break;
            }
        }
    }
    while(1);

    return *hash;
}

int get_(struct hash_table *hash, char *key){
    int position = hash_func(key, hash->count_data_table);
    int index = hash->hash_array[position];
    struct row r = hash->data_array[index];
    int val = -1;
    if(&r == NULL && r.value != -1)
    {
        do {
            if(r.key != NULL && strcmp(r.key, key) == 0){
                break;
            }
            else {
                if(r.next_addres >= 0) {
                    if(&hash->data_array[r.next_addres] != NULL)
                    {
                        r = hash->data_array[r.next_addres];
                    }

                }
                else{
                    break;
                }
            }
        }
        while(1);
    }

    if(r.key != NULL)
    {
        val = r.value;
    }
    return val;
}

struct hash_table set_(struct hash_table *hash, char *key, int *value)
{
    int val = *value;
    int position = hash_func(key, hash->count_data_table);
    int index = hash->hash_array[position];
    struct row r = hash->data_array[index];
    do {
        if(&r != NULL && strcmp(r.key, key) == 0){
            hash->data_array[index].value = val;
            break;
        }
        else {
            if(&r != NULL && r.next_addres >= 0) {
                index = r.next_addres;
                r = hash->data_array[r.next_addres];
            }
            else{
                break;
            }
        }
    }
    while(1);
    return *hash;
}

int equels_split(char* delims, char* elemints)
{
    int result = 0;
    int count = sizeof(delims)/ sizeof(char);

    for(int i = 0; i < count; ++i)
    {
        if(delims[i] == elemints[0])
        {
            result = 1;
            break;
        }
    }
    return result;
}

int split (const char *txt, char *delim, char ***tokens)
{
    printf("%s\n", txt);
    int *tklen, *t, count = 1;
    char **arr, *p = (char *) txt;

    while (*p != '\0')
    {
        if (equels_split(delim, p++))
        {
            count += 1;
        }
    }
    t = tklen = calloc (count, sizeof (int));
    for (p = (char *) txt; *p != '\0'; p++) equels_split(delim, p) ? *t++ : (*t)++;
    *tokens = arr = malloc (count * sizeof (char *));
    t = tklen;
    p = *arr++ = calloc (*(t++) + 1, sizeof (char *));
    while (*txt != '\0')
    {
        if (equels_split(delim, txt))
        {
            p = *arr++ = calloc (*(t++) + 1, sizeof (char *));
            txt++;
        }
        else *p++ = *txt++;
    }
    free (tklen);
    return count;
}


int main(int argc, char **argv)
{
    if(argc > 1){
        struct hash_table hash = create_hash_table(256);
        char buffer[1024];
        char** strings;
        char strs[1024] = {};

        // чтение из файла
        printf(argv[1]);
        printf("\n");
        FILE *fp = fopen(argv[1], "r");
        if(fp)
        {
            int pos = 0;
            while((fgets(buffer, 1024, fp))!=NULL)
            {
                printf("%s", buffer);
                if(pos < 1024)
                {
                    for(int i = 0; i < sizeof(buffer) / sizeof(char); ++i)
                    {
                        if(buffer[i] == '\0')
                            break;
                        strs[pos] = buffer[i];
                        pos++;
                    }
                }
            }
            fclose(fp);
        }
        else
        {
            printf("Path not open.");
            return 0;
        }
        printf("\n");
        printf("\n");
        printf("|\n");
        printf("%s", strs);
        printf("|\n");
        printf("\n");
        int count;
        char delims[] = { 0x20, 0x0A, 0x0D, 0x09 };
        count = split(&strs, delims, &strings);

        printf("%d \n", count);
        for(int i = 0; i < count; ++i)
        {

            printf("|%s|\n", strings[i]);
            int val = get_(&hash, strings[i]);
            if(val <= -1)
            {
                add(&hash, strings[i], 1);
            }
            else
            {
                val = 1 + val;
                set_(&hash, strings[i], &val);
            }
        }

        for(int i = 0; i < sizeof(hash.data_array); ++i)
        {
            if(&hash.data_array[i] != NULL && &hash.data_array[i].key != NULL && hash.data_array[i].value > 0) {
                printf("%s = %d+\n", hash.data_array[i].key, hash.data_array[i].value);
            }
        }
    }
    else
    {
        printf("Wrong first argument file path");
    }

    return 0;
}
