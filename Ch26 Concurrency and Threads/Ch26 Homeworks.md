# Ch26 Homeworks
### Q1:To start, let’s examine a simple program, “loop.s”. First, just look at the program, and see if you can understand it: cat loop.s. Then, run it with these arguments:
`./x86.py -p loop.s -t 1 -i 100 -R dx`
### This specifies a single thread, an interrupt every 100 instructions, and tracing of register %dx. Can you figure out what the value of %dx will be during the run? Once you have, run the same above and use the -c flag to check your answers; note the answers, on the left, show the value of the register (or memory value) after the instruction on the right has run.
A1:
```
   dx          Thread 0         
    0   
    -1   1000 sub  $1,%dx
    -1   1001 test $0,%dx
    -1   1002 jgte .top
    -1   1003 halt
```

### Q2：Now run the same code but with these flags:
`./x86.py -p loop.s -t 2 -i 100 -a dx=3,dx=3 -R dx`
### This specifies two threads, and initializes each %dx register to 3. What values will %dx see? Run with the -c flag to see the answers. Does the presence of multiple threads affect anything about your calculations? Is there a race condition in this code?
A2:
```
   dx          Thread 0                Thread 1         
    3   
    2   1000 sub  $1,%dx
    2   1001 test $0,%dx
    2   1002 jgte .top
    1   1000 sub  $1,%dx
    1   1001 test $0,%dx
    1   1002 jgte .top
    0   1000 sub  $1,%dx
    0   1001 test $0,%dx
    0   1002 jgte .top
    -1   1000 sub  $1,%dx
    -1   1001 test $0,%dx
    -1   1002 jgte .top
    -1   1003 halt
    3   ----- Halt;Switch -----  ----- Halt;Switch -----  
    2                            1000 sub  $1,%dx
    2                            1001 test $0,%dx
    2                            1002 jgte .top
    1                            1000 sub  $1,%dx
    1                            1001 test $0,%dx
    1                            1002 jgte .top
    0                            1000 sub  $1,%dx
    0                            1001 test $0,%dx
    0                            1002 jgte .top
    -1                            1000 sub  $1,%dx
    -1                            1001 test $0,%dx
    -1                            1002 jgte .top
    -1                            1003 halt
```
There’s no race condition because the two threads are not trying to update a common global variable at the same time.

### Now run the following:
`./x86.py -p loop.s -t 2 -i 3 -r -a dx=3,dx=3 -R dx`
### This makes the interrupt interval quite small and random; use different seeds with -s to see different interleavings. Does the frequency of interruption change anything about this program?
A3: the frequency of interruption changes nothing about this program, because the two thread have their independent %dx register.

### Q4:Next we’ll examine a different program(looping-race-nolock.s). This program accesses a shared variable located at memory address 2000; we’ll call this variable x for simplicity. Run it with a single thread and make sure you understand what it does, like this:
`./x86.py -p looping-race-nolock.s -t 1 -M 2000`
### What value is found in x (i.e., at memory address 2000) throughout the run? Use -c to check your answer.
A4:
```
 2000          Thread 0         
    0   
    0   1000 mov 2000, %ax
    0   1001 add $1, %ax
    1   1002 mov %ax, 2000
    1   1003 sub  $1, %bx
    1   1004 test $0, %bx
    1   1005 jgt .top
    1   1006 halt
```

### Q5:Now run with multiple iterations and threads:
`./x86.py -p looping-race-nolock.s -t 2 -a bx=3 -M 2000`
### Do you understand why the code in each thread loops three times? What will the final value of x be?
A5: %bx is initiated to be 3,  according to 
 `sub  $1, %bx` and `test $0, %bx`, it can be inferred that the code will loop  three times.
