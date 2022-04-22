#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "generator.h"
#include "stack.h"
#include "scanner.h"

int* varCount = NULL;
int curIndex;
char* gen = "generator";
char tempName[20];

extern char* fileName;
extern FILE * outptr;

int labelCount = 0; //counting unique label 
int tempvarCount = 0;

void newName(int type, char* returnName){
	if(type == VAR)
		sprintf(tempName,"T%d",tempvarCount++); /* generate a new label as T0, T1, etc */
  	else // creating new Label
    		sprintf(tempName,"L%d",labelCount++); /* new lables as L0, L1, etc */
	strcpy(returnName, tempName);
	return;
}

int isAbletoGencode(struct node_t *nodePtr){
	if(strcmp(nodePtr->name,"program") == 0 || strcmp(nodePtr->name,"block") == 0 ||
		strcmp(nodePtr->name,"stats") == 0 || strcmp(nodePtr->name,"stat") == 0 ||
		strcmp(nodePtr->name,"RO") == 0 || strcmp(nodePtr->name,"general loop") == 0)
		return 0;
	return 1;
	
}

void codeGen(struct node_t *nodePtr){
	char label[20], argR[20];
	if(nodePtr == NULL)
		return;
	
	if(strcmp(nodePtr->name,"expr") == 0){
		
	}
}

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
			fprintf(outptr,"PUSH\n");
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
		
		if(isAbletoGencode(nodePtr) == 1)
			codeGen(nodePtr);
		else{
                	semanticCheck(nodePtr->left);
                	semanticCheck(nodePtr->middle);
                	semanticCheck(nodePtr->right);
		}
		
		if(isBlock == 1){
			while(varCount[curIndex] > 0){
				pop();
                        	varCount[curIndex]--;
				fprintf(outptr,"POP\n");
                	}
			curIndex--;
		}
        }

}

