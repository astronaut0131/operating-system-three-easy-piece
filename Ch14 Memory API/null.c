#include <stdio.h>
#include <stdlib.h>
int main(){
	/* code of Q1 and Q2
	int *p = NULL;
	int test = *p + 1;
	*/
	int *p = (int *)malloc(10*sizeof(int));
	for(int i=0;i<10;i++){
		*(p+i) = i;
	}
	return 0;
}