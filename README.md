# Project 4: The Compiler
**Name**: ::tabs Duong Tuan Hoang<br>
**Section**: 	CMPSCI 4280 - Program Translation Project<br>
**VCS**:		git<br>
**Date**:		04/25/2022<br>
**Project Option**:	Local Scope<br> 

## Code Structure	
**main.c** - the main program to process the input<br>
**parser.c, parser.h** - the parser of the program<br>
**node.h** - header file defining the nodes of the tree<br>
**tree.c, tree.h** - file containing functions to build and destroy the parse tree<br>
**token.h** - header file defining tokenID and token data type<br>
**lex.h** - header file defining basic lexical definition<br>
**helper.h** - header file containing FSA table and references to operators, keywords, token names and errors<br>
**scanner.h, scanner.c** - the scanner program<br>
**stack.c, stack.h** - files defining the variable stack for the program<br>
**generator.c, generator.h** - file containing the function to do semantical checking and generate VM code<br>
**Makefile** 	

## Run the program
```bash
# Compile code
make

# To give the program keyboard input (remember to Enter and then Ctrl + D to finish the input)
./compfs

# To direct the input from a file into program
./compfs \< filename

# To let the program read input from a file
./compfs filename

# Remove all executable and object files
make clean
```
## Description
- Based on the lexical definition provided, the scanner will parse through the file and export 4 types of token **identifier**, **number**, **operator**, and **EOF**.
- The scanner will automatically skip *whitespace* and *comments* surrounded by **##**.
- This scanner can determine **5 errors** which is specified in the lex.h. If the program encounter error, the error is reported to the screen and then the program is terminated.
- The parser will call scanner every time it needs a token 
- The parser will parse the program based on the BNF grammar provided. There are some modifications made to ensure **LL(1)** and the associativity in the expression. **Modification**: 
	- \<**expr**\> -\> \<**N**\>\<**expr'**\>
	- \<**expr'**\> -\> -\<**N**\>\<**expr'**\> | **empty**
	- \<**stat**\> -\> .... | \<**general loop**\>;|... (instead of loop1 and loop2)
	- \<**general loop**\> -\> repeat (\<**loop1**\> | \<**loop2**\>)
	- The fragment [\<**expr**\>\<**RO**\>\<**expr**\>] is turned into \<**condition**\> for short
	- \<**N**\> -\> \<**A**\>\<**N'**\>
	- \<**N'**\>-\> +\<**A**\>\<**N'**\> | /\<**A**\>\<**N'**\> | **empty**
 
- The parser will give the specific error if it found out that the current token doesn't not match with the expected token	
- After that, main program will call function to check semantically. The program will parse through the tree built from the input file preorder-ly. 
- The scope rule is implemented the same as in C. 
	- A variable must be defined before using. 
	- A variable can not be defined twice (in the same scope, or any of its enclosing scope, or global scope)
- This compiler processes assuming that there are no more than 200 nested blocks in the program and there are no more than 100 variables existed at the same time on stack. Otherwise, the program will terminate and report error.
- At the termination, the program will pop all the variables left in the stack
- If the program follow the semantic rule, at the end of the traverse, the code for the VM will be generated in **file.asm**, while *file* is the name of the file (*'kb'* if the input is from keyboard)

## Problem
- Regarding to the parser implementation, when the parser encounters "%" token and it is not what the parser expects, it cannot print "%" error to the terminal, but "20051102"
