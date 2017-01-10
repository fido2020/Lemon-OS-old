#include <shell.h>
#include <conio.h>

using namespace console;

Shell::Shell(){
	
	clear();
	
	InitializeCommands();
	
	char* input;
	
	bool exit = false;
	while(!exit){
		
		print("\n\n$ Lemon> ");
		
		input = "";
		input = read();
		
		print(input);
		
		for(int i = 0; i < MAX_CMDS-1; i++){
			if(input == commands[i].cmd){
				print(commands[i].cmd);
				char* temp[] = {"10","edededed"};
				commands[i].func(temp);
				break;
			}
		}
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