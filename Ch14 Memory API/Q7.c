#include <stdio.h>
#include <stdlib.h>
int main(){
	int *p = (int *) malloc(100*sizeof(int));
	for(int i=0;i<100;i++){
		*(p+i) = i;
	}
	free(p+10);
	printf("%d\n", *(p+10));
	return 0;
}