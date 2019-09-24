#ifndef UTILS_H
#define	UTILS_H

#include <stdio.h>
#include <math.h>
#include <sys/socket.h> 
#include <netdb.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <time.h> 

#define clearline	 printf("\n");
#define clearconsole printf("\e[1;1H\e[2J")
#define STDBUFFERSIZE 50

void display_initial();

int stoi(char*);
char* itoa(int,int);
void console_to_red();
void console_to_green();
void console_to_yellow();
void console_to_blue();
void console_to_magenta();
void console_to_cyan();
void reset_color_console();
#endif
