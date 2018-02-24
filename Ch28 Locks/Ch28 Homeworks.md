# Ch28 Homeworks
### Q1:First let’s get ready to run x86.py with the flag -p flag.s. This code “implements” locking with a single memory flag. Can you understand what the assembly code is trying to do?
A1: 
```
       Thread 0                Thread 1         

1000 mov  flag, %ax // move flag to %ax
1001 test $0, %ax // test if flag == 0 (whether the lock is available)
1002 jne  .acquire // if flag != 0, the lock is already held
1003 mov  $1, flag // set flag = 1 (hold the lock)
1004 mov  count, %ax //critical section, increment of count by 1
1005 add  $1, %ax
1006 mov  %ax, count
1007 mov  $0, flag // set flag = 0 (release the lock)
1008 sub  $1, %bx // loop
1009 test $0, %bx
1010 jgt .top
1011 halt
//thread 1 works after thread 0's completion, and there is no race condition
----- Halt;Switch -----  ----- Halt;Switch -----  
                         1000 mov  flag, %ax 
                         1001 test $0, %ax
                         1002 jne  .acquire
                         1003 mov  $1, flag
                         1004 mov  count, %ax
                         1005 add  $1, %ax
                         1006 mov  %ax, count
                         1007 mov  $0, flag
                         1008 sub  $1, %bx
                         1009 test $0, %bx
                         1010 jgt .top
                         1011 halt
```

### Q2:When you run with the defaults, does flag.s work as expected? Does it produce the correct result? Use the -M and -R flags to trace variables and registers (and turn on -c to see their values). Can you predict what value will end up in flag as the code runs?
A2: the default `flag.s` gives the correct result and works as expected. Count will end up in 2 because both thread 0 and thread 1 increase it correctly.
`➜  HW-ThreadsLocks python x86.py -p flag.s -M count -R ax,bx -c`
gives
```
count      ax    bx          Thread 0                Thread 1         

    0       0     0   
    0       0     0   1000 mov  flag, %ax
    0       0     0   1001 test $0, %ax
    0       0     0   1002 jne  .acquire
    0       0     0   1003 mov  $1, flag
    0       0     0   1004 mov  count, %ax
    0       1     0   1005 add  $1, %ax
    1       1     0   1006 mov  %ax, count
    1       1     0   1007 mov  $0, flag
    1       1    -1   1008 sub  $1, %bx
    1       1    -1   1009 test $0, %bx
    1       1    -1   1010 jgt .top
    1       1    -1   1011 halt
    1       0     0   ----- Halt;Switch -----  ----- Halt;Switch -----  
    1       0     0                            1000 mov  flag, %ax
    1       0     0                            1001 test $0, %ax
    1       0     0                            1002 jne  .acquire
    1       0     0                            1003 mov  $1, flag
    1       1     0                            1004 mov  count, %ax
    1       2     0                            1005 add  $1, %ax
    2       2     0                            1006 mov  %ax, count
    2       2     0                            1007 mov  $0, flag
    2       2    -1                            1008 sub  $1, %bx
    2       2    -1                            1009 test $0, %bx
    2       2    -1                            1010 jgt .top
    2       2    -1                            1011 halt
```
### Q3:Change the value of the register %bx with the -a flag (e.g., -a bx=2,bx=2 if you are running just two threads). What does the code do? How does it change your answer for the question above?
A3:
```
       Thread 0                Thread 1         

1000 mov  flag, %ax
1001 test $0, %ax
1002 jne  .acquire
1003 mov  $1, flag
1004 mov  count, %ax
1005 add  $1, %ax
1006 mov  %ax, count
1007 mov  $0, flag
1008 sub  $1, %bx
1009 test $0, %bx
1010 jgt .top
1000 mov  flag, %ax
1001 test $0, %ax
1002 jne  .acquire
1003 mov  $1, flag
1004 mov  count, %ax
1005 add  $1, %ax
1006 mov  %ax, count
1007 mov  $0, flag
1008 sub  $1, %bx
1009 test $0, %bx
1010 jgt .top
1011 halt
----- Halt;Switch -----  ----- Halt;Switch -----  
                         1000 mov  flag, %ax
                         1001 test $0, %ax
                         1002 jne  .acquire
                         1003 mov  $1, flag
                         1004 mov  count, %ax
                         1005 add  $1, %ax
                         1006 mov  %ax, count
                         1007 mov  $0, flag
                         1008 sub  $1, %bx
                         1009 test $0, %bx
                         1010 jgt .top
                         1000 mov  flag, %ax
                         1001 test $0, %ax
                         1002 jne  .acquire
                         1003 mov  $1, flag
                         1004 mov  count, %ax
                         1005 add  $1, %ax
                         1006 mov  %ax, count
                         1007 mov  $0, flag
                         1008 sub  $1, %bx
                         1009 test $0, %bx
                         1010 jgt .top
                         1011 halt

```
There are 2 times of loop to increase value of count for both thread 0 and thread 1, so the final value of count will be 4.
It can be verified by 
`➜  HW-ThreadsLocks python x86.py -p flag.s -a bx=2,bx=2 -M count c`

