#ifndef _NODE_H
#define _NODE_H
#include "token.h"

#define MAXTOKENS 5

struct node_t{
	char* name;
	int numToken;
	struct token ** tokenList; 
	struct node_t* left;
	struct node_t* middle;
	struct node_t* right;	
};


#endif
