#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
static const int PAGE_SIZE = 4096;
long strtol_exit_if_fail(const char *str,const char *varname){
	char *endptr;
	long value = strtol(str,&endptr,10);
	if(*str == '\0' || *endptr != '\0'){
		fprintf(stderr, "Error:\"%s\" is not an integer\n", varname);
		exit(1);
	}
	return value;
}
int main(int argc,char const * argv[]){
	if(argc != 3){
		printf("the number of arguments is invalid for the program\n");
	}
	long int num_pages = strtol_exit_if_fail(argv[1],"num_pages_to_touch");
	long int num_trials = strtol_exit_if_fail(argv[2],"num_trials");
	size_t arrsize = num_pages * (PAGE_SIZE / sizeof(int));
	int *arr = (int *) calloc(arrsize,sizeof(int));
	size_t jump = PAGE_SIZE / sizeof(int);
	struct timeval start,end;
	for(size_t idx = 0;idx < arrsize;idx += jump){
		arr[idx] += 1;
	}
	int rc1 = gettimeofday(&start,NULL);
	if(rc1 == -1){
		fprintf(stderr, "Error: first gettimeofday() failed\n");
		exit(1);
	}
	for(int trial = 0; trial < num_trials;trial++){
		for(size_t idx = 0;idx <arrsize;idx += jump){
			arr[idx] += 1;
		}
	}
	int rc2 = gettimeofday(&end,NULL);
	if(rc2 == -1){
		fprintf(stderr, "Error: second gettimeofday() failed\n");
		exit(1);
	}
	long usecs = 1000000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
	printf("%ld\n",usecs);
	return 0;
}