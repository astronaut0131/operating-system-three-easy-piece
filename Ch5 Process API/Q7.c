#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
int main(){
	int rc = fork();
	if(rc < 0){
		fprintf(stderr, "fork failed\n");
	}
	else if(rc == 0){
		close(STDOUT_FILENO);
		open("Q7.output",O_WRONLY|O_CREAT|O_TRUNC);
		printf("this is the content in printf after close STDOUT_FILENO\n");
	}
	else{
		int wc = wait(NULL);
		printf("child process %d has terminated",wc);
	}
	return 0;
}