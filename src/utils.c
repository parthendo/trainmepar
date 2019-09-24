#include "utils.h"

/*
 * Color Pallete: Changes console color scheme.
 */
inline void console_to_red(){
	printf("\033[1;31m");
}
inline void console_to_green(){
	printf("\033[1;32m");
}
inline void console_to_yellow(){
	printf("\033[1;33m");
}
inline void console_to_blue(){
	printf("\033[1;34m");
}
inline void console_to_magenta(){
	printf("\033[1;35m");
}
inline void console_to_cyan(){
	printf("\033[1;36m");
}
inline void reset_color_console(){
	printf("\033[0m");
}

/*
 * This method uses FIGlet and TOIlet.
 * sudo apt install figlet toilet
 */
inline void display_initial(){
	console_to_cyan();
	system("figlet trainmepar");
	printf("\t\tAn over-the-network ticket booking system\n");
	sleep(1);
	reset_color_console();
}

/*
 * Convert string to int
 */

int stoi(char* str){
	int l = strlen(str);
	int num = 0;
	for(int i=0;i<l;i++)
		num = num*10 + (str[i]-'0');
	return num;
}

char* itoa(int val, int base){
	
	static char buf[32] = {0};
	int i = 30;
	for(; val && i ; --i, val /= base)
		buf[i] = "0123456789abcdef"[val % base];
	return &buf[i+1];
	
}
	