```
 2000          Thread 0                Thread 1         
    0   
    0   1000 mov 2000, %ax
    0   1001 add $1, %ax
    1   1002 mov %ax, 2000
    1   1003 sub  $1, %bx
    1   1004 test $0, %bx
    1   1005 jgt .top
    1   1000 mov 2000, %ax
    1   1001 add $1, %ax
    2   1002 mov %ax, 2000
    2   1003 sub  $1, %bx
    2   1004 test $0, %bx
    2   1005 jgt .top
    2   1000 mov 2000, %ax
    2   1001 add $1, %ax
    3   1002 mov %ax, 2000
    3   1003 sub  $1, %bx
    3   1004 test $0, %bx
    3   1005 jgt .top
    3   1006 halt
    3   ----- Halt;Switch -----  ----- Halt;Switch -----  
    3                            1000 mov 2000, %ax
    3                            1001 add $1, %ax
    4                            1002 mov %ax, 2000
    4                            1003 sub  $1, %bx
    4                            1004 test $0, %bx
    4                            1005 jgt .top
    4                            1000 mov 2000, %ax
    4                            1001 add $1, %ax
    5                            1002 mov %ax, 2000
    5                            1003 sub  $1, %bx
    5                            1004 test $0, %bx
    5                            1005 jgt .top
    5                            1000 mov 2000, %ax
    5                            1001 add $1, %ax
    6                            1002 mov %ax, 2000
    6                            1003 sub  $1, %bx
    6                            1004 test $0, %bx
    6                            1005 jgt .top
    6                            1006 halt
```

### Q6:Now run with random interrupt intervals:
`./x86.py -p looping-race-nolock.s -t 2 -M 2000 -i 4 -r -s 0`
### Then change the random seed, setting -s 1, then -s 2, etc. Can you tell, just by looking at the thread interleaving, what the final value of x will be? Does the exact location of the interrupt matter? Where can it safely occur? Where does an interrupt cause trouble? In other words, where is the critical section exactly?
A6:
```
 2000          Thread 0                Thread 1         
    0   
    0   1000 mov 2000, %ax
    0   1001 add $1, %ax
    1   1002 mov %ax, 2000
    1   1003 sub  $1, %bx
    1   ------ Interrupt ------  ------ Interrupt ------  
    1                            1000 mov 2000, %ax
    1                            1001 add $1, %ax
    2                            1002 mov %ax, 2000
    2                            1003 sub  $1, %bx
    2   ------ Interrupt ------  ------ Interrupt ------  
    2   1004 test $0, %bx
    2   1005 jgt .top
    2   ------ Interrupt ------  ------ Interrupt ------  
    2                            1004 test $0, %bx
    2                            1005 jgt .top
    2   ------ Interrupt ------  ------ Interrupt ------  
    2   1006 halt
    2   ----- Halt;Switch -----  ----- Halt;Switch -----  
    2                            1006 halt
```
The location of the interrupt matters.
the critical section is:
```
mov 2000, %ax
add $1,%ax
mov %ax, 2000
```
The interrupt should not split this, or it will cause trouble, making x a wrong value.

