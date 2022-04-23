#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "lex.h"
#include "node.h"
#include "token.h"
#include "parser.h"
#include "scanner.h"
#include "string.h"

extern char* fileName;
char* parser = "parser";
char nextChar;
struct token * nextTok;
FILE * fp;
int epsilon_flag = 0;

void printParserError(char* fmt, ...)
{
	char buf[PARABUFF];
	va_list args;

	va_start(args,fmt);
	vsprintf(buf,fmt,args);
	va_end(args);
	
	printRed();
	printf("%s:%d:%d: PARSER ERROR: ",fileName,nextTok->line,nextTok->charN);
	
	printYellow();
	printf(buf);
	
	fflush(stdout);
	printReset();

	free(nextTok);	
	exit(-1);
}

int isfileEmpty(FILE * fp, char* caller)
{
        if(fp != NULL)
        {
                fseek(fp, 0, SEEK_END);//set the pointer to the end of the file
                int test = ftell(fp);

                if(test == 0)
                {
                        fprintf(stderr, "ERROR: %s: Input is empty\n", caller);
                        exit(-1);
                }


        }
        else
        {
                fprintf(stderr, "ERROR: %s: Cannot open file '%s'\n", caller,fileName);
                exit(-1);
        }

        fseek(fp, 0, SEEK_SET); //reset the pointer back to the beginning of the file

        return 0;
}
void scanner()
{
	if(epsilon_flag == 0){
		nextTok = FSADriver(fp);
		while(nextTok->tokenID == COMMENT)
			nextTok = FSADriver(fp);
	}else
		epsilon_flag = 0;
	return;	
}
int matching(int Token, char* TokenIns)
{
	if(nextTok->tokenID == Token && (TokenIns == NULL || strcmp(nextTok->tokenIns,TokenIns) == 0))
			return 1;

	return 0;

}
struct node_t * runParser()
{
	struct node_t * tempRoot = NULL;
	fp = fopen(fileName, "r");
	if(isfileEmpty(fp,parser) == 1)
		return 0;
	nextChar = fgetc(fp);
	
	scanner();	
	tempRoot = program();	

	fclose(fp);
	free(nextTok->tokenIns);
	free(nextTok);
	return tempRoot; //the program run successfully
}
void copyToken(struct node_t ** newNode){
	int index = (*newNode)->numToken;
	struct token * tempToken = (*newNode)->tokenList[index];
	tempToken->tokenID = nextTok->tokenID;
	tempToken->tokenIns = (char*)malloc(sizeof(nextTok->tokenIns));
	if(tempToken->tokenIns == NULL){
		fprintf(stderr,"ERROR: %s: Cannot allocate memory for token instance\n", parser);
		exit(-1);
	}
	strcpy(tempToken->tokenIns, nextTok->tokenIns);
	tempToken->line = nextTok->line;
	tempToken->charN = nextTok->charN;
	(*newNode)->numToken++; 

}
struct node_t * program()
{
	struct node_t * tempNode = createNode("program");
	tempNode->left = vars();

	scanner();
	if(matching(KEYWORD,"main") == 0)
		printParserError("Expected 'main' token, but received '%s'\n",nextTok->tokenIns);
	copyToken(&tempNode);

	scanner();
	tempNode->right = block();	
	return tempNode;
}
struct node_t * block()
{
	struct node_t * tempNode = createNode("block");
	if(matching(OPERATOR,"{") == 0)
		printParserError("Expected a block of statements, but received '%s'\n",nextTok->tokenIns);
	copyToken(&tempNode);

	scanner();
	tempNode->left = vars();
	
	scanner();
	tempNode->right = stats();

	scanner();
	if(matching(OPERATOR,"}") == 0)
		printParserError("Reach the end of non-closing block, received '%s'\n",nextTok->tokenIns);
	copyToken(&tempNode);
	
