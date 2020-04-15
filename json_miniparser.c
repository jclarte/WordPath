#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "json_miniparser.h"
#include "lists.h"
#define BUFFSIZE 4096

enum ParserState {NOTHING, KEY, INBETWEEN, VALUE};

LexMap json_miniparse(char *filename) {

    LexMap lex = new_lexmap();

    char current_char;
    int buff_position = 0;
    int skip_next = 0;
    char current_word[BUFFSIZE];
    char current_description[BUFFSIZE*8];

    int STATE = NOTHING;

    FILE *fp;
    fp = fopen(filename, "r");
    if (fp == NULL) perror ("Error opening file");
    do {
        current_char = fgetc(fp);
        if (skip_next) {
            current_char = fgetc(fp);
            skip_next = 0;
        }
        switch (current_char)
        {
        case '"':
            switch (STATE)
            {
            case KEY:
                current_word[buff_position] = '\0';
                buff_position = 0;
                break;

            case VALUE:
                current_description[buff_position] = '\0';
                add_word(lex, current_word, current_description);

                buff_position = 0;
                break;
            
            default:
                break;
            }
            STATE++;
            STATE%=4;
            break;
        
        case '\\':
            skip_next = 1;
            break;

        default:

            switch (STATE)
            {
            case KEY:
                current_word[buff_position] = current_char;
                buff_position++;
                break;

            case VALUE:
                current_description[buff_position] = current_char;
                buff_position++;
                break;
            
            default:
                break;
            }

            break;
        }
    }
    while (current_char != EOF);

    fclose(fp);

    return lex;
}

List description_parser(LexMap L, char *description) {
    char current_char;
    Node current_node;
    Node next_node;
    int state = 0;
    int start_or_non_alpha_previous = 1;
    List succ_list = new_list();

    for (int i=0; i<strlen(description); i++) {
        current_char = description[i];
        if (i>0) {
            start_or_non_alpha_previous = (!isalpha(description[i-1]));
        }
        if (current_char>=65 && current_char<=90) current_char += 32; // lower case
        if (state == 0){

            // only check if current char start a word

            if (isalpha(current_char)) {
                current_node = get_node(L->sons, current_char);

                if (current_node != NULL && (!isalpha(description[i-1]))) {
                    state = 1;
                }
            }

            
        }
        else {
            next_node = get_node(current_node->sons, current_char);
            if (next_node == NULL) {
                if (current_node->is_defined) {
                    if (!l_contains(succ_list, current_node->index)) push(succ_list, current_node->index);
                }
                state = 0;
                current_node = NULL;
            }
            else {
                current_node = next_node;
            }
        }
    }

    return succ_list;
    
}

Graph graph_parser(LexMap L) {
    Graph G = new_digraph(L->n_words);

    for (size_t i=0; i<L->n_words; i++) {

        char *word = L->words[i];
        char *def = get_definition(L, word);

        List succ = description_parser(L, def);

        if (!is_empty(succ)) {
            size_t succ_size = succ->len;

            for (size_t j=0; j<succ_size;j++) {
                size_t dest = get(succ, j);
                add_edge(G, i, dest);
            }
        }

        destroy(succ);
    }

    return G;

}