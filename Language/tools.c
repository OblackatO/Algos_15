/* C interpreter for the L language		*/
/*		                				*/
/* (c) denis.zampunieris@uni.lu			*/
/*			                			*/
/* Enjoy!					            */
/* Please feel free to enhance it...	*/

/****************************************/
/* WIDGETS (help, memory dump, etc.)	*/
/****************************************/

void writeError()
{
	printf("\nerror: ");
	switch (error)
	{
	case 0: puts("Everything's fine !?!");
		break;

	case 1: puts("Unexpected end of line in input.");
		break;
	case 2: puts("Expected command.");
		break;
	case 3: puts("Expected expression.");
		break;
	case 4: puts("Expected identifier.");
		break;
	case 5: puts("Expected operator.");
		break;
	case 6: puts("Expected '('.");
		break;
	case 7: puts("Expected ')'.");
		break;

	case 20:puts("Not the right format for an integer value.");
		break;
	case 21:puts("A function with the same name is already defined.");
		break;
	case 22:puts("A parameter with the same name is already defined.");
		break;

	case 30:puts("Not the identifier of a variable in memory.");
		break;
	case 31:puts("Not the identifier of a defined function.");
		break;
	case 32:puts("Missing parameter in function call.");
		break;
	case 33:puts("Unknown arithmetic operator.");
		break;
	case 34:puts("Unknown comparison operator.");
		break;
	case 35:puts("Unknown expression.");
		break;

	default: puts("Unknown error?!");
	}
}

void displayHelp()
{
	puts("\n<command> ::=");
	puts("\t| QUIT");
	puts("\t| HELP");
	puts("\t| DUMP");
	puts("\t| LIST");
	puts("\t| DEFINE <function> ( var* ) <expression>");
	puts("\t| EVAL <expression>");
	puts("<function> ::= identifier");
	puts("<var> ::= identifier");
	puts("<expression> ::=");
	puts("\t| integer_value");
	puts("\t| var");
	puts("\t| ( SET <var> <expression> )");
	puts("\t| ( BEGIN <expression>* )");
	puts("\t| ( IF <expression> <expression> <expression> )");
	puts("\t| ( WHILE <expression> <expression> )");
	puts("\t| ( operator <expression>* )");
	puts("<operator> ::=");
	puts("\t| + | - | * | /");
	puts("\t| = | < | >");
	puts("\t| <function>");
}

void displayMemory(mem)
PEnvList mem;
{
PEnvList envlist;
PEnv env;
PVar var;

	envlist = mem;
	printf("\nmemory: ");
	while (envlist != NULL){
		env = envlist->env;
		while (env != NULL){
			var = env->var;
			printf("[%s,%i] ",var->name,var->value);
			env = env->next;
		}
		envlist = envlist->next;
	}
	puts("");
}

void displayDefinedFunctions()
{
PFunDefList fd;

	fd = definitions;
	printf("\nfunctions: ");
	while (fd != NULL){
		printf("'%s(%i)' ",fd->fundef->name,fd->fundef->nbparameters);
		fd = fd->next;
	}
	puts("");
}

/****************************************/
