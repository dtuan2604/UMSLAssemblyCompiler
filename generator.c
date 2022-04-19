#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "generator.h"
#include "stack.h"
#include "scanner.h"

int* varCount = NULL;
int curIndex;
char* gen = "generator";
extern char* fileName;

int isOverflow(){
	return curIndex >= MAX_BLOCKS ? 1 : 0;
}

void initVarcount(){
	varCount = (int*)malloc(MAX_BLOCKS * sizeof(int));
	if(varCount == NULL){
		fprintf(stderr,"ERROR: %s: Cannot allocate varCount memory\n", gen);
		exit(-1);
	}
	curIndex = 0;	
	varCount[curIndex] = 0;
		
	return;
}
void destroyVarcount(){
	int i;
	if(varCount == NULL)
		return;
	for(i = curIndex; i > -1; i--){
		while(varCount[curIndex] > 0){
			pop();
			varCount[curIndex]--;
		}
	}
	free(varCount);
	varCount = NULL;

}
void semanticCheck(struct node_t* nodePtr){
	if(nodePtr != NULL){
		int isBlock = 0;
		if(strcmp(nodePtr->name,"block") == 0){
			curIndex++; //advance the varCount to the next block
			
			if(isOverflow() == 1){
				printRed();
				printf("%s: ",fileName);
				printYellow();
				printf("There are too many nested blocks (over %d blocks)\n", MAX_BLOCKS);
				printReset();
				exit(-1);	
			}
			
			varCount[curIndex] = 0;
			isBlock = 1;
		}else if(strcmp(nodePtr->name,"vars") == 0 && nodePtr->numToken > 0){
			//checking if the variable is already defined
			int flag = find(nodePtr->tokenList[1]); 
			struct token* curToken = nodePtr->tokenList[1];
			if(flag > -1){
				struct token* predefToken = getToken(flag);
				printRed();
				printf("%s:%d:%d: SEMANTIC ERROR: ",fileName, curToken->line, curToken->charN);
				
				printYellow();
				printf("Multiple definitions for '%s' ",predefToken->tokenIns);
				printf("(previously defined at %s:%d:%d)\n", fileName, predefToken->line, predefToken->charN);
				printReset();
				free(predefToken);
				exit(-1);
			}
			
			push(curToken);
			varCount[curIndex]++;
		}else{
			//process non-vars node
			if(nodePtr->numToken > 0){
				int i = 0;
				struct token** tkList = nodePtr->tokenList;
				while(i < nodePtr->numToken){
					if(tkList[i]->tokenID == IDENT){
						int flag = find(tkList[i]);
						if(flag < 0){
							printRed();
                                			printf("%s:%d:%d: SEMANTIC ERROR: ",fileName, tkList[i]->line, tkList[i]->charN);

                                			printYellow();
							printf("'%s' has not been defined in this scope\n",tkList[i]->tokenIns);
							printReset();
							exit(-1);
						}

					}
					i++;
				}
			}	
		}

                semanticCheck(nodePtr->left);
                semanticCheck(nodePtr->middle);
                semanticCheck(nodePtr->right);
		
		if(isBlock == 1){
			while(varCount[curIndex] > 0){
				pop();
                        	varCount[curIndex]--;
                	}
			curIndex--;
		}
        }

}

