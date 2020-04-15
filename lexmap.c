#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexmap.h"

//----------------
// Node functions
//----------------

/*
create a new node with a given character and no sons
*/
Node new_node(char c)
{
    Node n = (Node)malloc(sizeof(_NODE));
    n->c = c;
    n->n_sons = 0;
    n->sons = new_nodelist();
    n->is_defined = 0;
    n->definition = NULL;
    return n;
}

/*
setup the definition fo a word
*/
void set_definition(Node n, char *def)
{
    n->definition = (char *)malloc(sizeof(char)*(strlen(def)+1));
    strcpy(n->definition, def);
    n->is_defined = 1;
}

/*
add a son node to a given node
*/
void add_son(Node n, Node son)
{
    add_node(n->sons, son);
    n->n_sons++;
}

void print_node(Node n) {
    printf("n_sons: %zu\n", n->n_sons);
    printf("is_defined: %d\n", n->is_defined);
    if (n->definition!=NULL) {
        printf("definition:%s\n", n->definition);
        printf("index:%zu\n", n->index);
    }

}

//--------------------
// NodeList functions
//--------------------

/*
create a new empty node list
*/
NodeList new_nodelist()
{
    NodeList L = (NodeList)malloc(sizeof(_NODELIST));
    L->node = NULL;
    L->next = NULL;
}

/*
append a new node at end of list
*/
void add_node(NodeList L, Node n)
{
    if (L->node == NULL)
    {
        L->node = n;
        return;
    }

    NodeList current = L;
    while (current->next != NULL)
    {
        current = current->next;
    }
    current->next = new_nodelist();
    add_node(current->next, n);
}

Node get_node(NodeList L, char c)
{
    if (L->node == NULL)
    {
        return NULL;
    }
    if (L->node->c == c)
    {
        return L->node;
    }

    NodeList current = L;
    while (current->next != NULL)
    {
        current = current->next;
        if (current->node->c == c)
        {
            return current->node;
        }
    }
    return NULL;
}

//-------------------
// LexMap functions
//-------------------
// Important : this data structure is implemented as more static one, not
// for dynamical adding/removing entries

/*
create a new lexical mapping, i.e. a tree for fast lexical search of 
entries.
*/
LexMap new_lexmap()
{
    LexMap LM = (LexMap)malloc(sizeof(_LEXMAP));
    LM->n_elements = 0;
    LM->n_words = 0;
    LM->sons = new_nodelist();
    return LM;
}

/*
add a new word to the entry
*/
void add_word(LexMap L, char *word, char *definition) {

    if (L->n_words == 0) {
        L->words = (char **)malloc(sizeof(char *));
    }
    else {
        
        L->words = (char **)realloc(L->words, sizeof(char *)*(L->n_words+1));
    }
    L->words[L->n_words] = (char *)malloc(sizeof(char)*(strlen(word)+1));
    strcpy(L->words[L->n_words], word);

    char current_character;
    size_t word_len = strlen(word);

    if (word_len == 0) return;

    // initialisation for the first character
    current_character = word[0];

    // create first node if it doesn't exists
    Node current_node = get_node(L->sons, current_character);
    if (current_node == NULL) {

        current_node = new_node(current_character);
        add_node(L->sons, current_node);
        L->n_elements++;
    }

    // walk the tree, creating nodes when needed
    for (int i = 1; i<word_len; i++) {
        current_character = word[i];
        Node next_node = get_node(current_node->sons, current_character);
        if (next_node == NULL) {
            next_node = new_node(current_character);
            add_node(current_node->sons, next_node);
            L->n_elements++;
        }
        current_node = next_node;
    }

    // define the word (erease older definition if already defined)
    set_definition(current_node, definition);
    current_node->index = L->n_words;
    L->n_words++;
}

/*
walk the tree to check if a given word exists in base
*/
int contains(LexMap L, char *word) {

    Node n = to_node(L, word);
    if (n==NULL) return 0;
    else return n->is_defined;
}

/*
return the index of word in map
*/
size_t get_index(LexMap L, char *word) {
    if (contains(L, word)) {
        Node n = to_node(L, word);
        return n->index;
    }
    else {
        return (size_t)(-1);
    }
}

/*
walk the tree the return the definition of the given word, or null
*/
char *get_definition(LexMap L, char *word) {
    Node n = to_node(L, word);
    if (n==NULL) return NULL;
    else return n->definition;
}

/*
walk the tree to find a given node matching a path
*/
Node to_node(LexMap L, char *path) {
    char current_character;
    size_t path_len = strlen(path);

    if (path_len == 0) return NULL;

    // initialisation for the first character
    current_character = path[0];

    Node current_node = get_node(L->sons, current_character);
    if (current_node == NULL) return NULL;

    // walk the tree until word found or exit
    for (int i = 1; i<path_len; i++) {
        current_character = path[i];
        current_node = get_node(current_node->sons, current_character);
        if (current_node == NULL) return NULL;
    }

    // once final lexem reached, return 1 if is defined
    return current_node;
}
