#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "generator.h"
#include "tree.h"
#include "stack.h"

#define BUFF 200

char prog[BUFF];
char* outfile = NULL;
char* fileName = NULL;
FILE * outptr = NULL;

void clearMem(){
	fflush(stdout);
	destroyVarcount(); //this one must be called before destroyStack
	destroyStack();
	destroyROOT();
}
int main(int argc, char** argv)
{
	strcpy(prog,argv[0]);
	fileName = (char *)malloc(BUFF);
	outfile = (char *)malloc(BUFF);

	if(fileName == NULL)
	{
                fprintf(stderr,"ERROR: %s: failed to allocate memory for filename\n",prog);
               	return EXIT_FAILURE;
       	}		
	memset(fileName,'\0',BUFF);

	if(argc == 2)
	{
		strcpy(fileName, argv[1]);
		strcpy(outfile, argv[1]);
		strcat(outfile, ".asm");	

	}
	else if(argc < 2)
	{
		strcpy(fileName,"kb.output.buffer");
		strcpy(outfile, "kb.asm");
		
		FILE *ptr = fopen(fileName, "w");
		int input;	
	
		if(ptr == NULL){
			fprintf(stderr,"ERROR: %s: Cannot open file\"%s\"\n",prog,fileName);
			return EXIT_FAILURE;
		}

		if((input = getchar()) == EOF)
		{
			fprintf(stderr,"ERROR: %s: Input is empty\n",prog);
			return EXIT_FAILURE;
		}	
		else
		{
			fprintf(ptr,"%c", input);
		}
		
		while((input = getchar()) != EOF)
			fprintf(ptr,"%c",input);	
		
		fclose(ptr);	
	}
	else
	{
		fprintf(stderr,"ERROR: %s: Please provide one file as an argument to the program\n",prog);
		return EXIT_FAILURE;
	}
	
	initROOT();
	atexit(clearMem);
	
	struct node_t * root = runParser();
	
	createStack();
	initVarcount();

	outptr = fopen(outfile,"w");
	if(outptr == NULL){
		fprintf(stderr,"ERROR: %s: failed to open file '%s'\n",prog, outfile);
		return EXIT_FAILURE;
	}
	
	semanticCheck(root);
	
	
	printf("SUCCESSFULLY COMPILE THE PROGRAM! The target file is: %s\n",outfile);
	fclose(outptr);
	
	free(fileName);
        fileName = NULL;
        free(outfile);
        outfile = NULL;

	return EXIT_SUCCESS;		
}

