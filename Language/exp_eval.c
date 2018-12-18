/* C interpreter for the L language		*/
/*		                				*/
/* (c) denis.zampunieris@uni.lu			*/
/*			                			*/
/* Enjoy!					            */
/* Please feel free to enhance it...	*/

/****************************************/
/* EXPRESSIONS EVALUATOR		*/
/****************************************/

int evalExpression();		// for mutual recursion

int evalValueExp(exp)
PExp exp;
{
POpdList opd;

	opd = exp->operands;
	return opd->intopd;
}

int evalVarExp(exp,mem)
PExp exp;
PEnvList mem;
{
POpdList opd;
char *varname;

	opd = exp->operands;
	varname = opd->charopd;
	return fetchVar(varname,mem);
}

int evalSetExp(exp,mem)
PExp exp;
PEnvList mem;
{
POpdList opd;
char *varname;
int varvalue;

	opd = exp->operands;
	varname = opd->charopd;
	opd = opd->nextopd;
	varvalue = evalExpression(opd->expopd,mem);
	if (error == 0){
		assignVar(varname,varvalue,mem);
	}
	return varvalue;
}

int evalBeginExp(exp,mem)
PExp exp;
PEnvList mem;
{
POpdList opd;
int res;

	opd = exp->operands;
	res = 0;
	while (opd != NULL){
		res = evalExpression(opd->expopd,mem);
		if (error == 0){
			opd = opd->nextopd;
		} else {
			opd = NULL;
		}
	}
	return res;
}

int evalIfExp(exp,mem)
PExp exp;
PEnvList mem;
{
POpdList opd;
int res;

	opd = exp->operands;
	res = evalExpression(opd->expopd,mem);
	if (error != 0){
		return 0;
	}
	if (res !=0){
		opd = opd->nextopd;
	} else {
		opd = opd->nextopd->nextopd;
	}
	res = evalExpression(opd->expopd,mem);
	if (error != 0){
		return 0;
	}
	return res;
}

int evalWhileExp(exp,mem)
PExp exp;
PEnvList mem;
{
PExp condition,iteration;
int condres,iterres;

	condition = exp->operands->expopd;
	iteration = exp->operands->nextopd->expopd;
	condres = 1;
	while (condres != 0){
		condres = evalExpression(condition,mem);
		if (error != 0){
			condres = 0;
		} else if (condres !=0){
			iterres = evalExpression(iteration,mem);
			if (error != 0){
				condres = 0;
			}
		}
	}
	return 0;
}

int evalArithmeticOpCallExp(exp,mem)
PExp exp;
PEnvList mem;
{
POpdList opd;
int v1,v2;

	opd = exp->operands;
	v1 = evalExpression(opd->expopd,mem);
	if (error != 0){
		return 0;
	}
	opd = opd->nextopd;
	v2 = evalExpression(opd->expopd,mem);
	if (error != 0){
		return 0;
	}
	switch (exp->kind)
	{
	case EXP_OPPLUS: return v1 + v2;
	case EXP_OPMINUS: return v1 - v2;
	case EXP_OPTIMES: return v1 * v2;
	case EXP_OPDIV: return v1 / v2;
	}
	error = 33;
	return 0;
}

int evalComparisonOpCallExp(exp,mem)
PExp exp;
PEnvList mem;
{
POpdList opd;
int v1,v2;


	opd = exp->operands;
	v1 = evalExpression(opd->expopd,mem);
	if (error != 0){
		return 0;
	}
	opd = opd->nextopd;
	v2 = evalExpression(opd->expopd,mem);
	if (error != 0){
		return 0;
	}
	switch (exp->kind)
	{
	case EXP_OPEQUAL: if (v1 == v2){return 1;} else {return 0;}
	case EXP_OPLOWER: if (v1 < v2){return 1;} else {return 0;}
	case EXP_OPGREATER: if (v1 > v2){return 1;} else {return 0;}
	}
	error = 34;
	return 0;
}

int evalFunCallExp(exp,mem)
PExp exp;
PEnvList mem;
{
int res;
char *name;
PFunDef fundef;
PEnvList oldmem;
int np;
POpdList opd;
PParamList param;
int arg;
PVar v;

	res = 0;
	opd = exp->operands;
	name = opd->charopd;
	if ((fundef = getFunctionDefinition(name,definitions)) == NULL){
		error = 31;
		return res;
	}
	oldmem = mem;
	mem = addNewFirstEnvironment(oldmem);
	np = 1;
	opd = opd->nextopd;
	param = fundef->parameters;
	while ((np <= fundef->nbparameters) && (error == 0)){
		if (opd == NULL){
			error = 32;
			continue;
		}
		arg = evalExpression(opd->expopd,oldmem);
		mem->env = bindVarEnv(param->name,arg,mem->env);
		np++;
		opd = opd->nextopd;
		param = param->next;
	}
	if (error == 0){
		res = evalExpression(fundef->definition,mem);
	}
	mem = removeFirstEnvironment(mem);
	return res;
}

int evalExpression(exp,mem)
PExp exp;
PEnvList mem;
{
	switch (exp->kind)
	{
	case EXP_VALUE: return evalValueExp(exp);
	case EXP_VAR: return evalVarExp(exp,mem);
	case EXP_SET: return evalSetExp(exp,mem);
	case EXP_BEGIN: return evalBeginExp(exp,mem);
	case EXP_IF: return evalIfExp(exp,mem);
	case EXP_WHILE: return evalWhileExp(exp,mem);
	case EXP_OPPLUS:
	case EXP_OPMINUS:
	case EXP_OPTIMES:
	case EXP_OPDIV: return evalArithmeticOpCallExp(exp,mem);
	case EXP_OPEQUAL:
	case EXP_OPLOWER:
	case EXP_OPGREATER: return evalComparisonOpCallExp(exp,mem);
	case EXP_FUNCALL: return evalFunCallExp(exp,mem);
	}
	error = 35;
	return 0;
}

/****************************************/