	return tempNode;		

}
struct node_t * vars()
{
	struct node_t * tempNode = createNode("vars");
	//create a node and add all token to the node
	if(matching(KEYWORD,"declare") == 1){
		copyToken(&tempNode);	
	
		scanner();
		if(matching(IDENT, NULL) == 0)
			printParserError("Expected an Identifier token after 'declare' token\n");
		copyToken(&tempNode);

		scanner();
		if(matching(OPERATOR,":=") == 0)
			printParserError("Expected ':=', but received '%s'\n",nextTok->tokenIns);
		copyToken(&tempNode);
		
		scanner();
		if(matching(KEYWORD,"whole") == 0)
			printParserError("Expected data type 'whole', but received '%s'\n",nextTok->tokenIns);
		copyToken(&tempNode);
		
		scanner();
		if(matching(OPERATOR,";") == 0) 
                        printParserError("Expected ';' before '%s' token\n",nextTok->tokenIns);
		copyToken(&tempNode);
	
		scanner();
		tempNode->middle = vars();
						
	}else{
		epsilon_flag = 1;
	}
	return tempNode; //should return a node then
}
struct node_t * stats()
{
	struct node_t * tempNode = createNode("stats");
	tempNode->left = stat();

	scanner();
	tempNode->right = mStat();
	
	return tempNode;	

}
struct node_t * stat()
{
	struct node_t * tempNode = createNode("stat");
	int block_flag = 0;
	if(matching(KEYWORD, NULL) == 1 || matching(OPERATOR, "{") == 1){
		if(strcmp(nextTok->tokenIns,"listen") == 0)
			tempNode->middle = in();
		else if(strcmp(nextTok->tokenIns,"yell") == 0)
			tempNode->middle = out();
		else if(strcmp(nextTok->tokenIns,"if") == 0)
			tempNode->middle = if_();
                else if(strcmp(nextTok->tokenIns,"repeat") == 0)
			tempNode->middle = gen_loop();
                else if(strcmp(nextTok->tokenIns,"assign") == 0)
			tempNode->middle = assign();
                else if(strcmp(nextTok->tokenIns,"portal") == 0)
			tempNode->middle = goto_();
                else if(strcmp(nextTok->tokenIns,"label") == 0)
			tempNode->middle = label();
		else if(strcmp(nextTok->tokenIns,"declare") == 0)
			printParserError("Expecting a block of statement, or non-declare statement, but received '%s'\n",nextTok->tokenIns); 
                else if(strcmp(nextTok->tokenIns,"{") == 0){
			tempNode->middle = block();
			block_flag = 1;
		}
	}else
		printParserError("Expected a statement, but received '%s' token\n", nextTok->tokenIns);
	
	scanner();
	if(block_flag == 0){
		if(matching(OPERATOR,";") == 0)  
                        printParserError("Expected ';' before '%s' token\n",nextTok->tokenIns);	
		copyToken(&tempNode);
	}else
		epsilon_flag = 1; //assuming after <block> terminal is an epsilon character
	return tempNode;
}
struct node_t * mStat()
{
	struct node_t * tempNode = createNode("mStat");
	if(matching(KEYWORD,NULL) == 1 || matching(OPERATOR,"{") == 1){
		tempNode->left = stat();

		scanner();
		tempNode->right = mStat();
	}else{
		epsilon_flag = 1; // received an epsilon token
	}

	return tempNode;
}
struct node_t * in()
{
	struct node_t * tempNode = createNode("in");
	if(matching(KEYWORD,"listen") == 0)
		printParserError("Do you mean 'listen' ? (Received '%s' token)\n",nextTok->tokenIns);
	copyToken(&tempNode);

	scanner();
	if(matching(IDENT,NULL) == 0)
		printParserError("Expected an identifier, but received '%s' token\n",nextTok->tokenIns);
	copyToken(&tempNode);
	
	return tempNode; 
}
struct node_t * out()
{
	struct node_t * tempNode = createNode("out");
	if(matching(KEYWORD,"yell") == 0)
                printParserError("Do you mean 'yell' ? (Received '%s' token)\n",nextTok->tokenIns);
	copyToken(&tempNode);

