#include "utils.h"
#include "serverutils.h"
#include "serverplayer.h"

static int turn = 0;

int main(){
	//if(turn == 0)
		//create_database();
	//turn++;
	int connfd;
	int listenfd = create_connection();
	listen(listenfd,10);
	while(1){
		connfd = accept(listenfd,(struct sockaddr*)NULL,NULL);
		if(fork() == 0){
			close(listenfd);
			int type = login(connfd);
			//Admin account
			if(type == 1){
				printf("Enters admin\n");
				runadmin(connfd);
				break;
			}
			//User/agent account
			else if(type > 1){
				printf("pp\n");
				runuser(connfd,type);
				break;
			}
			else{
				break;
			}
		}
	}
	close(connfd);
	return 0;
}