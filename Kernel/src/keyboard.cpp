#include <conio.h>
#include <system.h>
#include <stdtype.h>

using namespace console;

namespace console{
	char* read()
	{
		bool upperCase = false;
		char buff;
		char* buffstr;
		int i = 0;
		bool reading = true;
		inportb(0x64);
		while(reading)
		{
			if(inportb(0x64) & 0x1)                 
			{
				switch(inportb(0x60))
				{ 
		  /*case 1:
					print('(char)27);           Escape button
					buffstr[i] = (char)27;
					i++;
					break;*/
			case 2:
					print('1');
					buffstr[i] = '1';
					i++;
					break;
			case 3:
					print('2');
					buffstr[i] = '2';
					i++;
					break;
			case 4:
					print('3');
					buffstr[i] = '3';
					i++;
					break;
			case 5:
					print('4');
					buffstr[i] = '4';
					i++;
					break;
			case 6:
					print('5');
					buffstr[i] = '5';
					i++;
					break;
			case 7:
					print('6');
					buffstr[i] = '6';
					i++;
					break;
			case 8:
					print('7');
					buffstr[i] = '7';
					i++;
					break;
			case 9:
					print('8');
					buffstr[i] = '8';
					i++;
					break;
			case 10:
					print('9');
					buffstr[i] = '9';
					i++;
					break;
			case 11:
					print('0');
					buffstr[i] = '0';
					i++;
					break;
			case 12:
					print('-');
					buffstr[i] = '-';
					i++;
					break;
			case 13:
					print('=');
					buffstr[i] = '=';
					i++;
					break;
			case 14:
					if(i > 0){
						print('\b'); // Backspace
						i--;
						buffstr[i] = 0;
					}
					break;
		   /* case 15:
					print('\t');          Tab button
					buffstr[i] = '\t';
					i++;
					break;*/
			case 16:
					print('q');
					buffstr[i] = 'q';
					i++;
					break;
			case 17:
					print('w');
					buffstr[i] = 'w';
					i++;
					break;
			case 18:
					print('e');
					buffstr[i] = 'e';
					i++;
					break;
			case 19:
					print('r');
					buffstr[i] = 'r';
					i++;
					break;
			case 20:
					print('t');
					buffstr[i] = 't';
					i++;
					break;
			case 21:
					print('y');
					buffstr[i] = 'y';
					i++;
					break;
			case 22:
					print('u');
					buffstr[i] = 'u';
					i++;
					break;
			case 23:
					print('i');
					buffstr[i] = 'i';
					i++;
					break;
			case 24:
					print('o');
					buffstr[i] = 'o';
					i++;
					break;
			case 25:
					print('p');
					buffstr[i] = 'p';
					i++;
					break;
			case 26:
					print('[');
					buffstr[i] = '[';
					i++;
					break;
			case 27:
					print(']');
					buffstr[i] = ']';
					i++;
					break;
			case 28:
				   // print('\n');
				   // buffstr[i] = '\n';
					  i++;
				   reading = 0;
					break;
		  /*  case 29:
					print('q');           Left Control
					buffstr[i] = 'q';
					i++;
					break;*/
			case 30:
					print('a');
					buffstr[i] = 'a';
					i++;
					break;
			case 31:
					print('s');
					buffstr[i] = 's';
					i++;
					break;
			case 32:
					print('d');
					buffstr[i] = 'd';
					i++;
					break;
			case 33:
					print('f');
					buffstr[i] = 'f';
					i++;
					break;
			case 34:
					print('g');
					buffstr[i] = 'g';
					i++;
					break;
			case 35:
					print('h');
					buffstr[i] = 'h';
					i++;
					break;
			case 36:
					print('j');
					buffstr[i] = 'j';
					i++;
					break;
			case 37:
					print('k');
					buffstr[i] = 'k';
					i++;
					break;
			case 38:
					print('l');
					buffstr[i] = 'l';
					i++;
					break;
			case 39:
					print(';');
					buffstr[i] = ';';
					i++;
					break;
			case 40:
					if(upperCase){
						print('"');               //   Single quote (')
						buffstr[i] = '"';
						i++;
					} else {
						print('\'');               //   Single quote (')
						buffstr[i] = '\'';
						i++;
					}
					break;
			case 41:
					print((char)'`');               // Back tick (`)
					buffstr[i] = (char)'`';
					i++;
					break;
		 /* case 42:                                 Left shift 
					print('q');
					buffstr[i] = 'q';
					i++;
					break;
			case 43:                                 \ (< for somekeyboards)   
					print((char)92);
					buffstr[i] = 'q';
					i++;
					break;*/
			case 44:
					print('z');
					buffstr[i] = 'z';
					i++;
					break;
			case 45:
					print('x');
					buffstr[i] = 'x';
					i++;
					break;
			case 46:
					print('c');
					buffstr[i] = 'c';
					i++;
					break;
			case 47:
					print('v');
					buffstr[i] = 'v';
					i++;
					break;                
			case 48:
					print('b');
					buffstr[i] = 'b';
					i++;
					break;               
			case 49:
					print('n');
					buffstr[i] = 'n';
					i++;
					break;                
			case 50:
					print('m');
					buffstr[i] = 'm';
					i++;
					break;               
			case 51:
					print(',');
					buffstr[i] = ',';
					i++;
					break;                
			case 52:
					print('.');
					buffstr[i] = '.';
					i++;
					break;            
			case 53:
					print('/');
					buffstr[i] = '/';
					i++;
					break;            
			case 54:
					print('.');
					buffstr[i] = '.';
					i++;
					break;            
			case 55:
					print('/');
					buffstr[i] = '/';
					i++;
					break;            
		  /*case 56:
					print(' ');           Right shift
					buffstr[i] = ' ';
					i++;
					break;*/           
			case 57:
					print(' ');
					buffstr[i] = ' ';
					i++;
					break;
			case 58:
					upperCase = !upperCase; // Caps Lock
					break;
				}
			}
		}
		buffstr[i-1] = 0;         
		buffstr[i] = 0;                   		
		return buffstr;
	}
}