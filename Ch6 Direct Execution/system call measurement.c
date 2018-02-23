/*
struct timespec {
	time_t tv_sec;   <-- second
	long tv_nsec;    <-- nanosecond
}
*/
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <math.h>
int main(){
	char readbuffer[100];
	for(int i=100;i<=1000000;i*=10){	
		struct timespec time1 = {0,0};
		struct timespec time2 = {0,0};
		clock_gettime(CLOCK_REALTIME,&time1);
		for(int j=0;j<i;j++){
			read(STDIN_FILENO,readbuffer,0);
		}
		clock_gettime(CLOCK_REALTIME,&time2);
		long int total_time = (time2.tv_sec - time1.tv_sec) * pow(10,9) + time2.tv_nsec - time1.tv_nsec;
		printf("%d times of read() takes %ld ns,on average %ld ns/read() \n",i,total_time,total_time/i);	
	}	
	return 0;
}