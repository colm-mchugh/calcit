#include "shell.h"

int main(int argc, char **argv)
{
	int return_code = run_shell(stdin, stdout, stderr);
	return return_code;
}
