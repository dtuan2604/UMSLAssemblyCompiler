#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "string.h"

struct stack* varStack = NULL;
char* file = "stack";
void createStack(){
	varStack = (struct stack *)malloc(sizeof(struct stack));
	if(varStack == NULL){
		fprintf(stderr,"ERROR: %s: failed to allocate Stack memory\n",file);
		exit(-1);
	}
	
	varStack->size = 0;
	varStack->body = (struct token **)malloc(MAX_VARS * sizeof(struct token*));
	if(varStack->body == NULL){
                fprintf(stderr,"ERROR: %s: failed to allocate Stack memory\n",file);
                exit(-1);
        }
	return;
}

void destroyStack(){
	if(varStack == NULL)
		return;
	int i;
	int size = varStack->size;
	for(i = 0; i < size; i++){
		free(varStack->body[i]);
		varStack->body[i] = NULL;
	}

	free(varStack->body);
	varStack->body = NULL;
	
	free(varStack);
	varStack = NULL;
	return;

}
int isEmpty(){
	if(varStack == NULL){
		fprintf(stderr,"ERROR: %s: Cannot access Stack memory since it is null\n",file);
		exit(-1);
	}
	return varStack->size == 0 ? 1 : 0; 
}
int isFull(){
	return varStack->size >= MAX_VARS ? 1 : 0;
}
void push(struct token* newToken){
	if(isFull() == 1){
		fprintf(stderr,"ERROR: %s: Stack overflow, there are already more than 100 variables on stack\n",file);
		fflush(stdout);
		exit(-1);
	}	
	
	struct token* tempTok = (struct token*)malloc(sizeof(newToken));
	if(tempTok == NULL){
		fprintf(stderr,"ERROR: %s: Cannot allocate memory for stack\n",file);
		exit(-1);
	}
	tempTok->tokenID = newToken->tokenID;
	tempTok->tokenIns = (char*)malloc(sizeof(newToken->tokenIns));
	if(tempTok->tokenIns == NULL){
                fprintf(stderr,"ERROR: %s: Cannot allocate memory for token instance\n", file);
                exit(-1);
        }
	strcpy(tempTok->tokenIns, newToken->tokenIns);
	tempTok->line = newToken->line;
	tempTok->charN = newToken->charN;

	varStack->body[varStack->size] = tempTok;
	varStack->size++;
	return;
}
void pop(){
	if(isEmpty() == 1){
		fprintf(stderr,"ERROR: %s: There is nothing in the stack to be pop\n",file);
		exit(-1);
	}
	
	int size = --(varStack->size);
	struct token* oldToken = varStack->body[size];
	varStack->body[size] = NULL;
	free(oldToken);

	return;
	
}
struct token* getToken(int index){
	int arrayID = varStack->size - index - 1;
	struct token* targetTok = varStack->body[arrayID];
	struct token* tempTok = (struct token*)malloc(sizeof(struct token));
	if(tempTok == NULL){
                fprintf(stderr,"ERROR: %s: Cannot allocate memory for stack\n",file);
        	exit(-1);
	}
        tempTok->tokenID = targetTok->tokenID;
        tempTok->tokenIns = (char*)malloc(sizeof(targetTok->tokenIns));
        if(tempTok->tokenIns == NULL){
                fprintf(stderr,"ERROR: %s: Cannot allocate memory for token instance\n", file);
                exit(-1);
        }
	strcpy(tempTok->tokenIns, targetTok->tokenIns);
	tempTok->line = targetTok->line;
        tempTok->charN = targetTok->charN;

	return tempTok;	

}
int find(struct token* targetTok){
	if(isEmpty() == 1){
        	return -1;
	}

	int index  = varStack->size - 1;
	while(index >= 0){
		struct token* temp = varStack->body[index];
		if(temp->tokenID == targetTok->tokenID &&
		strcmp(temp->tokenIns, targetTok->tokenIns) == 0)
			break;
		index--;
	}
	return index == -1 ? -1 : varStack->size - 1 - index;
}





















