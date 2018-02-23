#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc,char *argv[])
{
	int rc = fork();
	int x = 100;
	if(rc < 0){
		fprintf(stderr,"fork failed\n");
		exit(1);
	} else if(rc == 0){
		printf("original x in child process is %d\n", x);
		x = 10;
		printf("changed x in child process is %d\n",x);
	}
	else{
		x = 20;
		printf("changed x in parent process is %d\n",x);
	}
}