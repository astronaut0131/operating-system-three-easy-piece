#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <stdlib.h>

#include <mach/thread_act.h>
#include <mach/thread_policy.h>
#include <mach/task_info.h>
#include <pthread.h>

long get_time_diff(struct timeval end,struct timeval start);
void calculate_context_switch_cost(int switch_rounds);
void set_thread_affinity_tag(int affinity_tag);
/* about the processor affinity on OS X
	see https://github.com/asnr/ostep/tree/master/virtualisation/6_cpu_mechanisms
	and https://superuser.com/questions/149312/how-to-set-processor-affinity-on-os-x

*/
static const int AFFINITY_TAG_ALL_PROCS = 9;


int main(){
	set_thread_affinity_tag(AFFINITY_TAG_ALL_PROCS);//bind the affinity tags of all processes to the same processor
	for(int switch_rounds = 10;switch_rounds <= 10000;switch_rounds *= 10){
		calculate_context_switch_cost(switch_rounds);
		sleep(5);
	}
	return 0;
}

long get_time_diff(struct timeval end,struct timeval start){
	return (end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec;
}

void set_thread_affinity_tag(int affinity_tag)
{
    thread_affinity_policy_data_t affinity = { .affinity_tag = affinity_tag };

    thread_policy_set(pthread_mach_thread_np(pthread_self()),
                      THREAD_AFFINITY_POLICY,
                      (thread_policy_t) &affinity,
                      THREAD_AFFINITY_POLICY_COUNT);
}

void calculate_context_switch_cost(int switch_rounds){
	int fd_A_to_B[2];//process A output to fd_A_to_B[1] and fd_A_to_B[0] input to process B
	int fd_B_to_A[2];//process B output to fd_B_to_A[1] and fd_B_to_A[0] input to process A
	pipe(fd_A_to_B);
	pipe(fd_B_to_A);
	int rc1 = fork();
	char receiver[2];
	if(rc1 < 0){
		fprintf(stderr, "child process A failed\n");
	}
	else if(rc1 == 0){ //process A
		struct timeval start;
		struct timeval end;
		close(fd_A_to_B[0]);//these two fd has nothing to do with process A
		close(fd_B_to_A[1]);
		read(fd_B_to_A[0],receiver,1);
		gettimeofday(&start,NULL);
		for(int i=0;i<switch_rounds;i++){
			write(fd_A_to_B[1],">",1);
			read(fd_B_to_A[0],receiver,1);
		}
		gettimeofday(&end,NULL);
		printf("%d times of context switch consumes %ld ms\n",2*switch_rounds,get_time_diff(end,start));
		printf("on average,a context switch consumes %.2f ms\n",get_time_diff(end,start)/2.0/switch_rounds);
		exit(0);
	}
	else{
		int rc2 = fork();
		if(rc2 < 0){
			fprintf(stderr, "child process B failed\n");
		}
		else if(rc2 == 0){ //process B
			close(fd_A_to_B[1]);
			close(fd_B_to_A[0]);
			write(fd_B_to_A[1],">",1); //this statement act as a start signal,when A receives this, the test starts
			for(int i=0;i<switch_rounds;i++){
				read(fd_A_to_B[0],receiver,1);
				write(fd_B_to_A[1],">",1);
			}
			exit(0);
		}
		else{
			wait(NULL);
			return;
		}
	}
}


