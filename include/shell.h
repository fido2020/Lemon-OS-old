#pragma once

bool run;

struct ShellFunction{
	void (*func)(char* args[]);
};

extern void RunShell(){
	run = true;
	while(run){
		
	}
}