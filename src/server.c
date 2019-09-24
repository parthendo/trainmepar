#include "utils.h"
#include "serverutils.h"
#include "serveradmin.h"

static int turn = 0;

int main(){
	//if(turn == 0)
		create_database();
	//turn++;
	while(1){
		int listenfd = create_connection();
		int connfd = accept(listenfd,(struct sockaddr*)NULL,NULL);
		if(fork() == 0){
			int type = login(connfd);
			//Admin account
			if(type == 1){
				printf("Enters admin\n");
				runadmin(connfd);
			}
			//User/agent account
			else if(type > 1){
				while(1){
					break;
				}
			}
		}
		else{
			close(connfd);
		}
	}
	return 0;
}