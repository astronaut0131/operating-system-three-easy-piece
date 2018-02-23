# Ch14 Homework
### Q1:First, write a simple program called null.c that creates a pointer to an integer, sets it to NULL, and then tries to dereference it. Com- pile this into an executable called null. What happens when you run this program?
A1:
```
#include <stdio.h>
int main(){
	int *p = NULL;
	int test = *p + 1;
	return 0;
}
```
Gives
```
[1]    77256 segmentation fault 
```

### Q2:Next, compile this program with symbol information included (with the -g flag). Doing so let’s put more information into the executable, enabling the debugger to access more useful information about variable names and the like. Run the program under the debugger by typing gdb null and then, once gdb is running, typing run. What does gdb show you?
A2:About Mac gdb Mach problem see:[解决GDB在Mac下不能调试的问题 - Joywek’s Blog - SegmentFault](https://segmentfault.com/a/1190000004136351)
```
➜  Ch14 Memory API gcc null.c -g -o null
➜  Ch14 Memory API gdb null
(gdb) run
```
Gives
```
Thread 3 received signal SIGSEGV, Segmentation fault.
0x0000000100000fa9 in main () at null.c:4
4		int test = *p + 1;
```
### Q3:Finally, use the valgrind tool on this program.We’ll use the memcheck tool that is a part of valgrind to analyze what happens. Run this by typing in the following: valgrind --leak-check=yes null. What happens when you run this? Can you interpret the output from the tool?
```

==6895== 4 bytes in 1 blocks are definitely lost in loss record 1 of 43
==6895==    at 0x100098616: malloc (in /usr/local/Cellar/valgrind/3.13.0/lib/valgrind/vgpreload_memcheck-amd64-darwin.so)
==6895==    by 0x100000F7A: main (null.c:8)
 
==6895== LEAK SUMMARY:
==6895==    definitely lost: 4 bytes in 1 blocks
==6895==    indirectly lost: 0 bytes in 0 blocks
==6895==      possibly lost: 72 bytes in 3 blocks
==6895==    still reachable: 200 bytes in 6 blocks
==6895==         suppressed: 17,837 bytes in 153 blocks
==6895== Reachable blocks (those to which a pointer was found) are not shown.
==6895== To see them, rerun with: --leak-check=full --show-leak-kinds=all
==6895== 
==6895== For counts of detected and suppressed errors, rerun with: -v
==6895== Use --track-origins=yes to see where uninitialised values come from
==6895== ERROR SUMMARY: 3 errors from 3 contexts (suppressed: 12 from 12)
```
I think `==6631==    definitely lost: 4 bytes in 1 blocks` is because int type takes 4 bytes.
### Q4:Write a simple program that allocates memory using malloc() but forgets to free it before exiting. What happens when this program runs? Can you use gdb to find any problems with it? How about valgrind (again with the --leak-check=yes flag)?
```
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
```

```
(gdb) run
Starting program: /Users/astronaut/Operating System Three Easy Pieces/Virtualization/Ch14 Memory API/null 
[New Thread 0x1403 of process 1049]
[New Thread 0x1503 of process 1049]
warning: unhandled dyld version (15)
[Inferior 1 (process 1049) exited normally]
```
Gdb runs the program normally.
With valgrind:
```

==7140== 40 bytes in 1 blocks are definitely lost in loss record 15 of 43
==7140==    at 0x100098616: malloc (in /usr/local/Cellar/valgrind/3.13.0/lib/valgrind/vgpreload_memcheck-amd64-darwin.so)
==7140==    by 0x100000F5A: main (null.c:8)

==7140== LEAK SUMMARY:
==7140==    definitely lost: 40 bytes in 1 blocks
==7140==    indirectly lost: 0 bytes in 0 blocks
==7140==      possibly lost: 72 bytes in 3 blocks
==7140==    still reachable: 200 bytes in 6 blocks
==7140==         suppressed: 17,837 bytes in 153 blocks
==7140== Reachable blocks (those to which a pointer was found) are not shown.
==7140== To see them, rerun with: --leak-check=full --show-leak-kinds=all
==7140== 
==7140== For counts of detected and suppressed errors, rerun with: -v
==7140== Use --track-origins=yes to see where uninitialised values come from
==7140== ERROR SUMMARY: 3 errors from 3 contexts (suppressed: 12 from 12)
```
It’s clear that 40 bytes leaks = 10 (array size)* 4 bytes(int variables)

### Q5:Write a program that creates an array of integers called data of size 100 using malloc; then, set data[100] to zero. What happens when you run this program? What happens when you run this program using valgrind? Is the program correct?
A5:
```
#include <stdio.h>
#include <stdlib.h>
int main(){
	int *data = (int *) malloc(100 * sizeof(int));
	*(data+100) = 0;
	return 0;
}
```
Nothing happened when I run it.
With valgrind:
```
==7352== Invalid write of size 4
==7352==    at 0x100000F75: main (in ./Q5)
==7352==  Address 0x100b57e30 is 0 bytes after a block of size 400 alloc'd
==7352==    at 0x100098616: malloc (in /usr/local/Cellar/valgrind/3.13.0/lib/valgrind/vgpreload_memcheck-amd64-darwin.so)
==7352==    by 0x100000F6A: main (in ./Q5)

==7352== LEAK SUMMARY:
==7352==    definitely lost: 400 bytes in 1 blocks
==7352==    indirectly lost: 0 bytes in 0 blocks
==7352==      possibly lost: 72 bytes in 3 blocks
==7352==    still reachable: 200 bytes in 6 blocks
==7352==         suppressed: 17,837 bytes in 153 blocks
==7352== Reachable blocks (those to which a pointer was found) are not shown.
==7352== To see them, rerun with: --leak-check=full --show-leak-kinds=all
==7352== 
==7352== For counts of detected and suppressed errors, rerun with: -v
==7352== Use --track-origins=yes to see where uninitialised values come from
==7352== ERROR SUMMARY: 4 errors from 4 contexts (suppressed: 12 from 12)
```
The program is not correct:1. The used memory is not freed, this will lead to memory leak. 2. data[0] to data[99] are not initiated.3. 100 is out of the array’s memory range(Invalid write of size 4).

### Q6:Create a program that allocates an array of integers(as above),frees them, and then tries to print the value of one of the elements of the array. Does the program run? What happens when you use valgrind on it?
A6:
```
#include <stdio.h>
#include <stdlib.h>
int main(){
	int *p = (int *) malloc(100*sizeof(int));
	for(int i=0;i<100;i++){
		*(p+i) = i;
	}
	free(p);
	printf("%d\n", *(p+10));
	return 0;
}
```
The program run normally.
Gives
```
10
```
With valgrind:
```
==8152== Invalid read of size 4
==8152==    at 0x100000F53: main (in ./Q6)
==8152==  Address 0x100b57cc8 is 40 bytes inside a block of size 400 free'd
==8152==    at 0x1000989F3: free (in /usr/local/Cellar/valgrind/3.13.0/lib/valgrind/vgpreload_memcheck-amd64-darwin.so)
==8152==    by 0x100000F47: main (in ./Q6)
==8152==  Block was alloc'd at
==8152==    at 0x100098616: malloc (in /usr/local/Cellar/valgrind/3.13.0/lib/valgrind/vgpreload_memcheck-amd64-darwin.so)
==8152==    by 0x100000F0A: main (in ./Q6)
```
Invalid read because the memory has been freed.

### Q7:Now pass a funny value to free (e.g., a pointer in the middle of the array you allocated above). What happens? Do you need tools to find this type of problem?
A7: run result:
```
Q7(8265,0x7fffb393a3c0) malloc: *** error for object 0x7faa26402738: pointer being freed was not allocated
*** set a breakpoint in malloc_error_break to debug
[1]    8265 abort  
```
Don’t need tools, its error is reflected in the run result with error message.

### Q8:Try out some of the other interfaces to memory allocation. For example, create a simple vector-like data structure and related routines that use realloc() to manage the vector. Use an array to store the vectors elements; when a user adds an entry to the vector, use realloc() to allocate more space for it. How well does such a vector perform? How does it compare to a linked list? Use valgrind to help you find bugs.
A8: good code from[ostep/8_realloc.c at master · asnr/ostep · GitHub](https://github.com/asnr/ostep/blob/master/virtualisation/14_memory_api/8_realloc.c)