#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
int main(){
	int fd[2];
	pipe(fd);
	int rc = fork();
	char readbuffer[80];
	if(rc < 0){
		fprintf(stderr, "fork failed\n");
	}
	else if(rc == 0){
		close(fd[0]);
		printf("child:hello\n");
		char message[] = "goodbye";
		write(fd[1],message,strlen(message));

	}
	else{
		close(fd[1]);
		read(fd[0],readbuffer,sizeof(readbuffer));
		printf("parent:%s\n", readbuffer);
	}
}