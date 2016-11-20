
#include <vga.h>
#include <system.h>
#include <stdtype.h>

char* readString()
{
    char buff;
    char* buffstr;
    int i = 0;
    bool reading = true;
    while(reading)
    {
        if(inportb(0x64) & 0x1)                 
        {
            /*switch(inportb(0x60))
            { 
      /*case 1:
                putc('(char)27);           Escape button
                buffstr[i] = (char)27;
                i++;
                break;* /
        case 2:
                putc('1');
                buffstr[i] = '1';
                i++;
                break;
        case 3:
                putc('2');
                buffstr[i] = '2';
                i++;
                break;
        case 4:
                putc('3');
                buffstr[i] = '3';
                i++;
                break;
        case 5:
                putc('4');
                buffstr[i] = '4';
                i++;
                break;
        case 6:
                putc('5');
                buffstr[i] = '5';
                i++;
                break;
        case 7:
                putc('6');
                buffstr[i] = '6';
                i++;
                break;
        case 8:
                putc('7');
                buffstr[i] = '7';
                i++;
                break;
        case 9:
                putc('8');
                buffstr[i] = '8';
                i++;
                break;
        case 10:
                putc('9');
                buffstr[i] = '9';
                i++;
                break;
        case 11:
                putc('0');
                buffstr[i] = '0';
                i++;
                break;
        case 12:
                putc('-');
                buffstr[i] = '-';
                i++;
                break;
        case 13:
                putc('=');
                buffstr[i] = '=';
                i++;
                break;
        case 14:
                putc('\b');
                i--;
                buffstr[i] = 0;
                break;
       /* case 15:
                putc('\t');          Tab button
                buffstr[i] = '\t';
                i++;
                break;* /
        case 16:
                putc('q');
                buffstr[i] = 'q';
                i++;
                break;
        case 17:
                putc('w');
                buffstr[i] = 'w';
                i++;
                break;
        case 18:
                putc('e');
                buffstr[i] = 'e';
                i++;
                break;
        case 19:
                putc('r');
                buffstr[i] = 'r';
                i++;
                break;
        case 20:
                putc('t');
                buffstr[i] = 't';
                i++;
                break;
        case 21:
                putc('y');
                buffstr[i] = 'y';
                i++;
                break;
        case 22:
                putc('u');
                buffstr[i] = 'u';
                i++;
                break;
        case 23:
                putc('i');
                buffstr[i] = 'i';
                i++;
                break;
        case 24:
                putc('o');
                buffstr[i] = 'o';
                i++;
                break;
        case 25:
                putc('p');
                buffstr[i] = 'p';
                i++;
                break;
        case 26:
                putc('[');
                buffstr[i] = '[';
                i++;
                break;
        case 27:
                putc(']');
                buffstr[i] = ']';
                i++;
                break;
        case 28:
               // putc('\n');
               // buffstr[i] = '\n';
                  i++;
               reading = 0;
                break;
      /*  case 29:
                putc('q');           Left Control
                buffstr[i] = 'q';
                i++;
                break;* /
        case 30:
                putc('a');
                buffstr[i] = 'a';
                i++;
                break;
        case 31:
                putc('s');
                buffstr[i] = 's';
                i++;
                break;
        case 32:
                putc('d');
                buffstr[i] = 'd';
                i++;
                break;
        case 33:
                putc('f');
                buffstr[i] = 'f';
                i++;
                break;
        case 34:
                putc('g');
                buffstr[i] = 'g';
                i++;
                break;
        case 35:
                putc('h');
                buffstr[i] = 'h';
                i++;
                break;
        case 36:
                putc('j');
                buffstr[i] = 'j';
                i++;
                break;
        case 37:
                putc('k');
                buffstr[i] = 'k';
                i++;
                break;
        case 38:
                putc('l');
                buffstr[i] = 'l';
                i++;
                break;
        case 39:
                putc(';');
                buffstr[i] = ';';
                i++;
                break;
        case 40:
                putc((char)44);               //   Single quote (')
                buffstr[i] = (char)44;
                i++;
                break;
        case 41:
                putc((char)44);               // Back tick (`)
                buffstr[i] = (char)44;
                i++;
                break;
     /* case 42:                                 Left shift 
                putc('q');
                buffstr[i] = 'q';
                i++;
                break;
        case 43:                                 \ (< for somekeyboards)   
                putc((char)92);
                buffstr[i] = 'q';
                i++;
                break;* /
        case 44:
                putc('z');
                buffstr[i] = 'z';
                i++;
                break;
        case 45:
                putc('x');
                buffstr[i] = 'x';
                i++;
                break;
        case 46:
                putc('c');
                buffstr[i] = 'c';
                i++;
                break;
        case 47:
                putc('v');
                buffstr[i] = 'v';
                i++;
                break;                
        case 48:
                putc('b');
                buffstr[i] = 'b';
                i++;
                break;               
        case 49:
                putc('n');
                buffstr[i] = 'n';
                i++;
                break;                
        case 50:
                putc('m');
                buffstr[i] = 'm';
                i++;
                break;               
        case 51:
                putc(',');
                buffstr[i] = ',';
                i++;
                break;                
        case 52:
                putc('.');
                buffstr[i] = '.';
                i++;
                break;            
        case 53:
                putc('/');
                buffstr[i] = '/';
                i++;
                break;            
        case 54:
                putc('.');
                buffstr[i] = '.';
                i++;
                break;            
        case 55:
                putc('/');
                buffstr[i] = '/';
                i++;
                break;            
      /*case 56:
                putc(' ');           Right shift
                buffstr[i] = ' ';
                i++;
                break;* /           
        case 57:
                putc(' ');
                buffstr[i] = ' ';
                i++;
                break;
            }*/
        }
    }
    buffstr[i] = 0;                   
    return buffstr;
}