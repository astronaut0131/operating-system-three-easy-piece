#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
/*
This argument formed by OR'ing together optional parameters and (from <fcntl.h>) one of:

O_RDONLY, O_RDWR and O_WRONLY
Option parameters include:

O_APPEND data written will be appended to the end of the file. The file operations will always adjust the position pointer to the end of the file.
O_CREAT Create the file if it does not exist; otherwise the open fails setting errno to ENOENT.
O_EXCL Used with O_CREAT if the file already exists, then fail, setting errno to EEXIST.
O_TRUNC If the file already exists then discard its previous contents, reducing it to an empty file. Not applicable for a device or named pipe.
*/
int main(int argc,char *argv[])
{
	int fd = open("Q2.output",O_WRONLY|O_CREAT|O_TRUNC);
	if(fd < 0){
		printf("open Q2.output failed!\n");
		exit(1);
	}
	int rc = fork();
	if(rc < 0){
		fprintf(stderr, "fork failed" );
		exit(1);
	}
	else if(rc == 0){
		char child_content[] = "this is written by child\n";
		printf("fd in child process is %d\n",fd);
		printf("child process is writing content:%s into Q2.output\n", child_content);
		write(fd,child_content,strlen(child_content));
		printf("child process finish writing\n");
	}
	else{
		char parent_content[] = "this is written by parent\n";
		printf("fd in parent process is %d\n",fd);
		printf("parent process is writing content:%s into Q2.output\n", parent_content);
		write(fd,parent_content,strlen(parent_content));
		printf("parent process finish writing\n");
	}
	return 0;
}