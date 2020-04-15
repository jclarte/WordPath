#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>

typedef struct _Element
{
    size_t value;
    struct _Element *last;
    struct _Element *next;
} _Element;

typedef struct _List
{
    size_t len;
    struct _Element *start;
    struct _Element *end;
} _List;

typedef _List *List;

List new_list();
List copy(List L);
int is_empty(List L);

void push(List L, size_t value);
size_t get(List L, size_t index);
void printl(List L);
void pop(List L, size_t index);
void swap(List L, size_t index_a, size_t index_b);
void insert(List L, size_t index, size_t value);
void concat(List L_first, List L_last);
int l_contains(List L, size_t value);
List copy(List L);
List tail(List L);

void destroy(List L);

#endif