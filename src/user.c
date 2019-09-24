#include "utils.h"
#include "userutils.h"

void book_ticket(int socketfd){
	char readbuffer[STDBUFFERSIZE+5],readbuffer1[STDBUFFERSIZE+5],readbuffer2[STDBUFFERSIZE+5];
	clearconsole;
	//Input location
	console_to_blue();
	printf("Enter the source: ");
	console_to_green();
	scanf("%s",readbuffer);
	console_to_blue();
	printf("Enter the destination: ");
	console_to_green();
	scanf("%s",readbuffer1);
	console_to_blue();
	printf("Enter your date of journey: ");
	console_to_green();
	scanf("%s",readbuffer2);

	//Search location based trains
	write(socketfd,readbuffer,STDBUFFERSIZE);
	write(socketfd,readbuffer1,STDBUFFERSIZE);
	write(socketfd,readbuffer2,STDBUFFERSIZE);
	//Print relevant trains
	printf("Train Number\tDate\tSource\t\tDestination\n");
	while(1){
		read(socketfd,readbuffer,STDBUFFERSIZE);
		if(strcmp(readbuffer,"-1") == 0)
			break;
		//Print Train Number
		int l = strlen(readbuffer);
		printf("%s\t\t",readbuffer);
		//Print number of ticket day wise
		read(socketfd,readbuffer,STDBUFFERSIZE);
		printf("%s\t",readbuffer);
		//Print Source
		read(socketfd,readbuffer,STDBUFFERSIZE);
		printf("%s\t\t",readbuffer);
		//Print Destination
		read(socketfd,readbuffer,STDBUFFERSIZE);
		printf("%s\n",readbuffer);
	}
	
	//Book train

	while(1){
		printf("Enter Train No: ");
		scanf("%s",readbuffer);
		write(socketfd,&readbuffer,STDBUFFERSIZE);
		read(socketfd,&readbuffer1,STDBUFFERSIZE);
		if(strcmp(readbuffer1,"-1")==0){
			console_to_red();
			printf("\n Enter Correct train Number");
		}
		else
			break;
	}
	clearconsole;
	console_to_magenta();
	printf("\n Ticket Book successfully\n");
	reset_color_console();

}

void ticket_history(int socketfd, int uid){
	char readbuffer[STDBUFFERSIZE+5];
	console_to_blue();
	printf("UID\tTrain Number\tDate\n");
	reset_color_console();
	while(1){
		read(socketfd,readbuffer,STDBUFFERSIZE);
		if(strcmp(readbuffer,"-1") == 0)
			break;
		printf("%s\t",readbuffer);
		read(socketfd,readbuffer,STDBUFFERSIZE);
		printf("%s\t",readbuffer);
		read(socketfd,readbuffer,STDBUFFERSIZE);
		printf("%s\n",readbuffer);
	}	
}


void user_menu(int socketfd,int uid){

	int choice;
	while(1){
		printf("Press: \n1. Book Ticket\n2. Booking History\n3. Cancel Ticket\n4. Logout instance and exit\n");
		scanf("%d",&choice);
		switch(choice){
			case 1:
				write(socketfd,"1",STDBUFFERSIZE);
				book_ticket(socketfd);
			break;
			case 2:
				write(socketfd,"2",STDBUFFERSIZE);
				ticket_history(socketfd,uid);
			break;
			case 3:
				write(socketfd,"3",STDBUFFERSIZE);
			break;
			case 4:
				write(socketfd,"4",STDBUFFERSIZE);
				user_logout(socketfd,uid);
			return;
		}
	}
}