### Q4:Set bx to a high value for each thread, and then use the -i flag to generate different interrupt frequencies; what values lead to a bad outcomes? Which lead to good outcomes?
A4: If interrupt happens after 1001 or 1002(the flag of the held lock has not been set to 1 yet), both threads will hold the lock and cause race condition.
An example
`➜  HW-ThreadsLocks python x86.py -p flag.s -a bx=10,bx=10 -i 2 -M count -c`
Should give 20 but outputs 10, it’s because there are 12 lines of code in total, 12 can be divided by 2, so we will cause a race condition in each loop.

### Q5:Now let’s look at the program test-and-set.s. First, try to understand the code, which uses the xchg instruction to build a simple locking primitive. How is the lock acquire written? How about lock release?
A5:
```
➜  HW-ThreadsLocks cat test-and-set.s
.var mutex
.var count

.main
.top	

.acquire
mov  $1, %ax        
xchg %ax, mutex     # atomic swap of 1 and mutex
test $0, %ax        # if we get 0 back: lock is free!
jne  .acquire       # if not, try again

# critical section
mov  count, %ax     # get the value at the address
add  $1, %ax        # increment it
mov  %ax, count     # store it back

# release lock
mov  $0, mutex

# see if we're still looping
sub  $1, %bx
test $0, %bx
jgt .top	

halt

```
Lock acquire code is in .acquire and lock release code is after # release lock
A thread acquire the lock if mutex == 0 and atomically set mutex = 1 after holding the lock, release the lock just set mutex back to 1.This way, there will be no race condition like the one in Q4, in that test and set is done atomically.

### Q6:Now run the code, changing the value of the interrupt interval(-i) again, and making sure to loop for a number of times. Does the code always work as expected? Does it sometimes lead to an inefficient use of the CPU? How could you quantify that?
A6:
To test it, we can use a basic command 
`python x86.py -p test-and-set.s -a bx=10,bx=10 -M count -c`
And -i ranging from 1 to 12 to test whether the final value of count is always 20
Write a python script to do it.
```
import os
for i in range(1,13):
	command = 'python x86.py -p test-and-set.s -a bx=10,bx=10 -M count -c -i ' +str(i)
	output = os.popen(command).read().strip().split('\n')
	print(output[-1].split()[0])
```
Gives:
```
20
20
20
20
20
20
20
20
20
20
20
20
```
So it always gives the correct answer.
 I think the quantification of CPU efficiency can be the turnaround time of the critical section.
We can imagine that frequent interrupt in the execution of the critical section will lead the CPU to execute continuously checking whether the lock is held for other waiting threads, and there will be severe inefficiency especially when there is a single CPU,a lot of waiting threads and a critical section of much code, if the system use a round-robin schedule, a lot of CPU time will spent for the waiting threads to check whether the lock has been released rather than execute the critical section, the turnaround time of the critical section should be very long.

