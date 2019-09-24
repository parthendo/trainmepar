#include "utils.h"
#include "serverutils.h"

/*
 * Add train to database
 */
void add_train(int connfd){
	int train_no;
	char readbuffer[STDBUFFERSIZE+5];
	read(connfd,&readbuffer,STDBUFFERSIZE);
	train_no = stoi(readbuffer);
	int train_info_fd = open("Database/train_info_db",O_RDWR,0744);
	struct train_info t1,t2;
	while(read(train_info_fd,&t1,sizeof(t1))){
		if(train_no == t1.train_number){
			write(connfd,"-1",STDBUFFERSIZE);
			close(train_info_fd);		
			return;
		}
	}
	write(connfd,"1",STDBUFFERSIZE);
	t2.train_number=train_no;
	read(connfd,&readbuffer,STDBUFFERSIZE);
	strcpy(t2.source,readbuffer);
	read(connfd,&readbuffer,STDBUFFERSIZE);
	strcpy(t2.destination,readbuffer);
	for(int i=0;i<31;i++){
		t2.ticket_count[i]=100;
	}
	lseek(train_info_fd,0,SEEK_END);
	write(train_info_fd,&t2,sizeof(t2));
	close(train_info_fd);
}

/*
 * Print train on server end
 */

void print_train(int connfd){
	char readbuffer[STDBUFFERSIZE+5];
	int train_info_fd = open("Database/train_info_db",O_RDWR,0744);
	struct train_info obj;
	int date;
	read(connfd,readbuffer,STDBUFFERSIZE);
	date = stoi(readbuffer);
	while(read(train_info_fd,&obj,sizeof(obj))){
		if(obj.ticket_count[date-1]){
			write(connfd,itoa(obj.train_number,10),STDBUFFERSIZE);
			write(connfd,itoa(obj.ticket_count[date-1],10),STDBUFFERSIZE);
			write(connfd,obj.source,STDBUFFERSIZE);
			write(connfd,obj.destination,STDBUFFERSIZE);
		}
	}
	write(connfd,"-1",STDBUFFERSIZE);
	close(train_info_fd);
}

/*
 * Modify train on server end
 */
void modify_train(int connfd){
	int train_no,choice;
	char readbuffer[STDBUFFERSIZE+5];
	struct train_info obj;
	//Print train to see and select the train number
	print_train(connfd);
	read(connfd,readbuffer,STDBUFFERSIZE);
	train_no = stoi(readbuffer);
	int train_info_fd = open("Database/train_info_db",O_RDWR,0744);
	//########IMPLEMENT FILE LOCKING HERE#############
	int flag = 0;
	while(read(train_info_fd,&obj,sizeof(obj))){
		if(obj.train_number == train_no){
			write(connfd,"1",STDBUFFERSIZE);
			flag = 1;
			break;
		}
	}
	if(flag){
		lseek(train_info_fd,-sizeof(obj),SEEK_CUR);
		read(connfd,readbuffer,STDBUFFERSIZE);
		choice = stoi(readbuffer);
		if(choice == 1){
			read(connfd,readbuffer,STDBUFFERSIZE);
			choice = stoi(readbuffer);
			read(connfd,readbuffer,STDBUFFERSIZE);
			obj.ticket_count[choice-1] = stoi(readbuffer);
		}
		else if(choice == 2){
			read(connfd,readbuffer,STDBUFFERSIZE);
			strcpy(obj.source,readbuffer);
		}
		else{
			read(connfd,readbuffer,STDBUFFERSIZE);
			strcpy(obj.destination,readbuffer);
		}
		write(train_info_fd,&obj,sizeof(obj));
	}
	else{
		write(connfd,"-1",STDBUFFERSIZE);
		//CHECK HERE
		modify_train(connfd);
		return;
	}
	close(train_info_fd);
}

