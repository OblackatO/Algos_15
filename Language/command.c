/* C interpreter for the L language		*/
/*		                				*/
/* (c) denis.zampunieris@uni.lu			*/
/*			                			*/
/* Enjoy!					            */
/* Please feel free to enhance it...	*/

/****************************************/
/* COMMAND PARSER AND EVALUATOR			*/
/****************************************/

boolean functionDefinition()
{
TSymbol s;
PFunDef f;
PFunDefList flist;

	if ((s = nextSymbol()) == sy_error){
		return FALSE;
	}
	if ((f = buildFunctionDefinition(s)) == NULL){
		return FALSE;
	}
	definitions = setFunctionDefinition(f,definitions);
	printf("\nfunction '%s(%i)' is defined\n",f->name,f->nbparameters);
	return TRUE;
}

boolean expressionEvaluation()
{
PExp e;
int res;
TSymbol s;

	if ((s = nextSymbol()) == sy_error){
		return FALSE;
	}
	if ((e = buildExpression(s)) == NULL){
		return FALSE;
	}
	res = evalExpression(e,memory);
	clearExpression(e);
	if (error != 0){
		return FALSE;
	}
	printf("\nresult is: %i\n",res);
	return TRUE;
}

boolean readEvalCommand()
{
	pinput = 0;
	error = 0;
	switch(nextSymbol())
	{
	//nextSymbol makes part of lexer.c
	//matches an expression, for instance EVAL
	case sy_quit: stop = TRUE; return TRUE;
	case sy_help: displayHelp(); return TRUE;
	case sy_dump: displayMemory(memory); return TRUE;
	case sy_list: displayDefinedFunctions(); return TRUE;
	case sy_define: return functionDefinition();
	case sy_eval: return expressionEvaluation();
	}
	error = 2;
	return FALSE;
}

void readEvalCommandLoop()
{
	stop = FALSE;
	while (!stop){
		//printf("\nPrinting new line, before L input");
		printf(PROMPT);
		if (fgets(input,MAXINPUT,stdin) == NULL){
			continue;
		}
		if (readEvalCommand() != TRUE){
			writeError();
		}
	}
}

/****************************************/
