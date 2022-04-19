#ifndef _HELPER_H
#define _HELPER_H

#define NUM_STATES 15
#define NUM_CHARS 10

int FSATable[NUM_STATES][NUM_CHARS] = 
{
        {S2,S3,S4,S6,S8,S9,S1,S11,EOFTK,NO_CHAR_EXIST},
        {S2,S2,IDTK,IDTK,IDTK,IDTK,IDTK,IDTK,IDTK,IDTK},
        {NUMBTK,S3,NUMBTK,NUMBTK,NUMBTK,NUMBTK,NUMBTK,NUMBTK,NUMBTK,NUMBTK},
        {NO_OPR_EXIST,NO_OPR_EXIST,NO_OPR_EXIST,S5,NO_OPR_EXIST,NO_OPR_EXIST,NO_OPR_EXIST,NO_OPR_EXIST,NO_OPR_EXIST,NO_OPR_EXIST},
        {OPRTK,OPRTK,OPRTK,OPRTK,OPRTK,OPRTK,OPRTK,OPRTK,OPRTK,OPRTK},
        {OPRTK,OPRTK,OPRTK,S7,OPRTK,OPRTK,OPRTK,OPRTK,OPRTK,OPRTK},
        {OPRTK,OPRTK,OPRTK,OPRTK,OPRTK,OPRTK,OPRTK,OPRTK,OPRTK,OPRTK},
        {OPRTK,OPRTK,OPRTK,OPRTK,OPRTK,OPRTK,OPRTK,OPRTK,OPRTK,OPRTK},
        {OPRTK,OPRTK,OPRTK,S10,OPRTK,OPRTK,OPRTK,OPRTK,OPRTK,OPRTK},
        {OPRTK,OPRTK,OPRTK,OPRTK,OPRTK,OPRTK,OPRTK,OPRTK,OPRTK,OPRTK},
        {NO_OPR_EXIST,NO_OPR_EXIST,NO_OPR_EXIST,NO_OPR_EXIST,NO_OPR_EXIST,NO_OPR_EXIST,NO_OPR_EXIST,S12,NO_OPR_EXIST,NO_OPR_EXIST},
	{S13,S13,S13,S13,S13,S13,S13,S14,UNTERMINATED_COMM,S13},
        {S13,S13,S13,S13,S13,S13,S13,S14,UNTERMINATED_COMM,S13},
        {S13,S13,S13,S13,S13,S13,S13,S15,UNTERMINATED_COMM,S13},
	{COMMTK,COMMTK,COMMTK,COMMTK,COMMTK,COMMTK,COMMTK,COMMTK,COMMTK,COMMTK}
};

char comp_oprs[] = {60,62,33,'\0'};
char single_oprs[] = {37,40,41,42,43,44,45,46,47,59,91,93,123,125,'\0'};
char* tokenNames[] = {"Identifier","Number","Keyword","Operator","EOF"};
#define keySize 17

char* keywords[] = {"start", "stop", "repeat","until","whole","label","quit","listen",
                    "yell","main","portal","if","then","pick","declare","assign","proc"};
char* errorString[] = {"Identifier can not start with uppercase letter.",
			"There is no such operator existed in the lexical definition.",
			"This character is not allowed in the lexical definition.",
			"Identifier and number token must not exceed 8 characters.",
			"Reach end of file, unterminated comment."};
#endif
