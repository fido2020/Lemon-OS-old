#include <shell.h>

#include <memory.h>
#include <keyboard.h>
#include <fatal.h>
#include <serial.h>
#include <string.h>

char strbuf[512];

struct Variable {
	char* id;
	char* value;
};

int varcount;
Variable* vars;
Variable* currentVar;
int varLength;
bool redirectOutput;

Console* console;

// Shell Initializer
Shell::Shell(Console* con) {
	console = con;
	//strbuf = (char*)malloc(512);
	memset((uint8_t*)strbuf, 0, 512);
	console->puts("$ Lemon>");
}

struct Args {
	char** argv;
	int argc;
};

Args split_line(char* line)
{
	int bufsize = 64;
	int pos = 0;
	char** tokens = (char**)malloc(bufsize * sizeof(char*));
	char* token;

	token = strtok(line, " \t\r\n");
	while (token != NULL)
	{
		write_serial_string(token);
		tokens[pos] = token;
		pos++;

		if (pos >= bufsize) {
			bufsize += 64;
			tokens = (char**)realloc(tokens, bufsize * sizeof(char*));
		}

		token = strtok(NULL, " \r\n");
	}
	tokens[pos] = NULL;
	return { tokens,pos};
}

// Update Shell
void Shell::Update() {
	if (key_updated()) {
		char key = get_key();
		if (key == '\n') {
			strbuf[input_ptr] = '\0';
			if (redirectOutput) {
				currentVar->value = (char*)malloc(strlen(strbuf));
				strcpy(currentVar->value, strbuf);
				
				redirectOutput = false;
			}
			else {
				bool success;
				Args args = split_line(strbuf);
				char** argv = args.argv;
				int argc = args.argc;
				for (int i = 0; i < SHELL_COMMANDS; i++) {
					if (strcmp(argv[0], cmds[i].command) == 0) {
						cmds[i].func(argc, argv);
						success = true;
					}
				}
				for (int i = 0; i < argc; i++) {
					free(args.argv[i]);
				}
				free(args.argv);
				if (!success) {
					console->puts("\nUnknown Command: ");
					console->puts(argv[0]);
				}
			}
			input_ptr = 0;
			memset((uint8_t*)strbuf, 0, 512);
			console->putc('\n');
			console->puts("$ Lemon>");
		}
		else if (is_caps() && key == '4') {
			strbuf[input_ptr++] = '$';
			console->putc(strbuf[input_ptr - 1]);
		}
		else {
			strbuf[input_ptr++] = key;
			console->putc(strbuf[input_ptr - 1]);
		}
	}
}

void crash(int argc, char** argv) {
	fatal_error("You did this, you killed me", "ERR_ITS_ALL_YOUR_FAULT");
}

void clear(int argc, char** argv) {
	console->clear(0, 0, 0);
}

void print(int argc, char** argv) {
	console->putc('\n');

	for (int j = 0; j < varcount; j++) {
		console->puts("\nVar:");
		console->puts(vars[j].id);
	}

	for (int i = 1; i < argc; i++) {
		if(argv[i][0] != '$')
			console->puts(argv[i]);
		else {
			for (int j = 0; j < varcount; j++) {
				if (strcmp(argv[i], vars[j].id) == 0) {
					currentVar = &(vars[j]);
					//break;
				}
				else currentVar = NULL;
				console->puts("\nTested If Var:");
				console->puts(vars[j].id);
			}
			if (currentVar == NULL) {
				console->puts("Unknown variable: ");
				console->puts(argv[i]);
			}
			else {
				console->puts(currentVar->value);
			}
		}
	}
}

void input(int argc, char** argv) {
	if (argc <= 1 || argv[1][0] != '$') {
		console->puts("\nUsage: input $var");
		return;
	}
	if(varcount > 0)
		for (int i = 0; i < varcount; i++) {
			if (strcmp(argv[1],vars[i].id) == 0) {
				currentVar = &(vars[i]);
				break;
			}
			else currentVar = NULL;
		}
	if (currentVar == NULL) {
		console->puts("\nCreating variable: ");
		console->puts(argv[1]);
		vars = (Variable*)realloc(vars, ++varcount * sizeof(Variable));
		vars[varcount - 1].id = (char*)malloc(strlen(argv[1]));
		strcpy(vars[varcount - 1].id, argv[1]);
		console->puts(itoa(strlen(argv[1])));
	}
	redirectOutput = true;
}