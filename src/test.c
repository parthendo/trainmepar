#include <pthread.h>
#include <stdio.h>
#include <string.h>

int stoi(char* str){
	int l = strlen(str);
	int num = 0;
	int flag = 0;
	for(int i=0;i<l;i++){
		if(i == 0 && !(str[i]>='0' && str[i]<='9')){
			flag = 1;continue;
		}
		num = num*10 + (str[i]-'0');
	}
	if(flag) num*=-1;
	return num;
}

int main(){
	int uid = 1;
	printf("%d",stoi("-2"));
	return 0;
}