#include "json_miniparser.h"
#include <stdio.h>
#include <string.h>

enum Actions {MEAN, DISTANCE, MAX, MIN};

void wp_mean(LexMap L, Graph G, List WL);
void wp_distance(LexMap L, Graph G, List WL, size_t d);
void wp_max(LexMap L, Graph G, List WL);
void wp_min(LexMap L, Graph G, List WL);
void _view_neigh(LexMap L, Graph G, char *word);

void _view_neigh(LexMap L, Graph G, char *word) {
    size_t w_index = get_index(L, word);
    printf("Neighbors of word <%s> : ", word);
    for (size_t i=0; i<G->degrees[w_index]; i++) {
        if (i!=0) printf(" ; ");
        printf("%s", L->words[G->nodes[w_index][i]]);
    }
    printf("\n");
}

void wp_mean(LexMap L, Graph G, List WL) {

    for (size_t i=0; i<WL->len; i++) {

        size_t acc = 0;
        size_t total = 0;
        double ratio;

        List P = shortest_path(G, get(WL, i));
        
        for (size_t j=0; j<WL->len; j++) {
            size_t v = get(P, get(WL, j));
            if (v != (size_t)(-1)) {
                acc += v;
                total++;
            }
            
        }
        if (total==0) {
            printf("Word <%s> doesn't reach any word ion list.\n", L->words[get(WL, i)]);
        }
        else {
            ratio = ((double)acc)/total;
            printf("Average distance from word <%s> to other words from list is %f.\n", L->words[get(WL, i)], ratio);
        }
        

        destroy(P);
    }
}

void wp_distance(LexMap L, Graph G, List WL, size_t d) {

    for (size_t i=0; i<WL->len; i++) {

        printf("Words at distance %zu of word <%s> are : ",d,  L->words[get(WL, i)]);

        List P = shortest_path(G, get(WL, i));
        int print_started = 0;
        
        for (size_t j=0; j<WL->len; j++) {
            size_t v = get(P, get(WL, j));
            if (v==d) {
                if (print_started) printf(", ");
                printf("%s", L->words[get(WL, j)]);
                print_started = 1;

            }
        }
        printf("\n");

        destroy(P);
    }
}

void wp_max(LexMap L, Graph G, List WL) {
    for (size_t i=0; i<WL->len; i++) {

        List P = shortest_path(G, get(WL, i));
        
        size_t M_index;
        size_t Max = 0;

        for (size_t j=0; j<WL->len; j++) {
            size_t v = get(P, get(WL, j));
            if (v<(size_t)(-1) && v>Max) {
                M_index = get(WL, j);
                Max = v;
            }
        }

        printf("Farthest word to word <%s> is <%s> with distance %zu.\n", L->words[get(WL, i)], L->words[M_index], Max);

        destroy(P);
    }
}

void wp_min(LexMap L, Graph G, List WL) {
    for (size_t i=0; i<WL->len; i++) {

        List P = shortest_path(G, get(WL, i));
        
        size_t M_index;
        size_t Min = (size_t)(-1);

        for (size_t j=0; j<WL->len; j++) {
            size_t v = get(P, get(WL, j));
            if (v>0 && v<Min) {
                M_index = get(WL, j);
                Min = v;
            }
        }

        printf("Closest word to word <%s> is <%s> with distance %zu.\n", L->words[get(WL, i)], L->words[M_index], Min);

        destroy(P);
    }
}


void main(int argc, char *argv[]) {

    char *filename;
    int action = -1;
    size_t d;
    List WordList;
    
    if (argc==1 || argc==3) {
        printf("do i look like a man ? rtfm dude !\n");
        return;
    }

    if (argc==2 && (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))) {
        printf("Word path.\n");
        printf("\n");
        printf("Usage:\n");
        printf("wp [options] -f <dictionary.json> [words...]\n");
        printf("wp -h | --help\n");
        printf("wp --version\n");
        printf("\n");
        printf("Options:\n");
        printf("-h --help                   Show this screen.\n");
        printf("-f --file <dictionary.json> Use file dictionary.json as input\n");
        printf("--mean                      Print the mean distance of all path between words.\n");
        printf("--distance=<k>              Print all couples of words with distance k\n");
        printf("--max                       Print the two words with the max distance\n");
        printf("--min                       Print the two words with the min distance\n");
        return;
    }

    if (argc==2 && !strcmp(argv[1], "--version")) {
        printf("Wordpath version .42beta CC-0\n");
        return;
    }

    if (argc>=4) {

        if (!strcmp(argv[1], "--mean")) {
            action = MEAN;
        }
        if (!strcmp(argv[1], "--max")) {
            action = MAX;
        }
        if (!strcmp(argv[1], "--min")) {
            action = MIN;
        }
        if (!strncmp(argv[1], "--distance=", 11)) {
            action = DISTANCE;
            sscanf(argv[1], "--distance=%zu", &d);
        }
        if (action == -1) {
            printf("do i look like a man ? rtfm dude !\n");
            return;
        }


        // check if there a format option|-f|dictionnary name|word list
        if (strcmp(argv[2], "-f") && strcmp(argv[2], "--file")) {
            printf("do i look like a man ? rtfm dude !\n");
            return;
        }
        filename = argv[3];

        

        
    }
    else {
        printf("do i look like a man ? rtfm dude !\n");
        return;
    }

    printf("Begin json parsing ");
    LexMap lex = json_miniparse(filename);
    printf("done.\n");

    printf("%zu words in dictionnary.\n", lex->n_words);

    printf("Building word list ");
    WordList = new_list();
    if (argc>4) {
        
        for (size_t i=4; i<argc; i++) {
            if (contains(lex, argv[i])) {
                push(WordList, get_index(lex, argv[i]));
            }
            else {
                printf("\nSkipping unknown word <%s>.", argv[i]);
            }
            
        }
    }
    else {
        for (size_t i=0; i<lex->n_words; i++) {
            push(WordList, i);
        }
    }
    printf("done.\n");


    printf("Building graph ");
    Graph G = graph_parser(lex);
    printf("done.\n");

    switch (action)
    {
    case MEAN:
        wp_mean(lex, G, WordList);
        break;
    
    case MAX:
        wp_max(lex, G, WordList);
        break;

    case MIN:
        wp_min(lex, G, WordList);
        break;

    case DISTANCE:
        wp_distance(lex, G, WordList, d);
        break;

    default:
        printf("Error.\n");
        break;
    }

}