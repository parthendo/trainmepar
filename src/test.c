#include <pthread.h>
#include <stdio.h>
#include <string.h>

struct temp
{
	int a;
	pthread_t t;
};

void* thread_start(void* arg){
	struct temp *pt = (struct temp*)arg;
	pt->a++;
	printf("current id = %ld %d\n",pt->t,pt->a);
}

int main(){
	pthread_t id1,id2,id3;
	int ans = 0;
	int k;
	struct temp p;
	p.t = id1;
	p.a = 1;
	int g = pthread_create(&id1,NULL,thread_start,&p);
	printf("%d\n",g);
	p.t = id2;
	p.a = 2;
	g = pthread_create(&id2,NULL,thread_start,&p);
	printf("%d\n",g);
	p.t = id3;
	p.a = 3;
	g = pthread_create(&id3,NULL,thread_start,&p);
	printf("%d\n",g);
	char str[100];
	printf("%d\n",pthread_join(id1,(void*)&p));
	ans+=p.a;
	printf("%d\n",pthread_join(id2,(void*)&p));
	ans+=p.a;
	printf("%d\n",pthread_join(id3,(void*)&p));
	ans+=p.a;
	printf("ans %d\n",ans );
	return 0;
}