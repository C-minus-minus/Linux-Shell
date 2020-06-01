//	Author 	 - Dylan Hartfield
//	Date	 - 10/11/2019
//	Function - This simply starts the shell

#include "shell.h"

int main(int argc, char *argv[]){

	//	create shell
	Shell shell = Shell(argc,argv);
	
	//	start shell
	shell.runShell();
}