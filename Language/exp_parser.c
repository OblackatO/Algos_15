/* C interpreter for the L language		*/
/*		                				*/
/* (c) denis.zampunieris@uni.lu			*/
/*			                			*/
/* Enjoy!					            */
/* Please feel free to enhance it...	*/

// From Wikipedia, the free encyclopedia:
// In computing, a parser is one of the components in an interpreter
// or compiler that checks for correct syntax and builds a data
// structure (often some kind of parse tree, abstract syntax tree or
// other hierarchical structure) implicit in the input tokens.
// The parser often uses a separate lexical analyser to create tokens
// from the sequence of input characters. 

/************************************************/
/* EXPRESSIONS PARSER & SEMANTICS TREE MANAGER	*/
/************************************************/

// DESTRUCTION OF SEMANTICS TREE

void clearExpression();		// for mutual recursion

void clearOperands(opd)
POpdList opd;
{
	if (opd->nextopd != NULL){
		clearOperands(opd->nextopd);
	}
	if (opd->charopd != NULL){
		free(opd->charopd);
	}
	if (opd->expopd != NULL){
		clearExpression(opd->expopd);
	}
	free(opd);
}

void clearExpression(exp)
PExp exp;
{
	if (exp->operands != NULL){
		clearOperands(exp->operands);
	}
	free(exp);
}

// CREATION OF BASIC STRUCTURES

PExp newExp(expkind)
int expkind;
{
PExp e;

	e = (PExp) malloc(sizeof(struct TExp));
	e->kind = expkind;
	e->operands = NULL;
	return e;
}

POpdList newOpd()
{
POpdList o;

	o = (POpdList) malloc(sizeof(struct TOpdList));
	o->intopd = 0;
	o->charopd = NULL;
	o->expopd = NULL;
	o->nextopd = NULL;
	return o;
}

char *newIdentifierFromBuffer()
{
char *id;

	id = (char *) malloc(pbuffer);
	strcpy(id,buffer);
	return id;
}

// CREATION OF SEMANTICS TREES FOR EXPRESSIONS

PExp buildExpression();		// for mutual recursion

PExp buildValueExp()
{
PExp e;
POpdList o;

	e = newExp(EXP_VALUE);
	o = newOpd();
	o->intopd = nextvalue;
	e->operands = o;
	return e;
}

PExp buildVarExp()
{
PExp e;
POpdList o;

	e = newExp(EXP_VAR);
	o = newOpd();
	o->charopd = newIdentifierFromBuffer();
	e->operands = o;	
	return e;
}

PExp buildSetExp()
{
PExp e1,e2;
TSymbol s;
POpdList o1,o2;

	e1 = newExp(EXP_SET);
	if ((s = nextSymbol()) == sy_error){
		goto end_set;
	}
	if (s != sy_ident){
		error = 4;
		goto end_set;
	}
	o1 = newOpd();
	o1->charopd = newIdentifierFromBuffer();
	e1->operands = o1;	
	if ((e2 = buildExpression(nextSymbol())) == NULL){
		goto end_set;
	}
	o2 = newOpd();
	o2->expopd = e2;
	o1->nextopd = o2;
	if ((s = nextSymbol()) == sy_error){
		goto end_set;
	}
	if (s != sy_closepar){
		error = 7;
		goto end_set;
	}
	return e1;
end_set:
	clearExpression(e1);
	return NULL;
}

PExp buildBeginExp()
{
PExp e1,e2;
TSymbol s;
POpdList o1,o2;

	e1 = newExp(EXP_BEGIN);
	o1 = NULL;
	while (TRUE){
		if ((s = nextSymbol()) == sy_error){
			goto end_begin;
		}
		if (s == sy_closepar){
			return e1;
		}
		if ((e2 = buildExpression(s)) == NULL){
			goto end_begin;
		}
		o2 = newOpd();
		o2->expopd = e2;
		if (o1 == NULL){
			e1->operands = o2;
		} else {
			o1->nextopd = o2;
		}
		o1 = o2;
	}
end_begin:
	clearExpression(e1);
	return NULL;
}

PExp buildIfExp()
{
PExp e1,e2;
TSymbol s;
POpdList o1,o2;

	e1 = newExp(EXP_IF);
	if ((e2 = buildExpression(nextSymbol())) == NULL){
		goto end_if;
	}
	o1 = newOpd();
	o1->expopd = e2;
	e1->operands = o1;	
	if ((e2 = buildExpression(nextSymbol())) == NULL){
		goto end_if;
	}
	o2 = newOpd();
	o2->expopd = e2;
	o1->nextopd = o2;
	o1 = o2;
	if ((e2 = buildExpression(nextSymbol())) == NULL){
		goto end_if;
	}
	o2 = newOpd();
	o2->expopd = e2;
	o1->nextopd = o2;
	if ((s = nextSymbol()) == sy_error){
		goto end_if;
	}
	if (s != sy_closepar){
		error = 7;
		goto end_if;
	}
	return e1;
end_if:
	clearExpression(e1);
	return NULL;
}

