#pragma once
#include <console.h>

#define SHELL_COMMANDS 4

void crash(int argc, char** argv);
void clear(int argc, char** argv);
void print(int argc, char** argv);
void input(int argc, char** argv);

struct ShellCommand {
	char* command;
	void (*func)(int argc, char** argv);
};

class Shell {
private:
	int input_ptr = 0;
	//Console* console;

	const int cmds_amount = 4;
	const ShellCommand cmds[4] = {
		{"crash",crash},
		{"clear",clear},
		{"print",print},
		{"input", input}
	};
public:
	Shell(Console * con);
	void Update();
};