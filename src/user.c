#include "utils.h"
#include "userutils.h"

int main(){
	char IP[STDBUFFERSIZE+5];
	display_initial();
	clearline;
	printf("Enter IP: ");
	scanf("%s",IP);
	int socketfd = create_connection(IP);
	if(socketfd!=0){
		int uid = user_login(socketfd);
		if(uid){
			printf("%d",uid);
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
