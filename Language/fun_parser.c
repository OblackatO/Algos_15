/* C interpreter for the L language		*/
/*		                				*/
/* (c) denis.zampunieris@uni.lu			*/
/*			                			*/
/* Enjoy!					            */
/* Please feel free to enhance it...	*/

/************************************************/
/* FUNCTION DEFINITIONS PARSER & MANAGER	*/
/************************************************/

// DESTRUCTION OF FUNCTION DEFINITIONS

void clearParameters(paramList)
PParamList paramList;
{
PParamList param;
PParamList nextparam;

	param = paramList;
	while (param != NULL){
		nextparam = param->next;
		free(param->name);
		free(param);
		param = nextparam;
	}
}

void clearFunDef(funDef)
PFunDef funDef;
{
	if (funDef->name != NULL){
		free(funDef->name);
	}
	if (funDef->parameters != NULL){
		clearParameters(funDef->parameters);
	}
	if (funDef->definition != NULL){
		clearExpression(funDef->definition);
	}
	free(funDef);
}

// CREATION OF FUNCTION DEFINITIONS

PFunDef newFunctionDefinition()
{
PFunDef fundef;

	fundef = (PFunDef) malloc(sizeof(struct TFunDef));
	fundef->name = NULL;
	fundef->nbparameters = 0;
	fundef->parameters = NULL;
	fundef->definition = NULL;
	return fundef;
}

PFunDefList setFunctionDefinition(func,funclist)
PFunDef func;
PFunDefList funclist;
{
PFunDefList newfunclist;

	newfunclist = (PFunDefList) malloc(sizeof(struct TFunDefList));
	newfunclist->fundef = func;
	newfunclist->next = funclist;
	return newfunclist;
}

PFunDef getFunctionDefinition(funcname,funclist)
char *funcname;
PFunDefList funclist;
{
PFunDefList node;

	node = funclist;
	while (node != NULL){
		if (strcmp(node->fundef->name,funcname) == 0){
			return node->fundef;
		}
		node = node->next;
	}
	return NULL;	
}

boolean findVarParametersList(varname, list)
char *varname;
PParamList list;
{
	PParamList var;

	var = list;
	while (var != NULL){
		if (strcmp(var->name,varname) == 0){
			return TRUE;
		}
		var = var->next;
	}
	return FALSE;	
}

PFunDef buildFunctionDefinition(firstSymbol)
TSymbol firstSymbol;
{
TSymbol s;
PFunDef fundef;
char *funcname;
char *paraname;
int nbparam;
PParamList param1,param2;
PExp def;

	fundef = newFunctionDefinition();
	if (firstSymbol != sy_ident){
		error = 4;
		goto end_fundef;
	}
	funcname = newIdentifierFromBuffer();
	if (getFunctionDefinition(funcname,definitions) != NULL){
		error = 21;
		goto end_fundef;
	}
	fundef->name = funcname;
	if ((s = nextSymbol()) == sy_error){
		goto end_fundef;
	}
	if (s != sy_openpar){
		error = 6;
		goto end_fundef;
	}
	nbparam = 0;
	while (TRUE){
		if ((s = nextSymbol()) == sy_error){
			goto end_fundef;
		}
		if (s != sy_ident){
			break;
		}
		nbparam++;
		param2 = (PParamList) malloc(sizeof(struct TParamList));
		paraname = newIdentifierFromBuffer();
		if (findVarParametersList(paraname,fundef->parameters)) {
			error = 22;
			goto end_fundef;
		}
		param2->name = paraname;
		param2->next = NULL;
		if (nbparam == 1){
			fundef->parameters = param2;
		} else {
			param1->next = param2;
		}
		param1 = param2;
	}
	fundef->nbparameters = nbparam;
	if (s != sy_closepar){
		error = 7;
		goto end_fundef;
	}
	if ((s = nextSymbol()) == sy_error){
		goto end_fundef;
	}
	def = buildExpression(s);
	if (def == NULL){
		goto end_fundef;
	}
	fundef->definition = def;
	return fundef;
end_fundef:
	clearFunDef(fundef);
	return NULL;
}

/****************************************/