### Q7:Use the -P flag to generate specific tests of the locking code. For example, run a schedule that grabs the lock in the first thread, but then tries to acquire it in the second. Does the right thing happen? What else should you test?
A7: the right thing happened.
Test by `python x86.py -p test-and-set.s -a bx=1,bx=1 -M count -c -P 000011111`
```
count          Thread 0                Thread 1         

    0   
    0   1000 mov  $1, %ax
    0   1001 xchg %ax, mutex
    0   1002 test $0, %ax
    0   1003 jne  .acquire
    0   ------ Interrupt ------  ------ Interrupt ------  
    0                            1000 mov  $1, %ax
    0                            1001 xchg %ax, mutex
    0                            1002 test $0, %ax
    0                            1003 jne  .acquire
    0                            1000 mov  $1, %ax
```
Should also test whether there is a race condition as  `flag.s` , or whether `xchg` is atomic
Test by `python x86.py -p test-and-set.s -a bx=1,bx=1 -M count -c -P 0011`
Gives
```
count          Thread 0                Thread 1         

    0   
    0   1000 mov  $1, %ax
    0   1001 xchg %ax, mutex
    0   ------ Interrupt ------  ------ Interrupt ------  
    0                            1000 mov  $1, %ax
    0                            1001 xchg %ax, mutex
    0   ------ Interrupt ------  ------ Interrupt ------  
    0   1002 test $0, %ax
    0   1003 jne  .acquire
    0   ------ Interrupt ------  ------ Interrupt ------  
    0                            1002 test $0, %ax
    0                            1003 jne  .acquire
    0   ------ Interrupt ------  ------ Interrupt ------  
    0   1004 mov  count, %ax
    0   1005 add  $1, %ax
    0   ------ Interrupt ------  ------ Interrupt ------  
    0                            1000 mov  $1, %ax
    0                            1001 xchg %ax, mutex
```

### Q8:Now let’s look at the code in peterson.s, which implements Peterson’s algorithm (mentioned in a sidebar in the text). Study the code and see if you can make sense of it.
A8:
```
# array of 2 integers (each size 4 bytes)
# load address of flag into fx register
# access flag[] with 0(%fx,%index,4)
# where %index is a register holding 0 or 1
# index reg contains 0 -> flag[0], if 1->flag[1]
.var flag   2     

# global turn variable
.var turn

# global count
.var count

.main

# put address of flag into fx
lea flag, %fx

# assume thread ID is in bx (0 or 1, scale by 4 to get proper flag address)
mov %bx, %cx   # bx: self, now copies to cx
neg %cx        # cx: - self
add $1, %cx    # cx: 1 - self

.acquire
mov $1, 0(%fx,%bx,4)    # flag[self] = 1
mov %cx, turn           # turn       = 1 - self

.spin1
mov 0(%fx,%cx,4), %ax   # flag[1-self]
test $1, %ax            
jne .fini               # if flag[1-self] != 1, skip past loop to .fini

.spin2                  # just labeled for fun, not needed
mov turn, %ax
test %cx, %ax           # compare 'turn' and '1 - self'
je .spin1               # if turn==1-self, go back and start spin again

# fall out of spin
.fini

# do critical section now
mov count, %ax
add $1, %ax
mov %ax, count

.release
mov $0, 0(%fx,%bx,4)    # flag[self] = 0


# end case: make sure it's other's turn
mov %cx, turn           # turn       = 1 - self
halt
```
Simulate by thread 0 and thread 1:
Say thread 0 called lock() first
flag[0] = 1
turn = 1 - 0 = 1
in that flag[1-self] i.e. flag[1] = 0 != 1(while condition was broken), thread 0 holds the key
Then thread 1 called lock()
flag[1] = 1
turn = 1 - 1 = 0
in that flag[1-self] i.e. flag[0] = 1 and turn = 1-self = 1-1 == 0 (while condition was always true)
Thread 1 started spinning waiting for thread 0
Thread 0 finished the critical section and turn = 1-0 = 1, so that thread 1 stop spinning and get the lock.

### Q9:Now run the code with different values of -i. What kinds of different behavior do you see? Make sure to set the thread IDs appropriately (using -a bx=0,bx=1 for example) as the code assumes it. A9: the code can always give correct answer
Test by `python x86.py -p peterson.s -a bx=0,bx=1 -c -M count` and different -i

### Q10:Can you control the scheduling (with the -P flag) to “prove” that the code works? What are the different cases you should show hold? Think about mutual exclusion and deadlock avoidance.
A10:
To prove the code works, use
`python x86.py -p peterson.s -a bx=0,bx=1 -c -M count -P 0000000001111111111111`
Which gives:
```
count          Thread 0                Thread 1         

    0   
    0   1000 lea flag, %fx
    0   1001 mov %bx, %cx
    0   1002 neg %cx
    0   1003 add $1, %cx
    0   1004 mov $1, 0(%fx,%bx,4)
    0   1005 mov %cx, turn
    0   1006 mov 0(%fx,%cx,4), %ax
    0   1007 test $1, %ax
    0   1008 jne .fini
    0   ------ Interrupt ------  ------ Interrupt ------  
    0                            1000 lea flag, %fx
    0                            1001 mov %bx, %cx
    0                            1002 neg %cx
    0                            1003 add $1, %cx
    0                            1004 mov $1, 0(%fx,%bx,4)
    0                            1005 mov %cx, turn
    0                            1006 mov 0(%fx,%cx,4), %ax
    0                            1007 test $1, %ax
    0                            1008 jne .fini
    0                            1009 mov turn, %ax
    0                            1010 test %cx, %ax
    0                            1011 je .spin1
    0                            1006 mov 0(%fx,%cx,4), %ax // back to .spin1
```
Thread 1 tried to get the lock after thread 0 had already held and failed (it got back to .spin1 after the while condition test).
Don’t know answer to the other questions in Q10 currently.

