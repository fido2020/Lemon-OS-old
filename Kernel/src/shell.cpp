#include <shell.h>

#include <common.h>

using namespace console;

Shell::Shell(){
	
	//clear();
	
	InitializeCommands();
	
	char* input;
	char* temp;
	
	bool exit = false;
	while(!exit){
		
		int i = 0;
		char* p;
		
		print("\n\n$ Lemon> ");
		
		input = read();
	}
}

void Shell::InitializeCommands(){

	/* ShellCommand structure:
		char* cmd;
		char* desc;
		void (*func)(char* args[]);
	*/
	
	ShellCommand colour;
	
	colour.cmd = "colour";
	colour.desc = "Changes the console colours.";
	colour.func = cmd_colour_func;
	
	commands[0] = colour;
}

void cmd_colour_func(char* args[]){
	print("test");
}