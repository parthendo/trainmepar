#include "utils.h"
#include "userutils.h"
#include "admin.h"

int main(){
	char IP[STDBUFFERSIZE+5], buffer[STDBUFFERSIZE+5];
	display_initial();
	clearline;
	printf("Enter IP: ");
	scanf("%s",IP);
	int socketfd = create_connection(IP);
	if(socketfd!=0){
		int uid = user_login(socketfd);
		if(uid == 1){
			console_to_blue();
			admin_menu(socketfd);
			close(socketfd);
		}
		else if(uid>1){
			console_to_blue();
			//user_menu(socketfd);
			close(socketfd);
		}
		else{
			console_to_red();
			printf("Error: Login failed. Try again later!");
			exit(0);
		}
	}
	else{
		exit(0);
		//Error message in red
	}
	return 0;
}
