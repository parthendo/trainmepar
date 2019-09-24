#------Final Target-----#
all:	server.out user.out

#------Compiler-----#
compiler = gcc -w
	
#------Object File-----#
utils.o:	 utils.c utils.h
	$(compiler) -c -o utils.o utils.c
userutils.o: userutils.c userutils.h utils.h
	$(compiler) -c -o userutils.o userutils.c
admin.o:	admin.c player.h userutils.h utils.h
	$(compiler) -c -o admin.o admin.c
user.o:		user.c player.h userutils.h utils.h
	$(compiler) -c -o user.o user.c
trainmepar.o: trainmepar.c player.h userutils.h utils.h
	$(compiler) -c -o trainmepar.o trainmepar.c
serverutils.o:	serverutils.c serverutils.h utils.h
	$(compiler) -c -o serverutils.o serverutils.c
serveradmin.o: serveradmin.c serverplayer.h serverutils.h utils.h
	$(compiler) -c -o serveradmin.o serveradmin.c
serveruser.o: serveruser.c serverplayer.h serverutils.h utils.h
	$(compiler) -c -o serveruser.o serveruser.c
server.o: server.c serverplayer.h serverutils.h utils.h
	$(compiler) -c -o server.o server.c
#------Prerequisite-----#
SERVER_P = utils.o serverutils.o serveradmin.o serveruser.o server.o
SERVER_R = $(compiler) -o server.out $(SERVER_P)

USER_P = utils.o userutils.o admin.o user.o trainmepar.o
USER_R = $(compiler) -o user.out $(USER_P)
#------Exe files-----#
server.out: $(SERVER_P)
	$(SERVER_R)
user.out: $(USER_P)
	$(USER_R)
#------Clean-----#
clean:
	rm -f *~ *.o 
