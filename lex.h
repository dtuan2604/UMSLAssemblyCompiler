#ifndef _LEX_H
#define _LEX_H

//lexical definition

#define MAX_LENGTH 8 //Assuming length for keyword and digit shouldn't be over 8

enum FSAIndex
{
        LETTER = 0,
        DIGIT,
        COMP_OPR, //operator with "=" only
        ASSIGN_OPR, //operator "="
        SINGLE_OPR, //operator without "=" only
        COLON_OPR, //operator ":"
        WHITESPACE,
        HASHTAG,
        EOFILE,
	NONEXIST
};

//state of the FSA table
enum State
{
        S1 = 0,S2,S3,S4,S5,S6,S7,
        S8,S9,S10,S11,S12,S13,S14,S15
};

enum finalState
{
	FINAL = 999,
        IDTK,
        NUMBTK,
        KEYWTK,
        OPRTK,
        EOFTK,
        COMMTK
};

enum ErrorState
{
	UNTERMINATED_COMM = -6,
	OVERBOUND,
	NO_CHAR_EXIST,
        NO_OPR_EXIST, 
        NO_ID_STARTWITH,
        ERROR
};


#endif
