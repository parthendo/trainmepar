#include "utils.h"

/*
 * Method that takes in the constant string containing the IP of the server as 
 * an input. Finding next available port for interaction. 
 * If successful, returns socket descriptor
 * Else returns 0
 */

int create_connection(char* IP){
	int socketfd, connfd;
	struct sockaddr_in servaddr,cli;
	socketfd = socket(AF_INET,SOCK_STREAM,0);
	if(socketfd == -1){
		console_to_red();
		printf("Error: Unable to establish connection. Try again later!\n");
		reset_color_console();
		return 0;
	}
	else{
		//Clear servaddr
    	bzero(&servaddr,sizeof(servaddr));

		// Assigning the IP and PORT
		// Implements linear search to find the next available port to establish connection
		servaddr.sin_family = AF_INET; 
    	servaddr.sin_addr.s_addr = inet_addr((const char*)IP); 
	//inet_aton(IP, &servaddr.sin_addr);    	
	int flag,port = 5000;/*
		for(port = 1024; port <= 49151; port++){
			servaddr.sin_port = htons(port); 
			if(connect(socketfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0)
				flag = 0;
			else{
				//Connection established
				flag = 1;
				break;
			}
		}*/
		servaddr.sin_port = htons(port); 
		connect(socketfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
		flag = 1;
		if(!flag){
			console_to_red();
			printf("Error: Connection with server failed. Try again later!\n");
			reset_color_console();
			return 0;
		}
		else{
			console_to_green();
			printf("Success message: Connection established successfully on port %d!",port);
			return socketfd; 
		}
	}
}

/*
 * Creates new user. Only one admin. Predefined. Multiple agents and users can be
 * registered. Returns UID 
 */

int create_new_user(int socketfd){
	
	char username[STDBUFFERSIZE+5],type[STDBUFFERSIZE+5],password[STDBUFFERSIZE+5],string[STDBUFFERSIZE+5];
	char* str;
	clearconsole;
	while(1){
		console_to_magenta();
		printf("\tRegister new user:\n");
		printf("Username: ");
		scanf("%s",username);
		write(socketfd,username,STDBUFFERSIZE);
		read(socketfd,string,STDBUFFERSIZE);
		if(strcmp(string,"1") == 0)
			break;
		else{
			clearconsole;
			console_to_red();
			printf("Username already taken\n");
		}
	}
	printf("Type(U/A): ");
	while(1){
		scanf("%s",type);
		if(!strcmp(type,"U") || !strcmp(type,"A")){
			write(socketfd,type,STDBUFFERSIZE);
			break;
		}
		else{
			clearconsole;
			console_to_magenta();
			printf("\tRegister new user:\n");
			printf("Username: %s\nType(U/A): ",username);
		}
	}
	printf("Password: ");
	str = getpass("\n");
	strcpy(password,str);
	write(socketfd,password,STDBUFFERSIZE);
	//Read UID
	read(socketfd,string,STDBUFFERSIZE);
	return stoi(string);
}


/*
 * Method:	Takes in the socket descriptor. If the username is found in the database,
 * it verifies the password else calls for a new user window. Returns the UID for each
 * user.
 */

int user_login(int socketfd){
	char string[STDBUFFERSIZE+5];
	char* str;
	clearconsole;
	//Username
	console_to_blue();
	printf("Username: ");
	scanf("%s",string);
	reset_color_console();
	write(socketfd,string,STDBUFFERSIZE);
	memset(string,'\0',sizeof(string));
	read(socketfd,string,STDBUFFERSIZE);
	//Password
	if(strcmp(string,"1") == 0){
		console_to_blue();
		printf("Password: ");
		reset_color_console();
		str = getpass("\n");
		strcpy(string,str);
		write(socketfd,string,STDBUFFERSIZE);
		memset(string,'\0',sizeof(string));
		read(socketfd,string,STDBUFFERSIZE);
		int uid = stoi(string);
		printf("here %d\n",uid);
		if(uid == -1){
			console_to_red();
			printf("Error: Multiple logins by user!\nTry again later!\n");
			reset_color_console();
			close(socketfd);
		}
		else if(uid == -2){
			console_to_red();
			printf("Error: Password does not match!\nTry again later!\n");
			reset_color_console();
			close(socketfd);
		}
		else
			return uid;
	}
	//New User
	else{
		console_to_magenta();
		printf("New user? Press enter to create a new account or any other key to exit:");
		if(getchar() == '\n'){
			return create_new_user(socketfd);
		}
		else{
			close(socketfd);
			exit(0);
		}
	}
	
}

/*
 * Logs out the user
 */
void user_logout(int socketfd,int uid){
	write(socketfd,itoa(uid,10),STDBUFFERSIZE);
}
