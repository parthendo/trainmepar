#include "utils.h"
#include "serverutils.h"

/*
 * Book ticket on the server end
 */
void book_ticket(int connfd, int uid){
	struct flock fl;
    memset(&fl, 0, sizeof(fl));
	fl.l_type = F_WRLCK;
	fl.l_whence = SEEK_CUR;
	fl.l_start = 0;
	fl.l_len = sizeof(struct train_info);
	struct train_info obj;
	int flag,train_no, train_info_fd = open("Database/train_info_db",O_RDWR,0744);
	char train[STDBUFFERSIZE+5],readbuffer[STDBUFFERSIZE+5],readbuffer1[STDBUFFERSIZE+5],readbuffer2[STDBUFFERSIZE+5];
	read(connfd,readbuffer,STDBUFFERSIZE);
	read(connfd,readbuffer1,STDBUFFERSIZE);
	read(connfd,readbuffer2,STDBUFFERSIZE);
	while(read(train_info_fd,&obj,sizeof(obj))){
		if(strcmp(obj.source,readbuffer) == 0 && strcmp(obj.destination,readbuffer1) == 0 
			&& obj.ticket_count[stoi(readbuffer2)-1]>0){
			
			write(connfd,itoa(obj.train_number,10),STDBUFFERSIZE);
			write(connfd,itoa(obj.ticket_count[stoi(readbuffer2)-1],10),STDBUFFERSIZE);
			write(connfd,obj.source,STDBUFFERSIZE);
			write(connfd,obj.destination,STDBUFFERSIZE);
		}
	}
	write(connfd,"-1",STDBUFFERSIZE);
	while(1){
		read(connfd,train,STDBUFFERSIZE);
		train_no = stoi(train);
		lseek(train_info_fd,0,SEEK_SET);
		flag = 0;
		while(read(train_info_fd,&obj,sizeof(obj))){
			if(strcmp(obj.source,readbuffer) == 0 && strcmp(obj.destination,readbuffer1) == 0 && 
				obj.ticket_count[stoi(readbuffer2)-1]>0 && obj.train_number == train_no){
				lseek(train_info_fd,-sizeof(obj),SEEK_CUR);
			
				if(fcntl(train_info_fd, F_SETLK, &fl) == -1){
					write(connfd, "-2",STDBUFFERSIZE);
					close(train_info_fd);
					return ;
				}
				write(connfd,"1",STDBUFFERSIZE);
				flag = 1;
				break;
			}
		}
		if(!flag)	write(connfd,"-1",STDBUFFERSIZE);
		else		break;
	}
	
	obj.ticket_count[stoi(readbuffer2)-1]--;
	write(train_info_fd,&obj,sizeof(obj));
	close(train_info_fd);

	//Update User-Train info file
	int user_train_info = open("Database/user_train_info_db",O_RDWR,0744);
	struct user_train_info temp_obj;
	temp_obj.uid = uid;
	temp_obj.train_number = obj.train_number;
	temp_obj.date = stoi(readbuffer2);
	lseek(user_train_info,0,SEEK_END);
	write(user_train_info,&temp_obj,sizeof(temp_obj));
	close(user_train_info);
}

void ticket_history(int connfd,int uid){
	struct user_train_info obj;
	int user_train_info = open("Database/user_train_info_db",O_RDWR,0744);
	while(read(user_train_info,&obj,sizeof(obj))){
		if(obj.uid == uid){
			write(connfd,itoa(uid,10),STDBUFFERSIZE);
			write(connfd,itoa(obj.train_number,10),STDBUFFERSIZE);
			write(connfd,itoa(obj.date,10),STDBUFFERSIZE);
		}
	}
	
	write(connfd,"-1",STDBUFFERSIZE);
}

void cancel_ticket(int connfd,int uid){
	struct user_train_info obj;
	char readbuffer[STDBUFFERSIZE+5],readbuffer1[STDBUFFERSIZE+5];
	int user_train_info = open("Database/user_train_info_db",O_RDWR,0744);
	ticket_history(connfd,uid);
	read(connfd,readbuffer,STDBUFFERSIZE);
	read(connfd,readbuffer1,STDBUFFERSIZE);
	int train_no=stoi(readbuffer);
	int date=stoi(readbuffer1);
	int flag=0;
	while(read(user_train_info,&obj,sizeof(obj))){
		if(obj.train_number==train_no && obj.date==date){
			write(connfd,"1",STDBUFFERSIZE);
				flag = 1;
				break;
		}
	}
	if(flag){
		lseek(user_train_info,0,SEEK_SET);
		int user_train_info_fd1 = open("Database/user_train_info_db1",O_CREAT|O_RDWR,0744);
		while(read(user_train_info,&obj,sizeof(obj))){
			if(obj.train_number!=train_no && obj.date!=date){
				write(user_train_info_fd1,&obj,sizeof(obj));
			}
		}
		close(user_train_info);
		close(user_train_info_fd1);
		system("rm Database/user_train_info_db");
		system("mv Database/user_train_info_db1 Database/user_train_info_db");
		int train_info_fd = open("Database/train_info_db",O_RDWR,0744);
		struct train_info obj;
		while(read(train_info_fd,&obj,sizeof(obj))){
			if(obj.train_number==train_no){
				obj.ticket_count[date-1]++;
				break;
			}
		}
		close(train_info_fd);
	}
	else{
		write(connfd,"-1",STDBUFFERSIZE);
		return;
	}
}

void runuser(int connfd,int uid){
	char readbuffer[STDBUFFERSIZE+5];
	int flag = 0;
	while(1){
		read(connfd,readbuffer,STDBUFFERSIZE);
		int choice = stoi(readbuffer);
		printf("%d",choice);
		switch(choice){
			case 1:
				book_ticket(connfd,uid);
			break;
			case 2:
				ticket_history(connfd,uid);
			break;
			case 3:
				cancel_ticket(connfd,uid);
			break;
			case 4:
				logout(connfd);
				flag = 1;
			break;
		}
		if(flag)	break;
	}
}