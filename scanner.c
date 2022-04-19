#include <stdio.h>
#include "lex.h"
#include "helper.h"
#include "scanner.h"
#include <string.h>
#include <ctype.h>
#include "token.h"
#include <stdlib.h>

extern char* fileName;
extern char nextChar;
static int countChar = 1;
static int countLine = 1;

int new_line_flag = 0;
int whitespace_flag = 0; 
int comment_flag = 0;
char* FSAFile = "FSADriver";

void printRed()
{
	printf("\033[1;31m");
}

void printYellow()
{
	printf("\033[1;33m");
}

void printReset()
{
	printf("\033[0m");
}

void printScannerError(struct token * tok)
{
        int errorCode = ERROR - tok->tokenID - 1;
        printRed();
	printf("%s:%d:%d: SCANNER ERROR: ",fileName,tok->line,tok->charN);
        printYellow();
	printf("%s ",errorString[errorCode]);
        printf("(string: \'%s\')\n",tok->tokenIns);
        printReset();
	exit(-1);
}

int isKeyword(char* identifier)
{
        int i = 0;
        int res;
	if(identifier[0] >= 65 && identifier[0] <= 90)
		return -1; //Error no identifier start with uppercase letter
        while(i < keySize)
        {   
                res = strcmp(identifier, keywords[i]);
                if(res == 0)
                        return 1; //1 means the identifier is keyword
                else
                        i++;
        }   
    
        return 0; //0 means the identifier is not keyword

}
int isOperator(char character)
{
        int i = 0;
        int comp_opr_size = strlen(comp_oprs);
        int single_opr_size = strlen(single_oprs);
    
        while(i < comp_opr_size)
        {   
                if(character == comp_oprs[i])
                        return 2; //means that this is comparison operator
                else
                        i++;
        }   
    
        i = 0;
        while(i < single_opr_size)
        {   
                if(character == single_oprs[i])
                        return 1; //means that this is single operator
                else
                        i++;
        }   
    
        return 0; //means that this is not comparison or single operator
}


int charToFSAIndex(char character)
{
	if(((character >= 65) && (character <= 90)) || 
		((character >= 97) && (character <= 122)) ||
		character == 95)
		return LETTER;
	else if((character >= 48) && (character <= 57))
		return DIGIT;
	else if(isspace(character) != 0)
	{
		new_line_flag = (character == '\n') ? 1 : 0;
		whitespace_flag = 1;
		return WHITESPACE;
	}	
	else if(character == EOF)
		return EOFILE;
	else //operator
	{
		int test = isOperator(character);
		if(test == 2)
			return COMP_OPR;
		else if(test == 1)
			return SINGLE_OPR;
		else if(character == 61)
			return ASSIGN_OPR;
		else if(character == 58)
			return COLON_OPR;
		else if(character == 35)
			return HASHTAG;
	}
	return NONEXIST; //No character matches with lexical definition

}

struct token * FSADriver(FILE* fp)
{
	int startChar = countChar;

	struct token* tok = (struct token*)malloc(sizeof(struct token));
	if(tok == NULL)
	{
                printf("ERROR: %s: failed to allocate memory\n",FSAFile);
                return NULL;
        } 
	tok->tokenIns = (char*)malloc(MAX_LENGTH + 1);
	if(tok->tokenIns == NULL)
        {   
                printf("ERROR: %s: failed to allocate memory\n",FSAFile);
                return NULL;
        }
	memset(tok->tokenIns,'\0',MAX_LENGTH + 1);   
	tok->line = -1;
	tok->charN = -1;	
	
	int state = S1;
	int nextState;
	int index = charToFSAIndex(nextChar);
	char* stringIns = (char*)malloc(MAX_LENGTH + 1);
	if(stringIns == NULL)
        {
                printf("ERROR: %s: failed to allocate memory\n",FSAFile);
                return NULL;
        }
	memset(stringIns,'\0',MAX_LENGTH + 1);

	while(state < FINAL)
	{
		if(countChar - startChar > MAX_LENGTH && comment_flag == 0){
			tok->tokenID = OVERBOUND;
                        strcpy(tok->tokenIns, stringIns);
                        tok->line = countLine;
                        tok->charN = startChar;
			printScannerError(tok);
		}

		nextState = FSATable[state][index];
		
		if(nextState < ERROR || nextState > FINAL)
		{
			tok->tokenID = nextState < ERROR ? nextState : nextState - (FINAL + 1);
			if(stringIns[0] == '\0')
				strncat(tok->tokenIns, &nextChar, 1);
			else
				strcpy(tok->tokenIns, stringIns);
			tok->line = tok->line == -1 ? countLine : tok->line; //the line might be already set if the token was the comment token
			tok->charN = tok->charN == -1 ? startChar : tok->charN; 
			
			if(tok->tokenID == IDENT)
			{
				int test = isKeyword(tok->tokenIns);
				switch(test){
					case 1: tok->tokenID = KEYWORD;
						break;
					case -1: tok->tokenID = NO_ID_STARTWITH; 
						break;
				}
			}else if(tok->tokenID == COMMENT){
				comment_flag = 0;
			}else if(tok->tokenID < ERROR){
				printScannerError(tok);
			}
					
			break;			
		}else{
			if(nextState == S12){
				comment_flag = 1;
				strncat(stringIns, &nextChar, 1);
				tok->line = countLine;
				tok->charN = startChar;
			}
			if(index == WHITESPACE)
                        {
                                if(new_line_flag == 1){
					countLine++;
                                	countChar = 0;
					startChar = 0;
				}else
					startChar = countChar;
                        }
			if(index != WHITESPACE && comment_flag == 0)
				strncat(stringIns, &nextChar, 1);
			state = nextState;
			nextChar = fgetc(fp);
			countChar++;
			index = charToFSAIndex(nextChar);
			if(index != WHITESPACE && whitespace_flag == 1){
				//skip whitespace and start counting the characters
				whitespace_flag = 0;
				startChar = countChar;
			}
		}	
				

	}
	free(stringIns);
	return tok;
}
