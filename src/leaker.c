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
#include <printf.h>
#include <limits.h>
#include <assert.h>
#include <string.h>
#include "hash_map.h"

hash_map* allocs;
int ALLOC_COUNTER;
int FREE_COUNTER;

size_t hash_ptr(const void * ptr){
    return (size_t) *((size_t*)ptr);
}

int compar_ptr(const void * a, const void *b){
    return (int)((*(size_t*)b%10)-(*(size_t*)a)%10);
}

void start_leaker(){
    allocs = new_hash_map(500, sizeof(void*), sizeof(size_t), &hash_ptr, &compar_ptr);
}

void * _leaker_malloc(size_t size, int line, const char file[]){

    void * ptr = malloc(size);
    if(ptr != NULL){
        char* a = malloc((strlen(file)+13) * sizeof(char));
        size_t pa = (size_t)a;
        sprintf(a, "%s:%d:", file, line);
        hash_map_put(allocs, &ptr, &pa);
    }
    return ptr;
}
void * _leaker_calloc(size_t count, size_t size, int line, const char file[]){
    void * ptr = calloc(count, size);
    if(ptr != NULL){
        char* a = malloc((strlen(file)+13) * sizeof(char));
        size_t pa = (size_t)a;
        sprintf(a, "%s:%d:", file, line);
        hash_map_put(allocs, &ptr, &pa);
    }
    return ptr;
}
void * _leaker_realloc(void * ptr, size_t size, int line, const char file[]){
    void *p = realloc(ptr,size);

    if(p != NULL){
        size_t s;
        hash_map_remove(allocs, &ptr, &s);
        char* a = (char*)s;
        free(a);

        a = malloc((strlen(file)+13) * sizeof(char));
        size_t pa = (size_t)a;
        sprintf(a, "%s:%d:", file, line);
        hash_map_put(allocs, &p, &pa);
    }

    return p;
}
void * _leaker_reallocf(void * ptr, size_t size, int line, const char file[]){
    void *p = reallocf(ptr,size);
    if(p == NULL){
        size_t s;
        hash_map_remove(allocs, &ptr, &s);
        char* a = (char*)s;
        free(a);
    };
    return p;
}
void * _leaker_valloc(size_t size, int line, const char file[]){
    void*ptr = valloc(size);
    if(ptr != NULL){
        char* a = malloc((strlen(file)+13) * sizeof(char));
        size_t pa = (size_t)a;
        sprintf(a, "%s:%d:", file, line);
        hash_map_put(allocs, &ptr, &pa);
    }
    return ptr;
}

void _leaker_free(void * ptr){
    size_t s;
    hash_map_remove(allocs, &ptr, &s);
    char* a = (char*)s;
    free(a);
    free(ptr);
}

void end_leaker(){
    printf("===================\n");
    printf("|   Leaker v0.1   |\n");
    printf("===================\n");
    if(allocs->elements > 0){
        size_t s;
        while(hash_map_remove_first(allocs, &s)){
            char* a = (char*)s;
            printf("%s not freed.\n", a);
            free(a);
        }
    } else {
        printf("No leaks detected!\n");
    }
    destroy_hash_map(allocs);
}