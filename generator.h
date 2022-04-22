#ifndef _GENERATOR_H_
#define _GENERATOR_H_

#include "node.h"
#define MAX_BLOCKS 100
enum{ VAR = 1111,LABEL };

char* newName(int type);
void codeGen(struct node_t *nodePtr);
int isAbletoGencode(struct node_t *nodePtr);

int isOverflow(); 
void initVarcount();
void destroyVarcount();
void semanticCheck(struct node_t* nodePtr);


#endif
