#include "lists.h"



//create new empty list
List new_list()
{
    List L = (List)malloc(sizeof(_List));
    L->len = 0;
    return L;
}

//check if list is empty
int is_empty(List L)
{
    return !L->len;
}

/*
simple way to view a list of integers
*/
void printl(List L)
{
    if (L->len == 0)
    {
        printf("Empty list\n");
        return;
    }
    
    _Element *current_element = L->start;

    for (size_t i = 0; i < (L->len - 1); i++)
    {
        printf("%zu ; ", current_element->value);
        current_element = current_element->next;
    }
    printf("%zu\n", current_element->value);
    
}
// List copy(List L);

/*
add a new element containing value
*/
void push(List L, size_t value)
{
    _Element *new_element = (_Element *)malloc(sizeof(_Element));

    new_element->value = value;
    new_element->next = NULL;

    if (L->len == 0)
    {
        L->start = new_element;
    }
    else
    {
        L->end->next = new_element;
    }
    
    new_element->last = L->end;
    L->end = new_element;
    L->len ++;

}


/*
private function to get element of specific index
*/
_Element *_reach(List L, size_t index)
{
    if (is_empty(L)) return NULL;
    if (index>L->len) return L->end;

    _Element *current_element = L->start;
    for (size_t i = 0; i<index;i++)
    {
        current_element = current_element->next;
    }
    return current_element;
}

/*
return the content of element of L of index index
*/
size_t get(List L, size_t index)
{
    _Element *reached_element = _reach(L, index);
    return reached_element->value;
}


/*
remove the element of index index from list
*/
void pop(List L, size_t index)
{
    if (index<0 || index >= L->len) return;

    _Element *reached_element = _reach(L, index);

    if (index==0) {
        L->start = reached_element->next;
    }
    else {
        reached_element->last->next = reached_element->next;
    }

    
    if (reached_element->next == NULL) {
        L->end = reached_element->last;
    }
    else {
        reached_element->next->last = reached_element->last;
    }
    L->len--;
    reached_element->last = NULL;
    reached_element->next = NULL;
    free(reached_element);
}

int l_contains(List L, size_t value) {
    for (size_t index=0; index<L->len; index++) {
        _Element *reached_element = _reach(L, index);
        if (reached_element->value == value) return 1;
    }
    return 0;
}

/*
swap values of elements of index a and index b if they are valid
*/
void swap(List L, size_t index_a, size_t index_b)
{
    if (index_a < 0 || index_b < 0 || index_a >= L->len || index_b >= L->len) return;

    size_t tmp;
    _Element *element_a = _reach(L, index_a);
    _Element *element_b = _reach(L, index_b);
    tmp = element_a->value;
    element_a->value = element_b->value;
    element_b->value = tmp;
}

/*
tail is a sublist of all elements except the first
*/
List tail(List L)
{
    List Tail = new_list();
    Tail->start = L->start->next;
    Tail->end = L->end;
    Tail->len = L->len-1;
    return Tail;
}

/*
make a fresh copy of list
*/
List copy(List L)
{
    List L_copy = new_list();
    for (size_t i = 0; i<L->len; i++ )
    {
        push(L_copy, get(L, i));
    }
    return L_copy;
}

/*
concatenation of L_first with a copy of L_last (to avoid many problems)
*/
void concat(List L_first, List L_last)
{
    List L_last_copy = copy(L_last);
    L_first->end->next = L_last_copy->start;
    L_first->end = L_last_copy->end;
    L_first->len += L_last_copy->len;
}

/*
insert element of value to position index, shifting all elements to the end
accept position = len of list to push element at the end
*/
void insert(List L, size_t index, size_t value)
{

    if (index<0 || index > L->len) return;

    _Element *new_element = (_Element *)malloc(sizeof(_Element));
    new_element->value = value;

    if (index==0)
    {
        L->start = new_element;
        L->start->last = new_element;
        L->len++;
        return;
    }

    if (index==L->len)
    {
        push(L, value);
        return;
    }
    
    _Element *element_after = _reach(L, index);
    _Element *element_before = _reach(L, index-1);
    
    new_element->last = element_before;
    new_element->next = element_after;

    element_before->next = new_element;
    element_after->last = new_element;

    L->len++;
}

void destroy(List L) {
    if (is_empty(L)) {
        free(L);
        return;
    }

    _Element *c, *n;
    c = L->start;
    L->start = NULL;
    L->end = NULL;
    while (c->next != NULL) {
        n = c->next;
        free(c);
        c = n;
    }
    free(L);
}