#pragma once

namespace console{
	
	void print(char data);
	void print(char* data);
	
	char* read();
	
	void clear();
	
	void panic(char* error, char* msg, bool hang);
}