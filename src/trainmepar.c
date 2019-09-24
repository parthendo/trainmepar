#include "utils.h"
#include "userutils.h"
#include "player.h"

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
			user_menu(socketfd,uid);
			close(socketfd);
		}
	}
	else{
		exit(0);
	}
	return 0;
}
