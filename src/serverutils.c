#include "utils.h"

/*
 * Connects on the server end. Updates log
 */

int create_connection(){
	char buffer[1024];
	int listenfd = 0;
	struct sockaddr_in serv_addr;

	listenfd = socket(AF_INET,SOCK_STREAM,0);
	if(listenfd == -1){
		console_to_red();
		printf("Error: Unable to establish connection. Try again later!\n");
		reset_color_console();
		return 0;
	}
	else{
		//Clear servaddr
    	bzero(&serv_addr,sizeof(serv_addr));
		
    	//UPDATE LOG FOR A NEW USER OR USER CONNECTION ATTEMPT

		serv_addr.sin_family = AF_INET;
    	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    	int port;
    	for(port = 1024; port <= 49151; port++){
    		serv_addr.sin_port = htons(port);
			if(bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == 1)	
				break;
		}
    	listen(listenfd,1000);
    	return listenfd;
	}
}

/*
 *	Login or new user
 */
void login(int listenfd){
	//Data variables
	char username[STDBUFFERSIZE+5],password[STDBUFFERSIZE+5],type[STDBUFFERSIZE+5];

	//Accept connection
	int connfd = accept(listenfd,(struct sockaddr*)NULL,NULL);
	read(connfd,username,STDBUFFERSIZE);
	printf("USERNAME INPUT: %s",username);
	//#############SEARCH USERNAME#############
	if(strcmp(username,"admin") == 0){
		write(connfd,"1",STDBUFFERSIZE);
		read(connfd,password,STDBUFFERSIZE);
		if(strcmp(password,"admin") == 0){
			printf("ACCESS GRANTED!!\n");
			//#############RETURN UID//#############
			write(connfd,"1",STDBUFFERSIZE);
		}
		else
			write(connfd,"-1",STDBUFFERSIZE);
	}
	else{
		printf("NEW USER COMING SOON");
	}
}