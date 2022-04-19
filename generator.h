#ifndef _GENERATOR_H_
#define _GENERATOR_H_

#include "node.h"
#define MAX_BLOCKS 100

int isOverflow(); 

void initVarcount();
void destroyVarcount();
void semanticCheck(struct node_t* nodePtr);


#endif
