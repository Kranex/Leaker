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

#ifndef LINKED_LIST_H
#define LINKED_LIST_H
#include "stddef.h"

typedef struct _element{
    void* data;
    struct _element* next;
} element;


typedef struct{
    element* head;
    element* tail;
    size_t size, length;
} linked_list;

linked_list* new_linked_list(size_t size);
void* linked_list_replace(linked_list* lst, void* orig, void* data, int (*compar) (const void*, const void*));
void* linked_list_remove(linked_list* lst, void* data, int (*compar) (const void*, const void*));
void linked_list_insert(linked_list* lst, void* data, size_t index);
void linked_list_append(linked_list* lst, void* data);
void linked_list_prepend(linked_list* lst, void* data);
void linked_list_pop(linked_list* lst, void* data, size_t index);
void linked_list_get(linked_list* lst, void* data, size_t index);
void linked_list_destroy(linked_list* lst);

#endif //LINKED_LIST_H
