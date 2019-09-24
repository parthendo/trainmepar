#ifndef SEREVERUTILS_H
#define SEREVERUTILS_H

#include "utils.h"


struct user_info{
	int uid;
	char username[STDBUFFERSIZE+5];
	char password[STDBUFFERSIZE+5];
	char type;
	int loggedin;
};

struct train_info{
	int train_number;
	int ticket_count[31];
	char source[STDBUFFERSIZE+5];
	char destination[STDBUFFERSIZE+5];
};

struct user_train_info{
	int uid;
	int train_number;
	int date;
};



int create_connection();
int login(int);
void create_database();
void logout(int);

#endif