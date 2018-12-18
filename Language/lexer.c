/* C interpreter for the L language		*/
/*		                				*/
/* (c) denis.zampunieris@uni.lu			*/
/*			                			*/
/* Enjoy!					            */
/* Please feel free to enhance it...	*/

// From Wikipedia, the free encyclopedia:
// In computer science, lexical analysis is the process of converting
// a sequence of characters into a sequence of tokens. A program or function
// which performs lexical analysis is called a lexical analyzer, lexer, or
// scanner. A lexer often exists as a single function which is called by a
// parser or another function.

/****************************************/
/* LEXICAL ANALYZER			*/
/****************************************/

char nextChar(forwardpointer)
boolean forwardpointer;		// indicates the need to move one char
{							// forward the reading pointer 'pinput'
char c;

	c = input[pinput];
	if (forwardpointer){
		pinput += 1;
	}
	if ((c == '\0') || (isalnum(c))
	|| (c == '+') || (c == '-') || (c == '*')
	|| (c == '/') || (c == '=') || (c == '<')
	|| (c == '>') || (c == '(') || (c == ')')){
		return c;
	}
	return ' ';
}

// words are delimited by blank or parenthese or the end of line
// ex: eval ( toto ) == eval (toto ) == eval ( toto) == eval (toto)
//     == eval(toto)

void nextWord()
{
char c;

	// skip previous blanks... as well as other irrelevant characters
	while ((c = nextChar(TRUE)) == ' '){
	}
	// start to store the next word: a list of continous chars 
	// terminated by a blank, a (, a ), a pre-defined operator
	// or the end of line
	pbuffer = 0;
	while (c != ' '){
		if (pbuffer < MAXBUFFER){
			buffer[pbuffer] = c;
			pbuffer += 1;
		}
		// end the word if it starts with the 3 following char's
		if ((c == '(') || (c == ')') || (c == '\0')){
			c = ' ';
		} else {
			// otherwise end the word if the next char is one
			// of the following symbols
			c = nextChar(FALSE);
			if ((c == '+') || (c == '-') || (c == '*')
			|| (c == '/') || (c == '=') || (c == '<')
			|| (c == '>') || (c == '(') || (c == ')')
			|| (c == '\0')){
				c = ' ';
			} else {
				c = nextChar(TRUE);
			}
		}
	}
	buffer[pbuffer] = '\0';
}

boolean buildIntegerFromBuffer(startindex)
int startindex;
{
char c;
long digit;

	nextvalue = 0;
	while (isdigit(c = buffer[startindex])){
		digit = c - '0';
		nextvalue = 10 * nextvalue + digit;
		startindex++;
	}
	if (c == '\0'){
		return TRUE;
	}
	return FALSE;
}

TSymbol nextSymbol()
{
	nextWord();	
	if (strcmp(buffer,"\0") == 0){
		error = 1;
		return sy_error;
	}
	if (strcmp(buffer,"(") == 0){
		return sy_openpar;
	}
	if (strcmp(buffer,")") == 0){
		return sy_closepar;
	}
	if (strcmp(buffer,"quit") == 0){
		return sy_quit;
	}
	if (strcmp(buffer,"help") == 0){
		return sy_help;
	}
	if (strcmp(buffer,"dump") == 0){
		return sy_dump;
	}
	if (strcmp(buffer,"list") == 0){
		return sy_list;
	}
	if (strcmp(buffer,"define") == 0){
		return sy_define;
	}
	if (strcmp(buffer,"eval") == 0){
		return sy_eval;
	}
	if (strcmp(buffer,"if") == 0){
		return sy_if;
	}
	if (strcmp(buffer,"while") == 0){
		return sy_while;
	}
	if (strcmp(buffer,"set") == 0){
		return sy_set;
	}
	if (strcmp(buffer,"begin") == 0){
		return sy_begin;
	}
	if (strcmp(buffer,"+") == 0){
		return sy_plus;
	}
	if (strcmp(buffer,"-") == 0){
		return sy_minus;
	}
	if (strcmp(buffer,"*") == 0){
		return sy_times;
	}
	if (strcmp(buffer,"/") == 0){
		return sy_div;
	}
	if (strcmp(buffer,"=") == 0){
		return sy_equal;
	}
	if (strcmp(buffer,"<") == 0){
		return sy_lower;
	}
	if (strcmp(buffer,">") == 0){
		return sy_greater;
	}
	if (isdigit(buffer[0])){
		if (buildIntegerFromBuffer(0)){
			return sy_value;
		}
		error = 20;
		return sy_error;
	}
	if (buffer[0] == '-'){
		if (buildIntegerFromBuffer(1)){
			nextvalue = - nextvalue;
			return sy_value;
		}
		error = 20;
		return sy_error;
	}
	return sy_ident; //is a sequence of chars, but we do not know. considered as a name.
}

/****************************************/
