#include <shell.h>

#include <keyboard.h>
#include <stddef.h>
#include <memory.h>
#include <serial.h>
#include <fatal.h>
#include <string.h>

Shell::Shell(Console* con) {
	keyboard_install();
	console = con;
	console->clear(0, 0, 0);
	console->puts("Lemon OS\n");
}

// Parse a command
void Shell::ParseCommand(char* cmd, int length) {
	char** arguments;
	int argcount = 0;

	int argstart = 0;
	int argindex = 0;

	int len = 0;
	bool quotes = false;

	// Find number of and size of arguements
	for (int i = 0; i < length; i++) {
		if (cmd[i] == ' ' && !quotes && cmd[i - 1] != ' ')
					argcount++;
		else if (cmd[i] == '"')
			quotes = !quotes;
	}
	
	console->puts(itoa(argcount));
	arguments = (char**)malloc(argcount);
	len = 0;

	int i;
	for (i = 0; i < length; i++) {
		if (cmd[i] == ' ') {
			if (cmd[i - 1] != ' '){
				int j;
				char* arg = (char*)malloc(len + 1);
				for (j = 0; j < len; j++) {
					arg[j] = cmd[j + argstart];
				}
				arg[j + 1] = '\0';
				arguments[argindex] = arg;
				argindex++;
				argstart += len;
			}
			
			argstart++;
		}
		else {
			len++;
		}
	}

	/*if (cmd[i - 1] != ' ') {
		for (int j = 0; j < len; j++) {
			arguments[argindex][j] = cmd[j + argstart];
		}
		argindex++;
		argstart += len;
	}*/

	console->puts(arguments[0]);
	console->putc('|');

	ExecuteCommand(arguments, argcount);

	free(arguments);
}

void Shell::ExecuteCommand(char** arguments, int argc)
{
	if (arguments[0] == "crash") {
		fatal_error("User induced crash", "ERR_");
	}
}

char buffer[256];
int bufferindex;

void Shell::Update() {
	if (key_updated()) {
		char key = get_key();
		if (key == '\n') {
			ParseCommand(buffer, bufferindex);
			console->putc('\n');
			bufferindex = 0;
		}
		else {
			buffer[bufferindex] = key;
			bufferindex++;
			console->putc(key, 255, 255, 255);
		}
	}
}