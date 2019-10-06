#include "utils.h"
#include "serverutils.h"

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
    	int port = 5000;
		serv_addr.sin_family = AF_INET;
    	serv_addr.sin_port = htons(port);
    	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    	
    	//serv_addr.sin_addr.s_addr = "172.16.130.46";

    	/*
    	for(port = 1024; port <= 49151; port++){
    		serv_addr.sin_port = htons(port);
			if(bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == 1)	
				break;
		}*/
		
		bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    	return listenfd;
	}
}

/*
 *	Login or new user
 * Returns 0 when admin, 1 when user/agent
 * -1 in all other cases
 */
int login(int connfd){
	//Data variables
	char username[STDBUFFERSIZE+5],password[STDBUFFERSIZE+5],type[STDBUFFERSIZE+5];

	//Accept connection
	read(connfd,username,STDBUFFERSIZE);

	//Searching user in the database
	int user_info_fd = open("Database/user_info_db",O_RDWR,0744);
	struct user_info obj;
	int flag = 0;
	while(read(user_info_fd,&obj,sizeof(obj))){
		if(strcmp(obj.username,username) == 0){
			flag = 1;
			break;
		}
	}
	if(flag){
		write(connfd,"1",STDBUFFERSIZE);
		read(connfd,password,STDBUFFERSIZE);
		if(strcmp(password,obj.password) == 0){
			printf("ACCESS GRANTED!!\n");
			if(obj.type == 'U' && obj.loggedin == 0){
				
				// Updating logged in status to 1
				// User session begins
				lseek(user_info_fd,-sizeof(obj),SEEK_CUR);
				obj.loggedin = 1;
				write(user_info_fd,&obj,sizeof(obj));
				write(connfd,itoa(obj.uid,10),STDBUFFERSIZE);
				close(user_info_fd);
				return obj.uid;
			}
			else if(obj.type == 'U' && obj.loggedin == 1){
				write(connfd,"-1",STDBUFFERSIZE);
				close(user_info_fd);
				return -1;
			}
			else if(obj.type == 'A'){
				lseek(user_info_fd,-sizeof(obj),SEEK_CUR);
				obj.loggedin++;
				write(user_info_fd,&obj,sizeof(obj));
				write(connfd,itoa(obj.uid,10),STDBUFFERSIZE);
				close(user_info_fd);
				return obj.uid;
			}
			else if(obj.type == 'O'){
				write(connfd,itoa(obj.uid,10),STDBUFFERSIZE);
				close(user_info_fd);
				return obj.uid;	
			}
		}
		else{
			write(connfd,"-2",STDBUFFERSIZE);
			close(user_info_fd);
			return -2;
		}
	}
	else{
		write(connfd,"-1",STDBUFFERSIZE);
		lseek(user_info_fd,-sizeof(obj),SEEK_CUR);
		read(user_info_fd,&obj,sizeof(obj));
		int new_UID = obj.uid+1;
		while(1){
			memset(username,'\0',sizeof(username));
			read(connfd,username,STDBUFFERSIZE);
			user_info_fd = open("Database/user_info_db",O_RDWR,0744);
			int flg = 0;
			while(read(user_info_fd,&obj,sizeof(obj))){
				if(strcmp(username,obj.username) == 0){
					flg = 1;
					break;
				}
			}
			if(flg == 0){
				write(connfd,"1",STDBUFFERSIZE);
				break;
			}
			else{
				write(connfd,"0",STDBUFFERSIZE);
			}
		}
		memset(type,'\0',sizeof(type));
		read(connfd,type,STDBUFFERSIZE);
		memset(password,'\0',sizeof(password));
		read(connfd,password,STDBUFFERSIZE);
		strcpy(obj.username,username);
		strcpy(obj.password,password);
		obj.uid = new_UID;
		obj.loggedin = 1;
		obj.type = type[0];
		//Reach the end of file
		lseek(user_info_fd,0,SEEK_END);
		write(user_info_fd,&obj,sizeof(obj));
		close(user_info_fd);
		write(connfd,itoa(new_UID,10),STDBUFFERSIZE);
		return new_UID;
	}
}


/*
 * Creates database files for user_info and train_info
 * Also enters the entry for the admin in users
 */
void create_database(){
	//Create database
	int user_info_fd = open("Database/user_info_db",O_CREAT|O_RDWR,0744);
	int train_info_fd = open("Database/train_info_db",O_CREAT|O_RDWR,0744);
	int usre_train_fd = open("Database/user_train_info_db",O_CREAT|O_RDWR,0744);
	struct train_info temp;
	temp.train_number = 1010;
	for(int i=0;i<30;i++)
		temp.ticket_count[i] = 100;
	strcpy(temp.source,"Kanpur");
	strcpy(temp.destination,"Bangalore");
	write(train_info_fd,&temp,sizeof(temp));
	close(train_info_fd);
	struct user_info obj;
	obj.uid = 1;
	strcpy(obj.username,"admin");
	strcpy(obj.password,"admin");
	obj.type = 'O';
	obj.loggedin = 0;
	write(user_info_fd,&obj,sizeof(obj));
	close(user_info_fd);
}

/*
 * Logs user out from server end
 */
void logout(int connfd){
	int user_info_fd, uid;
	struct user_info obj;
	char readbuffer[STDBUFFERSIZE+5];
	read(connfd,readbuffer,STDBUFFERSIZE);
	uid = stoi(readbuffer);
	user_info_fd = open("Database/user_info_db",O_RDWR,0744);
	while(read(user_info_fd,&obj,sizeof(obj))){
		if(uid == obj.uid)	break;
	}
	obj.loggedin--;
	lseek(user_info_fd,-sizeof(obj),SEEK_CUR);
	write(user_info_fd,&obj,sizeof(obj));
	close(user_info_fd);
}
