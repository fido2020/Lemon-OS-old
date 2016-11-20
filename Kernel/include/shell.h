#pragma once

struct ShellFunction{
	void (*func)(char* args[]);
};

extern void RunShell();