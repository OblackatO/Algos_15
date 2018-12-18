/* C interpreter for the L language		*/
/*		                				*/
/* (c) denis.zampunieris@uni.lu			*/
/*			                			*/
/* Enjoy!					            */
/* Please feel free to enhance it...	*/

/****************************************/
/* ENVIRONMENT MANAGER					*/
/****************************************/

// VARIABLES INSIDE A SINGLE ENVIRONMENT

// NOTE: as the 'name' parameter refers to some identifier from a
// semantics tree which will be destroyed after evaluation, there
// is need to copy this identifier when creating a new variable of
// that name into memory

PEnv bindVarEnv(name,val,env)
char *name;
int val;
PEnv env;
{
PEnv newnode;
PVar newvar;
char *varname;

	newvar = (PVar) malloc(sizeof(struct TVar));
	varname = (char *) malloc(strlen(name)+1);
	strcpy(varname,name);
	newvar->name = varname;
	newvar->value = val;
	newnode = (PEnv) malloc(sizeof(struct TEnv));
	newnode->var = newvar;
	newnode->next = env;
	return newnode;
}

PVar findVarEnv(name,env)
char *name;
PEnv env;
{
PEnv node;

	node = env;
	while (node != NULL){
		if (strcmp(node->var->name,name) == 0){
			return node->var;
		}
		node = node->next;
	}
	return NULL;	
}

int fetchVarEnv(name,env)
char *name;
PEnv env;
{
PVar var;

	var = findVarEnv(name,env);
	if (var != NULL){
		return var->value;
	}
	error = 30;
	return 0;
}

// VARIABLES INSIDE SOME MEMORY WITH CACHING MECHANISM:
// a linked and ordered list of environments - most prevalent is first

PVar findVar(name,mem)
char *name;
PEnvList mem;
{
PEnvList node;
PVar var;

	node = mem;
	while (node != NULL){
		var = findVarEnv(name,node->env);
		if (var != NULL){
			return var;
		}
		node = node->next;
	}
	return NULL;	
}

void assignVar(name,val,mem)
char *name;
int val;
PEnvList mem;
{
PVar var;

	var = findVar(name,mem);
	if (var != NULL){
		var->value = val;
	} else {
		mem->env = bindVarEnv(name,val,mem->env);
	}
}

int fetchVar(name,mem)
char *name;
PEnvList mem;
{
PVar var;
 
 	var = findVar(name,mem);
 	if (var != NULL){
 		return var->value;
 	}
 	error = 30;
 	return 0;
}

// CREATION AND DESTRUCTION OF ENVIRONMENTS

PEnvList addNewFirstEnvironment(envlist)
PEnvList envlist;
{
PEnvList newenv;

	newenv = (PEnvList) malloc(sizeof(struct TEnvList));
	newenv->env = NULL;
	newenv->next = envlist;
	return newenv;
}

void clearEnvironment(env)
PEnv env;
{
PEnv node;
PEnv nextnode;

	node = env;
	while (node != NULL){
		nextnode = node->next;
		free(node->var->name);
		free(node->var);
		free(node);
		node = nextnode;
	}
}

PEnvList removeFirstEnvironment(envlist)
PEnvList envlist;
{
PEnvList nextenv;

	nextenv = envlist->next;
	clearEnvironment(envlist->env);
	free(envlist);
	return nextenv;
}

/****************************************/
