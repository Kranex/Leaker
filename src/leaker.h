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

#ifndef C_LEAKER_H
#define C_LEAKER_H
#include "stddef.h"
#define malloc(x) _leaker_malloc(x, __LINE__, __FILE__)
#define calloc(x, y) _leaker_malloc(x, y, __LINE__, __FILE__)
#define realloc(x,y) _leaker_realloc(x,y, __LINE__, __FILE__)
#define reallocf(x, y) _leaker_reallocf(x, y, __LINE__, __FILE__)
#define valloc(x) _leaker_valloc(x, __LINE__, __FILE__)
#define free(x) _leaker_free(x)

void start_leaker();
void * _leaker_malloc(size_t size, int line, const char file[]);
void * _leaker_calloc(size_t count, size_t size, int line, const char file[]);
void * _leaker_realloc(void * ptr, size_t size, int line, const char file[]);
void * _leaker_reallocf(void * ptr, size_t size, int line, const char file[]);
void * _leaker_valloc(size_t size, int line, const char file[]);
void _leaker_free(void * ptr);
void end_leaker();

#endif //C_LEAK_H

