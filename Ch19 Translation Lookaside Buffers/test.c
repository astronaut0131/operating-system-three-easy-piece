#include <stdio.h>
#include <sched.h>
#include <sys/time.h>
int main(){
    unsigned long mask = 1;
    if(sched_setaffinity(0,sizeof(mask),&mask) < 0)
    return 0;
}