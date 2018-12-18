/* C interpreter for the L language		*/
/*		                				*/
/* (c) denis.zampunieris@uni.lu			*/
/*			                			*/
/* Enjoy!					            */
/* Please feel free to enhance it...	*/

/****************************************/
/* INTERFACE		            		*/
/****************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

/* Constants */

#define boolean		int
#define TRUE		1
#define FALSE		0

#define PROMPT      "\nL > "
#define MAXINPUT    1000	// maximum length of user's input
#define MAXBUFFER	50		// maximum length of a symbol

#define CMD_QUIT	0		// available commands
#define CMD_HELP	1
#define CMD_FUNDEF	2
#define CMD_EVAL	3

#define EXP_VALUE	0		// kinds of expression
#define EXP_VAR		1
#define EXP_SET		2
#define EXP_BEGIN	3
#define EXP_IF		4
#define EXP_WHILE	5
#define EXP_OPPLUS	6
#define EXP_OPMINUS	7
#define EXP_OPTIMES	8
#define EXP_OPDIV	9
#define EXP_OPEQUAL	10
#define EXP_OPLOWER	11
#define EXP_OPGREATER	12
#define EXP_FUNCALL	13

/* Types */

	// LIST OF LANGUAGE SYMBOLS (KEYWORDS)

typedef enum {
	sy_error, sy_quit, sy_help, sy_dump, sy_list, sy_define,
	sy_eval, sy_ident, sy_value, sy_openpar, sy_closepar,
	sy_set, sy_begin, sy_if, sy_while, sy_plus, sy_minus,
	sy_times, sy_div, sy_equal, sy_lower, sy_greater
	} TSymbol;

	// SEMANTICS TREE (EXPRESSION)

typedef struct TOpdList{		// list of operands (arguments)
	int intopd;				    // }
	char *charopd;				// } only 1 of these 3 fields is used
	struct TExp *expopd;		// }
 	struct TOpdList *nextopd;
 	} *POpdList;
 
typedef struct TExp{			// expression kind and operands
 	int kind;
 	POpdList operands;
 	} *PExp;

	// FUNCTION DEFINITIONS

typedef struct TParamList{		// list of (formal) parameters for function
	char *name;
	struct TParamList *next;
	} *PParamList;

typedef struct TFunDef{			// function name, parameters and definition
 	char *name;
	int nbparameters;
	PParamList parameters;
	int nblocal_vars;
	PParamList local_vars;
 	PExp definition;
 	} *PFunDef;
 
typedef struct TFunDefList{		// list of function definition nodes
	PFunDef fundef;
	struct TFunDefList *next;
	} *PFunDefList;
 
	// ENVIRONMENTS

typedef struct TVar{			// variable name and value
 	char *name;
 	int value;
 	} *PVar;
 
typedef struct TEnv{			// environment: list of <name,value> nodes
	PVar var;
	struct TEnv *next;
	} *PEnv;

typedef struct TEnvList{		// linked list of ordered environments
	PEnv env;			        // from the most-prevailing one to...
	struct TEnvList *next;		// ...the (final) global one
	} *PEnvList;

/****************************************/
