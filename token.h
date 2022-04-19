#ifndef _TOKEN_H
#define _TOKEN_H


//Token ID used to print to the output
enum tokenID
{
	IDENT = 0,
	NUMBER,
	KEYWORD,
	OPERATOR,
	EndOfFile,
	COMMENT
};

//A token will include token id, instance of that token under string value
//along with line number and the starting position of that token on the line
struct token
{
	int tokenID;
	char* tokenIns;
	int line;
	int charN;
};


#endif