### Q7:Now use a fixed interrupt interval to explore the program further. Run:
`./x86.py -p looping-race-nolock.s -a bx=1 -t 2 -M 2000 -i 1`
### See if you can guess what the final value of the shared variable x will be. What about when you change -i 2, -i 3, etc.? For which interrupt intervals does the program give the “correct” final answer?
A7: run with -i 1
```
 2000          Thread 0                Thread 1         
    0   
    0   1000 mov 2000, %ax
    0   ------ Interrupt ------  ------ Interrupt ------  
    0                            1000 mov 2000, %ax
    0   ------ Interrupt ------  ------ Interrupt ------  
    0   1001 add $1, %ax
    0   ------ Interrupt ------  ------ Interrupt ------  
    0                            1001 add $1, %ax
    0   ------ Interrupt ------  ------ Interrupt ------  
    1   1002 mov %ax, 2000
    1   ------ Interrupt ------  ------ Interrupt ------  
    1                            1002 mov %ax, 2000
    1   ------ Interrupt ------  ------ Interrupt ------  
    1   1003 sub  $1, %bx
    1   ------ Interrupt ------  ------ Interrupt ------  
    1                            1003 sub  $1, %bx
    1   ------ Interrupt ------  ------ Interrupt ------  
    1   1004 test $0, %bx
    1   ------ Interrupt ------  ------ Interrupt ------  
    1                            1004 test $0, %bx
    1   ------ Interrupt ------  ------ Interrupt ------  
    1   1005 jgt .top
    1   ------ Interrupt ------  ------ Interrupt ------  
    1                            1005 jgt .top
    1   ------ Interrupt ------  ------ Interrupt ------  
    1   1006 halt
    1   ----- Halt;Switch -----  ----- Halt;Switch -----  
    1   ------ Interrupt ------  ------ Interrupt ------  
    1                            1006 halt
```
Run with -i 2
```
 2000          Thread 0                Thread 1         
    0   
    0   1000 mov 2000, %ax
    0   1001 add $1, %ax
    0   ------ Interrupt ------  ------ Interrupt ------  
    0                            1000 mov 2000, %ax
    0                            1001 add $1, %ax
    0   ------ Interrupt ------  ------ Interrupt ------  
    1   1002 mov %ax, 2000
    1   1003 sub  $1, %bx
    1   ------ Interrupt ------  ------ Interrupt ------  
    1                            1002 mov %ax, 2000
    1                            1003 sub  $1, %bx
    1   ------ Interrupt ------  ------ Interrupt ------  
    1   1004 test $0, %bx
    1   1005 jgt .top
    1   ------ Interrupt ------  ------ Interrupt ------  
    1                            1004 test $0, %bx
    1                            1005 jgt .top
    1   ------ Interrupt ------  ------ Interrupt ------  
    1   1006 halt
    1   ----- Halt;Switch -----  ----- Halt;Switch -----  
    1                            1006 halt
```
Run with -i 3
```
 2000          Thread 0                Thread 1         
    0   
    0   1000 mov 2000, %ax
    0   1001 add $1, %ax
    1   1002 mov %ax, 2000
    1   ------ Interrupt ------  ------ Interrupt ------  
    1                            1000 mov 2000, %ax
    1                            1001 add $1, %ax
    2                            1002 mov %ax, 2000
    2   ------ Interrupt ------  ------ Interrupt ------  
    2   1003 sub  $1, %bx
    2   1004 test $0, %bx
    2   1005 jgt .top
    2   ------ Interrupt ------  ------ Interrupt ------  
    2                            1003 sub  $1, %bx
    2                            1004 test $0, %bx
    2                            1005 jgt .top
    2   ------ Interrupt ------  ------ Interrupt ------  
    2   1006 halt
    2   ----- Halt;Switch -----  ----- Halt;Switch -----  
    2                            1006 halt
```
Run with -i 3 will give the correct answer.

### Q8:Now run the same code for more loops(e.g.,set -a bx=100).What interrupt intervals, set with the -i flag, lead to a “correct” outcome? Which intervals lead to surprising results?
A8: interrupt intervals >= 3 lead to a correct outcome 200 while 1 and 2 give the wrong result x=100 with -a bx=100.

### Q9:We’ll examine one last program in this homework(wait-for-me.s). Run the code like this:
`./x86.py -p wait-for-me.s -a ax=1,ax=0 -R ax -M 2000`
### This sets the %ax register to 1 for thread 0, and 0 for thread 1, and watches the value of %ax and memory location 2000 throughout the run. How should the code behave? How is the value at location 2000 being used by the threads? What will its final value be?
A9:
The source code of wait-for-me.s:
```
.main
test $1, %ax     # ax should be 1 (signaller) or 0 (waiter)
je .signaller

.waiter	
mov  2000, %cx
test $1, %cx
jne .waiter
halt

.signaller
mov  $1, 2000
halt
```

