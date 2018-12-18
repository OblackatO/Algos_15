/* C interpreter for the L language		*/
/*		                				*/
/* (c) denis.zampunieris@uni.lu			*/
/*			                			*/
/* Enjoy!					            */
/* Please feel free to enhance it...	*/

#include "L-interpreter.h"	// constants and type definitions

/* Global variables */

boolean stop;			    // flag for the interpreter loop
char input[MAXINPUT];		// user's input at the prompt, to be evaluated
int pinput;			        // pointer to the current position in input
char buffer[MAXBUFFER + 1];	// next word or symbol (in lexer)
int pbuffer;			    // pointer to the current position in buffer
int nextvalue;			    // next value (in lexer)
int error;			        // current error number
PEnvList memory;		    // main memory (ordered list of environments)
PFunDefList definitions;	// list of known user-defined functions

/* Functions */

#include "tools.c"		    // widgets like help etc.
#include "lexer.c"		    // lexical analyzer
#include "exp_parser.c"		// expressions parser & semantics tree manager
#include "fun_parser.c"		// function definitions parser & manager
#include "environment.c"	// environments manager
#include "exp_eval.c"		// expressions evaluator
#include "command.c"		// commands parser and evaluator

/* Start point */

void main(argc, argv)
int argc;
char *argv[];
{
	puts("\nInterpreter for the L language - Welcome!");
	memory = addNewFirstEnvironment(NULL);
	definitions = NULL;
	readEvalCommandLoop();
	puts("\nBye!");
}

/****************************************/
