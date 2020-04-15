#ifndef LEXMAP_H
#define LEXMAP_H

typedef struct _LEXMAP *LexMap;
typedef struct _NODE *Node;
typedef struct _NODELIST *NodeList;

typedef struct _NODE {
    char c;
    size_t n_sons;
    int is_defined;
    char *definition;
    size_t index;
    NodeList sons;
} _NODE;

typedef struct _LEXMAP {
    size_t n_elements;
    size_t n_words;
    char **words;
    NodeList sons;
} _LEXMAP;

typedef struct _NODELIST {
    Node node;
    NodeList next;
} _NODELIST;

// LexMap functions
LexMap new_lexmap();
void add_word(LexMap L, char *word, char *definition);
int contains(LexMap L, char *word);
char *get_definition(LexMap L, char *word);
Node to_node(LexMap L, char *path);
void print_dict(LexMap L);
size_t get_index(LexMap L, char *word);

// Node functions
Node new_node(char c);
void set_definition(Node n, char *def);
void add_son(Node n, Node son);
void print_node(Node n);

// NodeList functions
NodeList new_nodelist();
void add_node(NodeList L, Node n);
Node get_node(NodeList L, char c);

#endif