PExp buildWhileExp()
{
PExp e1,e2;
TSymbol s;
POpdList o1,o2;

	e1 = newExp(EXP_WHILE);
	if ((e2 = buildExpression(nextSymbol())) == NULL){
		goto end_while;
	}
	o1 = newOpd();
	o1->expopd = e2;
	e1->operands = o1;	
	if ((e2 = buildExpression(nextSymbol())) == NULL){
		goto end_while;
	}
	o2 = newOpd();
	o2->expopd = e2;
	o1->nextopd = o2;
	if ((s = nextSymbol()) == sy_error){
		goto end_while;
	}
	if (s != sy_closepar){
		error = 7;
		goto end_while;
	}
	return e1;
end_while:
	clearExpression(e1);
	return NULL;
}

PExp buildOpCallExp(firstsymbol)
TSymbol firstsymbol;
{
PExp e1,e2;
TSymbol s;
POpdList o1,o2;

	switch (firstsymbol)
	{
	case sy_plus: e1 = newExp(EXP_OPPLUS); break;
	case sy_minus: e1 = newExp(EXP_OPMINUS); break;
	case sy_times: e1 = newExp(EXP_OPTIMES); break;
	case sy_div: e1 = newExp(EXP_OPDIV); break;
	case sy_equal: e1 = newExp(EXP_OPEQUAL); break;
	case sy_lower: e1 = newExp(EXP_OPLOWER); break;
	case sy_greater: e1 = newExp(EXP_OPGREATER); break;
	}
	if ((e2 = buildExpression(nextSymbol())) == NULL){
		goto end_opcall;
	}
	o1 = newOpd();
	o1->expopd = e2;
	e1->operands = o1;	
	if ((e2 = buildExpression(nextSymbol())) == NULL){
		goto end_opcall;
	}
	o2 = newOpd();
	o2->expopd = e2;
	o1->nextopd = o2;
	if ((s = nextSymbol()) == sy_error){
		goto end_opcall;
	}
	if (s != sy_closepar){
		error = 7;
		goto end_opcall;
	}
	return e1;
end_opcall:
	clearExpression(e1);
	return NULL;
}

PExp buildFunCallExp(firstSymbol)
TSymbol firstSymbol;
{
PExp e1,e2;
TSymbol s;
POpdList o1,o2;

	e1 = newExp(EXP_FUNCALL);
	if (firstSymbol == sy_error){
		goto end_funcall;
	}
	if (firstSymbol != sy_ident){
		error = 4;
		goto end_funcall;
	}
	o1 = newOpd();
	o1->charopd = newIdentifierFromBuffer();
	e1->operands = o1;
	o2 = newOpd();
	o1->nextopd = o2;
	o1 = o2;
	while (TRUE){
		if ((s = nextSymbol()) == sy_error){
			goto end_funcall;
		}
		if (s == sy_closepar){
			return e1;
		}
		if ((e2 = buildExpression(s)) == NULL){
			goto end_funcall;
		}
		o1->expopd = e2;
		o2 = newOpd();
		o1->nextopd = o2;
		o1 = o2;
	}
end_funcall:
	clearExpression(e1);
	return NULL;
}

PExp buildOperatorExp(firstSymbol)
TSymbol firstSymbol;
{
	if ((firstSymbol == sy_plus) || (firstSymbol == sy_minus)
	|| (firstSymbol == sy_times) || (firstSymbol == sy_div)
	|| (firstSymbol == sy_equal) || (firstSymbol == sy_lower)
	|| (firstSymbol == sy_greater)){
		return buildOpCallExp(firstSymbol);
	} else {
		return buildFunCallExp(firstSymbol);
	}
}

PExp buildExpression(firstSymbol)
TSymbol firstSymbol;
{
TSymbol s;

	switch (firstSymbol)
	{
	case sy_value: return buildValueExp();
	case sy_ident: return buildVarExp();
	case sy_openpar:
		if ((s = nextSymbol()) == sy_error){
			return NULL;
		}
		switch (s)
		{
		case sy_set: return buildSetExp();
		case sy_begin: return buildBeginExp();
		case sy_if: return buildIfExp();
		case sy_while: return buildWhileExp();
		default: return buildOperatorExp(s);
		}
	}
	error = 3;
	return NULL;
}

/****************************************/