        scanner();
	tempNode->middle = expr();
	
        return tempNode; 

}
struct node_t * condition() 
{
	struct node_t * tempNode = createNode("condition");
	if(matching(OPERATOR, "[") == 0)
        	printParserError("Expected '[' before '%s' token\n",nextTok->tokenIns);
	copyToken(&tempNode);

        scanner();
        tempNode->left = expr();

        scanner();
        tempNode->middle = RO();

        scanner();
        tempNode->right = expr();

        scanner();
        if(matching(OPERATOR, "]") == 0)
        	printParserError("Expected ']' before '%s' token\n",nextTok->tokenIns);
	copyToken(&tempNode);
	
	return tempNode;
}
struct node_t * if_()
{
	struct node_t * tempNode = createNode("if");
	if(matching(KEYWORD, "if") == 0)
		printParserError("Expected 'if' token for conditional statement but received '%s'\n",nextTok->tokenIns);
	copyToken(&tempNode);

	scanner();
	tempNode->left = condition();	
	
	scanner();
	if(matching(KEYWORD, "then") == 0)
                 printParserError("Expected 'then' token before '%s' token\n",nextTok->tokenIns);
	copyToken(&tempNode);

	scanner();
	tempNode->right = stat();

	return tempNode; 
}
struct node_t * gen_loop()
{
	struct node_t * tempNode = createNode("general loop");

	if(matching(KEYWORD, "repeat") == 0)
                 printParserError("Expected 'repeat' token for loop statement, but received '%s'\n",nextTok->tokenIns);
	copyToken(&tempNode);

	scanner();
	if(matching(OPERATOR,"[") == 1){
		tempNode->middle = loop1();
	}else if (matching(KEYWORD, NULL) == 1 || matching(OPERATOR, "{") == 1){
		tempNode->middle = loop2();
	}else{
		printParserError("Expected '[', '{' or keyword, but received '%s'\n",nextTok->tokenIns);
	}
	
	return tempNode;
}
struct node_t * loop1()
{
	struct node_t * tempNode = createNode("loop1");
	
	tempNode->left = condition();

	scanner();
	tempNode->right = stat();

	return tempNode;
}
struct node_t * loop2()
{
	struct node_t * tempNode = createNode("loop2");

	tempNode->left = stat();
	
	scanner();
	if(matching(KEYWORD, "until") == 0)
        	printParserError("Expected 'until' token, but received '%s'\n",nextTok->tokenIns);
	copyToken(&tempNode);

	scanner();
	tempNode->right = condition();
	
	return tempNode;
}
struct node_t * assign()
{
	struct node_t * tempNode = createNode("assign");

	if(matching(KEYWORD, "assign") == 0)
                 printParserError("Expected 'assign', but received '%s'\n",nextTok->tokenIns);
	copyToken(&tempNode);
	
	scanner();
	if(matching(IDENT, NULL) == 0)
                 printParserError("Expected an identifier, but received '%s'\n",nextTok->tokenIns);
	copyToken(&tempNode);

	scanner();
	if(matching(OPERATOR, "=") == 0)
		printParserError("Expected '=' token, but received '%s'\n",nextTok->tokenIns);
	copyToken(&tempNode);
	
	scanner();
	tempNode->middle = expr();
	
	return tempNode;
}
struct node_t * goto_()
{
	struct node_t * tempNode = createNode("goto");
	if(matching(KEYWORD, "portal") == 0)
                 printParserError("Expected 'portal' token, but received '%s'\n",nextTok->tokenIns);
	copyToken(&tempNode);

	scanner();
	if(matching(IDENT, NULL) == 0)
                 printParserError("Expected an identifier token, but received '%s'\n",nextTok->tokenIns);
	copyToken(&tempNode);

	return tempNode;
}
struct node_t * label()
{
	struct node_t * tempNode = createNode("label");

