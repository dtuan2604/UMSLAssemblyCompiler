#ifndef _PARSER_H
#define _PARSER_H
#include "token.h"
#include "tree.h"
#define PARABUFF 500

void scanner();
int matching(int Token, char* TokenIns);

void printParserError(char* fmt, ...);

int isfileEmpty(FILE* fp, char* caller);
void copyToken(struct node_t ** newNode);
struct node_t * runParser();

struct node_t * program();
struct node_t * block();
struct node_t * vars();

struct node_t * stats();
struct node_t * stat();
struct node_t * mStat();

struct node_t * in();
struct node_t * out();
struct node_t * if_();
struct node_t * gen_loop();
struct node_t * loop1();
struct node_t * loop2();
struct node_t * assign();
struct node_t * goto_();
struct node_t * label();
struct node_t * condition(); //extra non-terminal
struct node_t * RO();

struct node_t * expr();
struct node_t * N();
struct node_t * N_();
struct node_t * A();
struct node_t * M();
struct node_t * R();

#endif
