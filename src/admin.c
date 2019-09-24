#include "admin.h"

/*
 * Add new train to the database
 */
void add_train(int socketfd){
	char readbuffer[STDBUFFERSIZE+5];
	clearconsole;
	console_to_blue();
	printf("Enter Train No: ");
	console_to_green();
	scanf("%s",readbuffer);
	write(socketfd,&readbuffer,STDBUFFERSIZE);
	read(socketfd,&readbuffer,STDBUFFERSIZE);
	if(strcmp(readbuffer,"-1") == 0){
		console_to_red();
		printf("Train already Exist\n");
		return;
	}
	console_to_blue();
	printf("Enter Source Station: ");
	console_to_green();
	scanf("%s",readbuffer);
	write(socketfd,&readbuffer,STDBUFFERSIZE);
	console_to_blue();
	
	printf("Enter Destination Station: ");
	console_to_green();
	scanf("%s",readbuffer);
	write(socketfd,&readbuffer,STDBUFFERSIZE);

	console_to_magenta();
	printf("Add Train successfully!\n");
}

/*
 * Display train from the server end.
 */
void print_train(int socketfd){
	char readbuffer[STDBUFFERSIZE+5];
	int date;
	printf("Enter your day of travel: ");
	scanf("%d",&date);
	write(socketfd,itoa(date,10),STDBUFFERSIZE);
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
}

/*
 * Modify train
 */
void modify_train(int socketfd){
	//Variables
	int train_no;
	int date,tickets;
	char readbuffer[STDBUFFERSIZE+5];
	//Menu
	console_to_cyan();
	printf("See the train in the below table and select the train number you want to modify\n\n");
	console_to_blue();
	print_train(socketfd);
	console_to_green();
	
	printf("Enter the train number: ");
	scanf("%d",&train_no);
	//Check if train exists in the database
	write(socketfd,itoa(train_no,10),STDBUFFERSIZE);
	read(socketfd,readbuffer,STDBUFFERSIZE);
	clearconsole;
	if(strcmp(readbuffer,"1") == 0){
		while(1){
			int choice;
			console_to_blue();
			
			printf("For train: ");
			console_to_green();
			printf("%d\n",train_no);
			console_to_blue();
			printf("Press\n1 to modify tickets\n2 to modify source\n3 to modify destination\n");
			scanf("%d",&choice);
			switch(choice){
				case 1:
					
					printf("Enter the date: "); 
					console_to_green();
					scanf("%d",&date);
					console_to_blue();
					
					printf("Enter the tickets: ");
					console_to_green();
					scanf("%d",&tickets);
					write(socketfd,"1",STDBUFFERSIZE);
					write(socketfd,itoa(date,10),STDBUFFERSIZE);
					write(socketfd,itoa(tickets,10),STDBUFFERSIZE);
					printf("Tickets on date %d for train %d are set to %d successfully!\n",date,tickets,train_no);
					break;
				case 2:
					console_to_blue();
					printf("Enter the new source of the train: ");
					console_to_green();
					scanf("%s",readbuffer);
					write(socketfd,"2",STDBUFFERSIZE);
					write(socketfd,readbuffer,STDBUFFERSIZE);
					printf("New source for the train %d is %s\n",train_no,readbuffer);
					break;
				case 3:
					console_to_blue();
					
					printf("Enter the new destination of the train: ");
					console_to_green();
					scanf("%s",readbuffer);
					write(socketfd,"3",STDBUFFERSIZE);
					write(socketfd,readbuffer,STDBUFFERSIZE);
					printf("New destination for the train %d is %s\n",train_no,readbuffer);
					break;
				default:
					clearconsole;
					console_to_red();
	  				printf("Invalid choice\n");
					break;
	  		}
			console_to_cyan();
			
			printf("Press enter to exit");
	  		if(getchar() == '\n')
				break;
		}
	}
	else{
		modify_train(socketfd);
		return;
	}
}

/*
 * Delete train from database
 */
void delete_train(int socketfd){
	//Variables
	int train_no;
	char readbuffer[STDBUFFERSIZE+5];
	//Menu
	console_to_cyan();
	printf("See the train in the below table and select the train number you want to delete\n\n");
	console_to_blue();
	print_train(socketfd);
	console_to_green();
	
	printf("Enter the train number: ");
	scanf("%d",&train_no);
	//Check if train exists in the database
	write(socketfd,itoa(train_no,10),STDBUFFERSIZE);
	read(socketfd,readbuffer,STDBUFFERSIZE);
	clearconsole;
	if(strcmp(readbuffer,"1") == 0){
		console_to_green();
		printf("Train number %d is deleted from the database\n",train_no);
	}
	else{
		delete_train(socketfd);
		return;
	}
}

/*
 * Prints users in the list
 */
void print_user(int socketfd){
	char readbuffer[STDBUFFERSIZE+5];
	printf("UID\tUsername\tType\tLogged In Instances\n");
	while(1){
		read(socketfd,readbuffer,STDBUFFERSIZE);
		if(strcmp(readbuffer,"-1") == 0)
			break;
		printf("%s\t",readbuffer);
		read(socketfd,readbuffer,STDBUFFERSIZE);
		printf("%s\t",readbuffer);
		read(socketfd,readbuffer,STDBUFFERSIZE);
		printf("%s\t",readbuffer);
		read(socketfd,readbuffer,STDBUFFERSIZE);
		printf("%s\n",readbuffer);
	}	
}

/*
 * Delete user
 */
void delete_user(int socketfd){
	//Variables
	int UID;
	char readbuffer[STDBUFFERSIZE+5];
	//Menu
	console_to_cyan();
	printf("See the user in the below table and select the user number you want to delete\n\n");
	console_to_blue();
	print_user(socketfd);
	console_to_green();
	
	printf("Enter the UID: ");
	scanf("%d",&UID);
	//Check if UID exists in the database
	write(socketfd,itoa(UID,10),STDBUFFERSIZE);
	read(socketfd,readbuffer,STDBUFFERSIZE);
	clearconsole;
	if(strcmp(readbuffer,"1") == 0){
		console_to_green();
		printf("UID %d is deleted from the database\n",UID);
	}
	else{
		delete_user(socketfd);
		return;
	}
}

void admin_menu(int socketfd){

	clearconsole;
	int ch;
	while(1){
		console_to_blue();
		printf("Press\n1 to add train\n2 to diplay trains\n3 to modify train\n4 to delete train\n5 to display user\n6 to delete user\nlog out admin and exit\n");
		console_to_green();
		
		printf("\nEnter choice: ");
		scanf("%d",&ch);
		switch(ch){
			case 1:
				printf("here\n");
				write(socketfd,"1",STDBUFFERSIZE);
				add_train(socketfd);
				break;
			case 2:
				write(socketfd,"2",STDBUFFERSIZE);
				print_train(socketfd);
				break;
			case 3:
				write(socketfd,"3",STDBUFFERSIZE);
				modify_train(socketfd);
				break;
			case 4:
				write(socketfd,"4",STDBUFFERSIZE);
				delete_train(socketfd);
				break;
			case 5:
				write(socketfd,"5",STDBUFFERSIZE);
				print_user(socketfd);
				break;
			case 6:
				write(socketfd,"6",STDBUFFERSIZE);
				delete_user(socketfd);
				break;
			case 7:
				write(socketfd,"7",STDBUFFERSIZE);
				return;
			default:
				console_to_red();
				clearconsole;
				printf("Invalid choice, try again\n");
				break;
		}
	}
}

