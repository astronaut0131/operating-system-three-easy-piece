# Ch5 Homework
### Q1:Write a program that calls fork(). Before calling fork(), have the main process access a variable (e.g., x) and set its value to some- thing (e.g., 100). What value is the variable in the child process? What happens to the variable when both the child and parent change the value of x?
A1:The value in child process is the previous value.Both the child process and the parent process can change x into different values without conflict, their local variables are independent from each other.
```
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
```
```
changed x in parent process is 20
original x in child process is 100
changed x in child process is 10
```


### Q2:Write a program that opens a file (with the open() system call) and then calls fork() to create a new process. Can both the child and parent access the file descriptor returned by open()? What happens when they are writing to the file concurrently, i.e., at the same time?
So far I don’t know how to make two process writing to a file concurrently, so I write the code below instead.
A2:Both the child and parent process can access the file descriptor at the same time.They can write to the same file one after another, in that I turned O_TRUNC on, it look like the file is saved after both the child and parent process have finished writing.
```
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
```
```
fd in parent process is 3
parent process is writing content:this is written by parent
 into Q2.output
parent process finish writing
fd in child process is 3
child process is writing content:this is written by child
 into Q2.output
child process finish writing
```
```
➜  ~ sudo cat Q2.output
this is written by parent
this is written by child
```


### Q3:3. Write another program using fork(). The child process should print “hello”; the parent process should print “goodbye”. You should try to ensure that the child process always prints first; can you do this without calling wait() in the parent?
A3:Use  `pipe()` , [6.2.2 Creating Pipes in C](http://tldp.org/LDP/lpg/node11.html) contains the usage.In short, the output of fd1 is the input of fd0 , since the child and parent process shares the file description , so if we want to send message from child to parent, we must close fd0 in child and close fd1 in parent.
```
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
```
```
child:hello
parent:goodbye
```

### Q4:Write a program that calls fork() and then calls some form of exec() to run the program /bin/ls. See if you can try all of the variants of exec(),including execl(), execle(), execlp(), execv(), execvp(), and execvP(). Why do you think there are so many variants of the same basic call?
```

```
```

```
### Q5:Now write a program that uses wait() to wait for the child process to finish in the parent. What does wait() return? What happens if you use wait() in the child?
A5:As is written in Ch5 notes,`wait()` returns child process PID if child process exists, -1 if child process does not exist.In this case, the child process has no child process, so gets -1.
```
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
```
```
wait returns -1 in child process
wait returns 6436 in parent process
```
### Q6:Write a slight modification of the previous program, this time us- ing waitpid() instead of wait(). When would waitpid() be useful?
A6:Read [Wait System Call in C - GeeksforGeeks](https://www.geeksforgeeks.org/wait-system-call-c/) ,if a parent process has more than one child processes, `wait()` will only reap any arbitrarily child process, so if we want to reap a specified child process, we use `waitpid()` to do that.But so far I don’t know how to create more than one child process : P
```
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
```
```
running the child process (7004)
parent process is informed that child process (7004) has terminated
```
### Q7:Write a program that creates a child process, and then in the child closes standard output (STDOUT FILENO). What happens if the child calls printf() to print some output after closing the descriptor?
A7:Nothing happens because there is no file descriptor assigned to standard output after closing the default one, the process will output no where.We’ve learned before that we can open a file give the standard output file descriptor to it, and what we print will be write into it.
```
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
int main(){
	int rc = fork();
	if(rc < 0){
		fprintf(stderr, "fork failed\n");
	}
	else if(rc == 0){
		close(STDOUT_FILENO);
		printf("this is the content in printf after close STDOUT_FILENO\n");
	}
	else{
		int wc = wait(NULL);
		printf("child process %d has terminated",wc);
	}
	return 0;
}
```
```
child process 7209 has terminated
```
And we can try to open a file after close standard output.
```
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
```
```
➜  ~ sudo cat Q7.output 
Password:
this is the content in printf after close STDOUT_FILENO
```
### Q8:Write a program that creates two children, and connects the stan- dard output of one to the standard input of the other, using the pipe() system call.
A:About child process creation, see [how-to-use-fork-to-create-only-2-child-processes](https://stackoverflow.com/questions/10909011/how-to-use-fork-to-create-only-2-child-processes).Use dup2 to substitute file descriptor for standard input and output, see also [6.2.2 Creating Pipes in C](http://tldp.org/LDP/lpg/node11.html).
```
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
```
```
child process 1 is sending message:'this is the test message' into the pipeline
child process2 is receiving message from the pipeline
child process2 receives message:'this is the test message' from the pipeline
```