### Q11:Now study the code for the ticket lock in ticket.s. Does it match the code in the chapter?
A11:
```
.var ticket
.var turn
.var count

.main
.top	

.acquire
mov $1, %ax
fetchadd %ax, ticket  # grab a ticket 
.tryagain
mov turn, %cx         # check if it's your turn 
test %cx, %ax
jne .tryagain

# critical section
mov  count, %ax       # get the value at the address
add  $1, %ax          # increment it
mov  %ax, count       # store it back

# release lock
mov $1, %ax
fetchadd %ax, turn

# see if we're still looping
sub  $1, %bx
test $0, %bx
jgt .top	

halt
```
It matches with the code in the chapter
```
int FetchAndAdd(int *ptr) {
		int old = *ptr;
		*ptr = old + 1;
		return old;
}
typedef struct __lock_t {
		int ticket;
		int turn;
} lock_t;
void lock_init(lock_t *lock) {
		lock->ticket = 0;
		lock->turn   = 0;
}
void lock(lock_t *lock){
		int myturn = FetchAndAdd(&lock->ticket);
		while (lock->turn != myturn)
			;//spin
}
void unlock(lock_t *lock){
		lock->turn = lock->turn + 1;
}
```

### Q12:Now run the code, with the following flags:-a bx=1000, bx=1000 (this flag sets each thread to loop through the critical 1000 times). Watch what happens over time; do the threads spend much time spinning waiting for the lock?
A12: It does give the correct result, but it spent much time executing whether the lock has been released for the waiting thread.

### Q13: How does the code behave as you add more threads?
A13: the code will behave even worse especially under CPU scheduler like RR scheduler and with a single CPU core, every time the thread holding the lock executes the critical section, all of the rest waiting thread also have to execute their checking of whether the lock has been released. 

### Q14:Now examine yield.s, in which we pretend that a yield instruction enables one thread to yield control of the CPU to another (realistically, this would be an OS primitive, but for the simplicity of simulation, we assume there is an instruction that does the task). Find a scenario where test-and-set.s wastes cycles spinning, but yield.s does not. How many instructions are saved? In what scenarios do these savings arise?
A14: when the critical section have many instructions, and there is a single CPU core, `test-and-set.s` always wastes time cycle spinning for all the waiting threads, but `yield.s` just waste once. Under the same condition, the longer the critical section is, the more savings `yield.s` will have.

### Q15:Finally, examine test-and-test-and-set.s. What does this lock do? What kind of savings does it introduce as compared to test-and-set.s?
A15:
code of `test-and-set.s`:
```
.var mutex
.var count

.main
.top	

.acquire
mov  $1, %ax        
xchg %ax, mutex     # atomic swap of 1 and mutex
test $0, %ax        # if we get 0 back: lock is free!
jne  .acquire       # if not, try again

# critical section
mov  count, %ax     # get the value at the address
add  $1, %ax        # increment it
mov  %ax, count     # store it back

# release lock
mov  $0, mutex

# see if we're still looping
sub  $1, %bx
test $0, %bx
jgt .top	

halt
```
code of  `test-and-test-and-set.s`:
```
.var mutex
.var count

.main
.top	

.acquire
mov  mutex, %ax
test $0, %ax
jne .acquire
mov  $1, %ax        
xchg %ax, mutex     # atomic swap of 1 and mutex
test $0, %ax        # if we get 0 back: lock is free!
jne .acquire        # if not, try again

# critical section
mov  count, %ax     # get the value at the address
add  $1, %ax        # increment it
mov  %ax, count     # store it back

# release lock
mov  $0, mutex

# see if we're still looping
sub  $1, %bx
test $0, %bx
jgt .top	

halt
```
We can see that the main difference is in .acquire
 `test-and-test-and-set.s` do test-and-set operation only when the mutex flag is 0 (the lock is available now) while `test-and-set.s` do test-and-set operation even if the lock is not available for the current time.