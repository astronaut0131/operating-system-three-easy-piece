#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
int main(){
	int fd[2];
	pipe(fd);
	int rc1 = fork();
	char message[] = "this is the test message";
	char readbuffer[100];
	if (rc1 < 0)
	{
		fprintf(stderr, "fail to create the first child process\n" );
	}
	else if(rc1 == 0){ 
		/*
		suppose we want to send message rc1->rc2
		we know that the output of fd[1] is the input of fd[0]
		so we can close fd[0] in rc1 and close fd[1] in rc2,this doesn't matter in this case.
		if we don't close fd[0] and fgets after writing into pipeline,
		the message will be caught by the fgets in the child process rather than the parent process 
		because the child process runs first,and parent process won't get the right message after child get the output of pipline.
		Thus,we may close fd[0] in child and fd[1] in parent.A
		*/
		printf("child process 1 is sending message:'%s' into the pipeline\n", message);
		close(fd[0]);
		dup2(fd[1],STDOUT_FILENO);
		printf("%s\n", message);
		}
	else{ //child 2 should be create by the same parent of child 1
		int rc2 = fork();
		if(rc2 < 0){

			fprintf(stderr, "fail to create the second child process\n" );
		}
		else if(rc2 == 0){
			printf("child process2 is receiving message from the pipeline\n");
			close(fd[1]);
			dup2(fd[0],STDIN_FILENO);
			fgets(readbuffer,80,stdin);
			readbuffer[strlen(readbuffer)-1] = '\0'; //get off the '/n'
			printf("child process2 receives message:'%s' from the pipeline\n",readbuffer);
		}
	}
}