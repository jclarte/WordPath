#ifndef JSON_MINIPARSER_H
#define JSON_MINIPARSER_H

#include <stdio.h>
#include "lexmap.h"
#include "graphs.h"

LexMap json_miniparse(char *filename);
Graph graph_parser(LexMap L);

#endif