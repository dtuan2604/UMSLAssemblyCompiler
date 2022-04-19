#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "node.h"
#include <string.h>

int curr = 0;
int curSize = DEF_NODES;
struct node_t ** ROOT = NULL; 
char* caller = "tree";

void initROOT(){
	ROOT = (struct node_t **)malloc(curSize * sizeof(struct node_t *));
	if(ROOT == NULL){
		fprintf(stderr,"ERROR: %s: failed to allocate ROOT memory\n",caller);
		exit(-1);
	}
	
	int i;
	for(i = 0; i < curSize; i++){
		ROOT[i] = (struct node_t *)malloc(sizeof(struct node_t));
		
 		if(ROOT[i] == NULL){
                	fprintf(stderr,"ERROR: %s: failed to allocate ROOT memory\n",caller);
                	exit(-1);
        	}
	}
}

void upsizeROOT(){
	curSize += DEF_NODES;
	struct node_t ** tempROOT = (struct node_t **)malloc(curSize * sizeof(struct node_t *));
	if(tempROOT == NULL){
                fprintf(stderr,"ERROR: %s: failed to allocate tempROOT memory\n",caller);
                exit(-1);
        }
	
	int i;
	for(i = 0; i < curr; i++){
		tempROOT[i] = ROOT[i];
	}

	for(;i < curSize; i++){
		tempROOT[i] = (struct node_t *)malloc(sizeof(struct node_t));

                if(tempROOT[i] == NULL){
                        fprintf(stderr,"ERROR: %s: failed to allocate ROOT memory\n",caller);
                        exit(-1);
                }

	}
	free(ROOT);

	ROOT = tempROOT;
}
void destroyROOT(){
	int i, j;
	for(i = 0; i < curr; i++){
		for(j = 0; j < MAXTOKENS; j++){
			free(ROOT[i]->tokenList[j]);
		}
		free(ROOT[i]);
		ROOT[i] = NULL;
	}
	for(;i<curSize; i++){
		free(ROOT[i]);
		ROOT[i] = NULL;
	}
	free(ROOT);
	ROOT = NULL;
}

struct node_t* createNode(char* name){
	if(curr >= curSize)
		upsizeROOT();	
	
	struct node_t * newNode = (struct node_t*)malloc(sizeof(struct node_t));
	if(newNode == NULL){
                fprintf(stderr,"ERROR: %s: failed to allocate memory\n",caller);
                exit(-1);
        }

	newNode->name = (char*)malloc(sizeof(name));
	if(newNode->name == NULL){
                fprintf(stderr,"ERROR: %s: failed to allocate memory\n",caller);
                exit(-1);
        }	
	strcpy(newNode->name, name);

	newNode->tokenList = (struct token **)malloc(MAXTOKENS * sizeof(struct token*));
	if(newNode->tokenList == NULL){
		fprintf(stderr,"ERROR: %s: failed to allocate memory\n",caller);
                exit(-1);
	}
	
	int i;
	for(i = 0; i < MAXTOKENS; i++){
		newNode->tokenList[i] = (struct token *)malloc(sizeof(struct token));
		if(newNode->tokenList[i] == NULL){
                	fprintf(stderr,"ERROR: %s: failed to allocate memory\n",caller);
                	exit(-1);
        	}
	}	
	
	newNode->numToken = 0;	
	newNode->left = NULL;
	newNode->middle = NULL;
	newNode->right = NULL;
	
	ROOT[curr] = newNode;
	curr++;
	return newNode;
}


