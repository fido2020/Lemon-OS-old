#pragma once

#define MAX_CMDS 256

void cmd_colour_func(char* args[]);

struct ShellCommand{
	char* cmd;
	char* desc;
	void (*func)(char* args[]);
};

class Shell{
	public:
		Shell();
	private:
		ShellCommand commands[MAX_CMDS];
		void InitializeCommands();
		
		void alias();
		
		//void cmd_colour_func(char* args[]);
};