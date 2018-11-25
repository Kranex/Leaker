/* Copyright 2018 Oliver Strik
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef HASH_MAP_H
#define HASH_MAP_H

#include "stddef.h"
#include "linked_list.h"

typedef struct{
    linked_list** bucketArr;
    size_t buckets, elements;
    size_t keySize, dataSize;
    size_t (*hash)(const void*);
    int (*compar)(const void*, const void*);
} hash_map;

typedef struct{
    void * key;
    void * data;
    int (*compar)(const void*, const void*);
}key_data_pair;

/**
 * Creates a new hash_map and returns a pointer to ot. If nBuckets is 0, then it will use the default of INCREMENT.
 * @param nBuckets the number of initial buckets.
 * @param keySize the sizeof a key.
 * @param dataSize the sizeof the data.
 * @param hash pointer to the function for hashing keys.
 * @param compar pointer to the function for comparing keys.
 * @return a pointer to the new hash_map.
 */
hash_map* new_hash_map(size_t nBuckets, size_t keySize, size_t dataSize, size_t (*hash)(const void*), int (*compar)(const void*, const void*));
void hash_map_put(hash_map* map, void * key, void * data);
int hash_map_get(hash_map* map, void* key, void* data);
int hash_map_remove(hash_map* map, void*key, void* data);
int hash_map_remove_first(hash_map* map, void* data);
int hash_map_compare_key_data_pair(const void* a, const void* b);
void destroy_hash_map(hash_map* map);

#endif //HASH_MAP_H