void delete_train(int connfd){
	int train_no,choice;
	char readbuffer[STDBUFFERSIZE+5];
	struct train_info obj;
	//Print train to see and select the train number
	print_train(connfd);
	read(connfd,readbuffer,STDBUFFERSIZE);
	train_no = stoi(readbuffer);
	int train_info_fd = open("Database/train_info_db",O_RDWR,0744);
	//########IMPLEMENT FILE LOCKING HERE#############
	int flag = 0;
	while(read(train_info_fd,&obj,sizeof(obj))){
		if(obj.train_number == train_no){
			write(connfd,"1",STDBUFFERSIZE);
			flag = 1;
			break;
		}
	}
	if(flag){
		lseek(train_info_fd,0,SEEK_SET);
		int train_info_fd1 = open("Database/train_info_db1",O_CREAT|O_RDWR,0744);
		while(read(train_info_fd,&obj,sizeof(obj))){
			if(obj.train_number!=train_no){
				write(train_info_fd1,&obj,sizeof(obj));
			}
		}
		close(train_info_fd);
		close(train_info_fd1);
		system("rm Database/train_info_db");
		system("mv Database/train_info_db1 Database/train_info_db");
	}
	else{
		write(connfd,"-1",STDBUFFERSIZE);
		delete_train(connfd);
		return;
	}
}

/*
 * Display users
 */

void display_user(int connfd){
	int user_info_fd = 	open("Database/user_info_db",O_RDWR,0744);
	struct user_info obj;
	while(read(user_info_fd,&obj,sizeof(obj))){
		char temp[STDBUFFERSIZE+5];
		temp[0] = obj.type;
		temp[1] = '\0';
		write(connfd,itoa(obj.uid,10),STDBUFFERSIZE);
		write(connfd,obj.username,STDBUFFERSIZE);
		write(connfd,temp,STDBUFFERSIZE);
		write(connfd,itoa(obj.loggedin,10),STDBUFFERSIZE);
	}
	write(connfd,"-1",STDBUFFERSIZE);
	close(user_info_fd);
}

/*
 * 
 */

void delete_user(int connfd){
	int UID,choice;
	char readbuffer[STDBUFFERSIZE+5];
	struct user_info obj;
	//Print train to see and select the train number
	display_user(connfd);
	read(connfd,readbuffer,STDBUFFERSIZE);
	UID = stoi(readbuffer);
	int user_info_fd = open("Database/user_info_db",O_RDWR,0744);
	//########IMPLEMENT FILE LOCKING HERE#############
	int flag = 0;
	while(read(user_info_fd,&obj,sizeof(obj))){
		if(obj.uid == UID){
			write(connfd,"1",STDBUFFERSIZE);
			flag = 1;
			break;
		}
	}
	if(flag){
		lseek(user_info_fd,0,SEEK_SET);
		int user_info_fd1 = open("Database/user_info_db1",O_CREAT|O_RDWR,0744);
		while(read(user_info_fd,&obj,sizeof(obj))){
			if(obj.uid!=UID){
				write(user_info_fd1,&obj,sizeof(obj));
			}
		}
		close(user_info_fd);
		close(user_info_fd1);
		system("rm Database/user_info_db");
		system("mv Database/user_info_db1 Database/user_info_db");
	}
	else{
		write(connfd,"-1",STDBUFFERSIZE);
		delete_user(connfd);
		return;
	}
}


void runadmin(int connfd){
	int flag = 0;
	int user_info_fd;
	struct user_info obj;
	char readbuffer[STDBUFFERSIZE+5];
	while(1){
		read(connfd,readbuffer,STDBUFFERSIZE);
		int choice = stoi(readbuffer);
		printf("%d",choice);
		switch(choice){
			case 1:
				add_train(connfd);
			break;
			case 2:
				print_train(connfd);
			break;
			case 3:
				modify_train(connfd);
			break;
			case 4:
				delete_train(connfd);
			break;
			case 5:
				display_user(connfd);
			break;
			case 6:
				delete_user(connfd);
			break;
			case 7:
				user_info_fd = open("Database/user_info_db",O_RDWR,0744);
				read(user_info_fd,&obj,sizeof(obj));
				//Logging admin off
				obj.loggedin = 0;
				flag = 1;
			break;
		}
		if(flag) break;
	}
}