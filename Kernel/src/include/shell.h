#pragma once

#include <console.h>

class Shell {
private:
	char* commands;
	char* currentdirectory;
	char** variables;

	Console* console;

	void ParseCommand(char* cmd, int length);
	void ExecuteCommand(char** arguments, int argc);
public:
	Shell(Console* con);
	void Update();
};