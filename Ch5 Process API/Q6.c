#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
int main(){
	int rc = fork();
	if(rc < 0){
		fprintf(stderr, "fork failed\n" );
	}
	else if(rc == 0){
		printf("running the child process (%d)\n", (int)getpid());
	}
	else{
		int wc = waitpid(rc,NULL,0);
		printf("parent process is informed that child process (%d) has terminated\n", wc);
	}
	return 0;
}