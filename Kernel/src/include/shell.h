#pragma once
#include <console.h>

#define SHELL_COMMANDS 11

void crash(int argc, char** argv);
void clear(int argc, char** argv);
void print(int argc, char** argv);
void input(int argc, char** argv);
void sysinfo(int argc, char** argv);
void randomtext(int argc, char** argv);
void ls(int argc, char** argv);
void randint(int argc, char** argv);
void randhex(int argc, char** argv);
void commands(int argc, char** argv);
void viewimage(int argc, char** argv);

struct ShellCommand {
	char* command;
	void (*func)(int argc, char** argv);
};

const static ShellCommand shell_cmds[SHELL_COMMANDS] = {
		{"crash",crash},
		{"clear",clear},
		{"print",print},
		{"input", input},
		{"sysinfo", sysinfo},
		{"randomtext", randomtext},
		{"ls", ls},
		{"randint", randint},
		{"randhex", randhex},
		{"cmds", commands},
		{"viewimage", viewimage},
	};

class Shell {
private:
	int input_ptr = 0;
	//Console* console;

	const int cmds_amount = SHELL_COMMANDS;
public:
	Shell(Console * con);
	void Update(bool key_updated, char key);
};