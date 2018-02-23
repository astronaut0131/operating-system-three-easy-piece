#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
int main(){
	int rc = fork();
	if(rc < 0){
		fprintf(stderr, "fork failed\n");
	}
	else if(rc == 0){
		int wc = wait(NULL);
		printf("wait returns %d in child process\n",wc);
	}
	else{
		int wc = wait(NULL);
		printf("wait returns %d in parent process\n",wc);
	}
	return 0;
}