	if(matching(KEYWORD, "label") == 0)
                 printParserError("Expected 'label' token, but received '%s'\n",nextTok->tokenIns);
	copyToken(&tempNode);

        scanner();
        if(matching(IDENT, NULL) == 0)
                 printParserError("Expected an identifier token, but received '%s'\n",nextTok->tokenIns);
	copyToken(&tempNode);

        return tempNode;
}
struct node_t * RO()
{
	struct node_t * tempNode = createNode("RO");

	if(matching(OPERATOR,NULL) == 1){
		if(strcmp(nextTok->tokenIns,">=") == 0 || 
			strcmp(nextTok->tokenIns,"<=") == 0 ||
			strcmp(nextTok->tokenIns,"==") == 0 ||
			strcmp(nextTok->tokenIns,"!=") == 0)
			copyToken(&tempNode);
		else if(strcmp(nextTok->tokenIns,".") == 0){
			copyToken(&tempNode);

			scanner();
			if(matching(OPERATOR,".") == 0)
				printParserError("Do you mean '...' ?\n");
			copyToken(&tempNode);
			
			scanner();
                        if(matching(OPERATOR,".") == 0)
                                printParserError("Do you mean '...' ?\n");
			copyToken(&tempNode);
		}else
			printParserError("Expected comparision operator, but received '%s'\n",nextTok->tokenIns);

	}else
		printParserError("Expected comparision operator, but received '%s'\n",nextTok->tokenIns);

	return tempNode;
}
struct node_t * expr()
{
	struct node_t * tempNode = createNode("expr");

	tempNode->left = N();

	scanner();
	tempNode->right = expr_();
	
	return tempNode;
}
struct node_t * expr_()
{
	struct node_t * tempNode = createNode("expr'");

	if(matching(OPERATOR,"-") == 1){
                copyToken(&tempNode);

                scanner();
                tempNode->left = N();
		
		scanner();
		tempNode->right = expr_();
        }else{
                epsilon_flag = 1;
        }

        return tempNode;	
}
struct node_t * N()
{
	struct node_t * tempNode = createNode("N");

	tempNode->left = A();
	
	scanner();
	tempNode->right = N_();
	
	return tempNode;
}
struct node_t * N_()
{
	struct node_t * tempNode = createNode("N'");
	if(matching(OPERATOR, "+") == 1 || matching(OPERATOR, "/") == 1){
		copyToken(&tempNode);
	
		scanner();
		tempNode->left = A();
			
		scanner();
		tempNode->right = N_();
	}else{
		epsilon_flag = 1;
	}
		
	return tempNode;
}
struct node_t * A()
{
	struct node_t * tempNode = createNode("A");

	tempNode->left = M();

	scanner();
	if(matching(OPERATOR,"*") == 1){
		copyToken(&tempNode);

		scanner();
		tempNode->right = A();
	}else{
		epsilon_flag = 1;
	}
	
	return tempNode;
}
struct node_t * M()
{
	struct node_t * tempNode = createNode("M");

	if(matching(OPERATOR,"%") == 1){
		copyToken(&tempNode);
	
		scanner();
		tempNode->middle = M();
	}else{
		tempNode->middle = R();
	}
	
	return tempNode;
}
struct node_t * R()
{
	struct node_t * tempNode = createNode("R");

	if(matching(OPERATOR,"(") == 1){
		copyToken(&tempNode);

		scanner();
		tempNode->middle = expr();

		scanner();
		if(matching(OPERATOR,")") == 0)
			printParserError("Expected ')' before '%s' token\n",nextTok->tokenIns);
		copyToken(&tempNode);
	}else if(matching(IDENT, NULL) == 1 || matching(NUMBER, NULL) == 1){
		copyToken(&tempNode);
	}else
		printParserError("Exepcted an expression, or an identifier, or a number, but received '%s'\n",nextTok->tokenIns);

	return tempNode;
}
















