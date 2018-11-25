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
#include <assert.h>
#include "linked_list.h"

/**
 * Returns a new empty linked list.
 * @param size the size of elements in the list.
 * @return an empty linked list.
 */
linked_list* new_linked_list(size_t size) {
    linked_list* lst = malloc(sizeof(linked_list));
    lst->size = size;
    lst->head = NULL;
    lst->tail = NULL;
    lst->length=0;
    return lst;
}

void linked_list_insert(linked_list* lst, void* data, size_t index){
    assert(index <= lst->length);
    if(index == 0) {
        //special case.
        linked_list_prepend(lst, data);
        return;
    }else if(index == lst->length){
        //more efficient.
        linked_list_append(lst, data);
        return;
    }

    element* e = malloc(sizeof(element));

    // put data in e.
    e->data = malloc(lst->size);
    memcpy(e->data, data, lst->size);

    element* o = NULL; // previous element;
    element* n = lst->head; // next element;

    // descend list.
    while(index-- > 0){
        o = n;
        n = o->next;
    }

    if(o == NULL){
        // if appending.
        lst->head = e;
    }else{
        // if inserting.
        o->next = e;
    }

    e->next = n;
    lst->length++;
}

void* linked_list_replace(linked_list* lst, void* orig, void* data, int (*compar) (const void*, const void*)){
    element* e = lst->head;

    while(e != NULL){
        if(compar(e->data, orig) == 0){
            void* old;

            void* n = malloc(lst->size);
            memcpy(n, data, lst->size);

            old = e->data;
            e->data = n;
            return old;
        }
        e = e->next;
    }

    return NULL;
}

void* linked_list_remove(linked_list* lst, void* data, int (*compar) (const void*, const void*)){
    element* o = NULL;
    element* e = lst->head;

    while(e != NULL){
        if(compar(e->data, data) == 0){
            void* old = e->data;

            if(o == NULL){
                lst->head = e->next;
            }else{
                o->next = e->next;
            }
            free(e);
            lst->length--;
            return old;
        }
        o = e;
        e = o->next;
    }

    return NULL;
}

/**
 * Appends a new element to the list.
 * @param lst the list.
 * @param data pointer to the element.
 * @param size the size of the element.
 */
void linked_list_append(linked_list* lst, void* data) {
    if(lst->length == 0){
        linked_list_prepend(lst, data);
        return;
    }
    element* e = malloc(sizeof(element));

    //cpy data.
    e->data = malloc(lst->size);
    memcpy(e->data, data, lst->size);
    //init next.
    e->next = NULL;

    //append
    lst->tail->next = e;
    lst->tail = e;
    lst->length++;
}

void linked_list_prepend(linked_list* lst, void* data){
    element* e = malloc(sizeof(element));

    //cpy data.
    e->data = malloc(lst->size);
    memcpy(e->data, data, lst->size);
    //init next.
    e->next = lst->head;

    //prepend
    lst->head = e;
    if(lst->length == 0) lst->tail = e;
    lst->length++;
}

void linked_list_pop(linked_list* lst, void* data, size_t index){
    assert(lst->length > 0);
    assert(index < lst->length);

    element* e = lst->head; // current element;
    element* o = NULL; // old element;

    // descend list.
    while(index-- > 0){
        o = e;
        e = o->next;
    }

    // copy data if there is somewhere to put it.
    if(data != NULL){
        memcpy(data, e->data, lst->size);
    }

    //remove element e.
    if(o == NULL){
        // if index 0 was requested.
        lst->head = e->next;
    }else{
        o->next = e->next;
    }
    lst->length--;
    free(e->data);
    free(e);
}

void linked_list_get(linked_list* lst, void* data, size_t index){
    assert(lst->length > 0);
    assert(index < lst->length);

    element* e = lst->head; // current element;
    element* o = NULL; // old element;

    // descend list.
    while(index-- > 0){
        o = e;
        e = o->next;
    }

    // copy data.
    memcpy(data, e->data, lst->size);
}

void linked_list_destroy(linked_list* lst){
    // Destroy all the nodes.
    while(lst->length > 0){
        linked_list_pop(lst, NULL, 0);
    }
    free(lst);
}