```
 2000      ax          Thread 0                Thread 1         
    0       1   
    0       1   1000 test $1, %ax
    0       1   1001 je .signaller
    1       1   1006 mov  $1, 2000
    1       1   1007 halt
    1       0   ----- Halt;Switch -----  ----- Halt;Switch -----  
    1       0                            1000 test $1, %ax
    1       0                            1001 je .signaller
    1       0                            1002 mov  2000, %cx
    1       0                            1003 test $1, %cx
    1       0                            1004 jne .waiter
    1       0                            1005 halt
```
In thread 0, %ax = 1 and leads to execute .signaller while in thread 1, %ax = 0 and leads to execute .waiter.

### Q10:Now switch the inputs:
`./x86.py -p wait-for-me.s -a ax=0,ax=1 -R ax -M 2000`
### How do the threads behave? What is thread 0 doing? How would changing the interrupt interval (e.g., -i 1000, or perhaps to use random intervals) change the trace outcome? Is the program efficiently using the CPU?
A10:
```
 2000      ax          Thread 0                Thread 1         
    ?       ?   
    ?       ?   1000 test $1, %ax
    ?       ?   1001 je .signaller
    ?       ?   1002 mov  2000, %cx
    ?       ?   1003 test $1, %cx
    ?       ?   1004 jne .waiter
    ?       ?   1002 mov  2000, %cx
    ?       ?   1003 test $1, %cx
    ?       ?   1004 jne .waiter
    ?       ?   1002 mov  2000, %cx
    ?       ?   1003 test $1, %cx
    ?       ?   1004 jne .waiter
    ?       ?   1002 mov  2000, %cx
    ?       ?   1003 test $1, %cx
    ?       ?   1004 jne .waiter
    ?       ?   1002 mov  2000, %cx
    ?       ?   1003 test $1, %cx
    ?       ?   1004 jne .waiter
    ?       ?   1002 mov  2000, %cx
    ?       ?   1003 test $1, %cx
    ?       ?   1004 jne .waiter
    ?       ?   1002 mov  2000, %cx
    ?       ?   1003 test $1, %cx
    ?       ?   1004 jne .waiter
    ?       ?   1002 mov  2000, %cx
    ?       ?   1003 test $1, %cx
    ?       ?   1004 jne .waiter
    ?       ?   1002 mov  2000, %cx
    ?       ?   1003 test $1, %cx
    ?       ?   1004 jne .waiter
    ?       ?   1002 mov  2000, %cx
    ?       ?   1003 test $1, %cx
    ?       ?   1004 jne .waiter
    ?       ?   1002 mov  2000, %cx
    ?       ?   1003 test $1, %cx
    ?       ?   1004 jne .waiter
    ?       ?   1002 mov  2000, %cx
    ?       ?   1003 test $1, %cx
    ?       ?   1004 jne .waiter
    ?       ?   1002 mov  2000, %cx
    ?       ?   1003 test $1, %cx
    ?       ?   1004 jne .waiter
    ?       ?   1002 mov  2000, %cx
    ?       ?   1003 test $1, %cx
    ?       ?   1004 jne .waiter
    ?       ?   1002 mov  2000, %cx
    ?       ?   1003 test $1, %cx
    ?       ?   1004 jne .waiter
    ?       ?   1002 mov  2000, %cx
    ?       ?   1003 test $1, %cx
    ?       ?   1004 jne .waiter
    ?       ?   ------ Interrupt ------  ------ Interrupt ------  
    ?       ?                            1000 test $1, %ax
    ?       ?                            1001 je .signaller
    ?       ?                            1006 mov  $1, 2000
    ?       ?                            1007 halt
    ?       ?   ----- Halt;Switch -----  ----- Halt;Switch -----  
    ?       ?   1002 mov  2000, %cx
    ?       ?   1003 test $1, %cx
    ?       ?   1004 jne .waiter
    ?       ?   1005 halt
```
A10: thread 0 is in a endless loop until the interrupt. The interrupt interval determines when the loop in thread 0 will be terminated. This program is not efficiently using CPU because thread 0 is looping endlessly until the interrupt, and if there is no interrupt thread 1 will starve and thread 0 will never get out of the loop.
