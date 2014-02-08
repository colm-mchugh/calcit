#include "list.h"
extern List* parse_list;
extern int yyparse();

int main(int argc, char **argv)
{
	yyparse();
	return 0;
}
