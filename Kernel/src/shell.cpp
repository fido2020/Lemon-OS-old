#include <shell.h>

#include <memory.h>
#include <keyboard.h>
#include <fatal.h>
#include <serial.h>
#include <string.h>
#include <math.h>

#include <windowmanager.h>
#include <initrd.h>
#include <video.h>
#include <bitmap.h>

char strbuf[512];

struct Variable {
	char* id;
	char* value;
};

int varcount = 0;
Variable* vars;
Variable* currentVar;
int varLength;
bool redirectOutput = false;

char* c_dir = "/dev/initrd";

Console* console;

// Shell Initializer
Shell::Shell(Console* con) {
	console = con;
	//strbuf = (char*)malloc(512);
	memset((uint8_t*)strbuf, 0, 512);
	console->puts("Lemon: ", 255, 255, 0);
	console->puts(c_dir, 0, 255, 64);
	console->puts("$ ");
	vars = (Variable*)malloc(sizeof(Variable)*5);
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
					if (strcmp(argv[0], shell_cmds[i].command) == 0) {
						shell_cmds[i].func(argc, argv);
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
					console->puts(" type 'cmds' for a list of commands");
				}
			}
			input_ptr = 0;
			memset((uint8_t*)strbuf, 0, 512);
			console->putc('\n');
			console->puts("Lemon: ", 255, 255, 0);
			console->puts(c_dir, 0, 255, 64);
			console->puts("$ ");
		}
		else if (is_caps() && key == '4') {
			strbuf[input_ptr++] = '$';
			console->putc(strbuf[input_ptr - 1]);
		}
		else if(key == '\b'){
			if(input_ptr > 0){
				strbuf[input_ptr--] = '\0';
				console->putc('\b');
			}
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
					break;
				}
				else currentVar = NULL;
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
		varcount++;
		vars = (Variable*)realloc(vars, varcount * sizeof(Variable));
		vars[varcount - 1].id = (char*)malloc(strlen(argv[1]));
		strcpy(vars[varcount - 1].id, argv[1]);
	}
	redirectOutput = true;
}

void sysinfo(int argc, char** argv){
	console->puts("\nLemon OS Alpha\n",255,255,0);
	console->puts	("\n\
	      ######     \n\
	   ############  \n\
	  ############## \n\
	#################\n\
	  ############## \n\
	   ############  \n\
	      ######     \n",255,255,0);
}

void randomtext(int argc, char** argv){
	console->putc('\n');
	for(int i = 0; i < 1000; i++){
		console->putc(rand() % 128, rand() % 255, rand() % 255, rand() % 255);
		screen_update();
	}
}

void ls(int argc, char** argv){
	lemoninitfs_node_t* nodes = initrd_list();
	for(int i = 0; i < initrd_get_header()->num_files; i++){
		console->puts("\n");
		console->puts(nodes[i].filename);
		for(int j = 0; j < 20-strlen(nodes[i].filename); j++){
			console->putc(' ');
		}
		console->puts("    ");
		console->puts(itoa(nodes[i].size/1024));
		console->puts("KB");
	}
}

void randint(int argc, char** argv){
	char* buf = (char*)malloc(12);
	console->putc('\n');
	console->puts(itoa(rand() % 100, buf,10));
	free(buf);
}

void randhex(int argc, char** argv){
	char* buf = (char*)malloc(12);
	console->puts("\n0x");
	console->puts(itoa(rand()*rand(), buf,16));
	free(buf);
}

void commands(int argc, char** argv){
	console->putc('\n');
	for(int i = 0; i < SHELL_COMMANDS; i++){
		console->puts(shell_cmds[i].command);
		console->putc('\n');
	}
}

uint32_t imgwidth;
uint32_t imgheight;
void* image_pixel_data_address;

void viewimage_render_callback(int x, int y){
	drawbitmap_noscale(x,y + 24,imgwidth,imgheight, (uint8_t*)image_pixel_data_address, 24);
}

void viewimage(int argc, char** argv){
	if(argc < 2){
		console->puts("\nUsage: ");
		console->puts(argv[0]);
		console->puts(" <file>");
		return;
	}

	lemoninitfs_header_t* initrd_header = initrd_get_header();
	lemoninitfs_node_t* initrd_nodes = initrd_list();

	char* file;
	int fid;

	for(int i = 0; i < initrd_header->num_files; i++){
		if(strcmp(initrd_nodes[i].filename, argv[1]) == 0){
			file = initrd_nodes[i].filename;
			fid = i;
			break;
		}
	}

	if(!fid){
		console->puts("File not found.");
		return;
	}

	bitmap_file_header_t* bitmap_fh = (bitmap_file_header_t*)initrd_read(fid);
	bitmap_info_header_t* bitmap_ih = (bitmap_info_header_t*)(initrd_read(fid) + sizeof(bitmap_file_header_t));

	imgwidth = bitmap_ih->width;
	imgheight = bitmap_ih->height;
	
	image_pixel_data_address = bitmap_fh + 54;

	Window* win = wm_getinstance()->Window_New(10,10,imgwidth,imgheight + 24,windowtype_framebuffer);

	win->render_callback = viewimage_render_callback;
	while(win->exists);
	free(win);
}