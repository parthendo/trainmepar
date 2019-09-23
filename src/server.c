#include "utils.h"
#include "serverutils.h"

int main(){
	int listenfd = create_connection();
	login(listenfd);
	
	return 0;
}