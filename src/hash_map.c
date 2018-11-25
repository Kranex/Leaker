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

#include <stdlib.h>
#include <string.h>
#include "hash_map.h"
#include "linked_list.h"

#define INCREMENT 32

/**
 * Creates a new hash_map and returns a pointer to ot. If nBuckets is 0, then it will use the default of INCREMENT.
 * @param nBuckets the number of initial buckets.
 * @param keySize the sizeof a key.
 * @param dataSize the sizeof the data.
 * @param hash pointer to the function for hashing keys.
 * @param compar pointer to the function for comparing keys.
 * @return a pointer to the new hash_map.
 */
hash_map* new_hash_map(size_t nBuckets, size_t keySize, size_t dataSize, size_t (*hash)(const void*), int (*compar)(const void*, const void*)){
    hash_map* map = malloc(sizeof(hash_map));

    map->buckets = nBuckets ? nBuckets : INCREMENT;
    map->elements = 0;
    map->keySize = keySize;
    map->dataSize = dataSize;
    map->hash = hash;
    map->compar = compar;

    map->bucketArr = malloc(map->buckets*sizeof(linked_list*));
    for(int i = 0; i < map->buckets; i++){
        map->bucketArr[i] = new_linked_list(sizeof(key_data_pair));
    }
    return map;
}

/**
 *
 * @param map
 * @param key
 * @param data
 */
void hash_map_put(hash_map* map, void * key, void * data){
    key_data_pair kdp;
    kdp.key = malloc(map->keySize);
    memcpy(kdp.key, key, map->keySize);
    kdp.data = malloc(map->dataSize);
    memcpy(kdp.data, data, map->dataSize);
    kdp.compar = map->compar;

    size_t bi = map->hash(key)%(map->buckets); // get bucket index.
    linked_list* list = map->bucketArr[bi]; // get the bucket.
    key_data_pair* ret = linked_list_replace(list, &kdp, &kdp, &hash_map_compare_key_data_pair);
    if(ret == NULL){
        linked_list_append(list, &kdp);
        map->elements++;
    }else{
        free(ret->key);
        free(ret->data);
        free(ret);
    }; // put element in the bucket.

    if(((double)map->elements) / ((double)map->buckets) > 0.75){
        //resize
    }
}

int hash_map_get(hash_map* map, void* key, void* data){
    size_t li = 0, bi = map->hash(key)%(map->buckets); // get bucket index.
    element* e = (map->bucketArr[bi])->head;
    if(e == NULL) return 0;
    key_data_pair* kdp = ((key_data_pair*)e->data);

    while(e != NULL){
        if(kdp->compar(kdp->key, key) == 0){
            memcpy(data, kdp->data, map->dataSize);
            return 1;
        }
        e = e->next;
    }

    return 0;
}

int hash_map_remove(hash_map* map, void*key, void* data){
    size_t bi = map->hash(key)%(map->buckets); // get bucket index.
    key_data_pair tmp;
    tmp.key = malloc(map->keySize);
    memcpy(tmp.key, key, map->keySize);
    tmp.compar = map->compar;
    key_data_pair* kdp = linked_list_remove(map->bucketArr[bi], &tmp, &hash_map_compare_key_data_pair);
    if(kdp == NULL) return 0;
    memcpy(data, kdp->data, map->dataSize);
    free(kdp->data);
    free(kdp->key);
    free(kdp);
    free(tmp.key);
    map->elements--;
    return 1;
}

int hash_map_remove_first(hash_map* map, void* data){
    if(map->elements <= 0) return 0;
    key_data_pair kdp;
    for(int i = 0; i < map->buckets; i++){
        linked_list* lst = map->bucketArr[i];
        if(lst->length > 0){
            linked_list_pop(lst, &kdp, 0);
            break;
        }
    }
    memcpy(data, kdp.data, map->dataSize);
    free(kdp.data);
    free(kdp.key);
    map->elements--;
    return 1;
}

int hash_map_compare_key_data_pair(const void* a, const void* b){
    void* ka = ((key_data_pair*)a)->key;
    void* kb = ((key_data_pair*)b)->key;
    return ((key_data_pair*)a)->compar(ka,kb);
}

void destroy_hash_map(hash_map* map){
    linked_list* lst;
    key_data_pair kdp;
    for(int i = 0; i < map->buckets; i++){
        lst = map->bucketArr[i];
        while (lst->length > 0){
            linked_list_pop(lst, &kdp, 0);
            free(kdp.data);
            free(kdp.key);
        }
        free(lst);
    }
    free(map->bucketArr);
    free(map);
}