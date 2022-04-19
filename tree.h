#ifndef _TREE_H
#define _TREE_H


#define MAXTOKEN 8
#define DEF_NODES 10

void initROOT();
void upsizeROOT();
void destroyROOT();

struct node_t* createNode(char* name);



#endif
