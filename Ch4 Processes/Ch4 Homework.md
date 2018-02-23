# Ch4 Homework
Important scheduler information in README file:
System will switch when the current process is FINISHED or ISSUES AN IO
after IOs, the process issuing the IO will run LATER (when it is its turn)

* 1.Q:Run the program with the following flags: ./process-run.py -l 5:100,5:100. What should the CPU utilization be (e.g., the percent of time the CPU is in use?) Why do you know this? Use the -c and -p flags to see if you were right.

A:According to the README file,5:100 stands for a process that has 5 instructions and 100% of instructions use CPU.There is no I/O requests, the CPU first executes instructions of process A, and meantime process B is in Ready state and waiting process A to be done, once all the instructions of process A are done, it continues to executes all instructions of process B, so the CPU is in use all the time during the execution of the two process, i.e. 100% CPU utilization.
```
➜  HW-CPU-Intro python process-run.py -l 5:100,5:100 -c -p
Time     PID: 0     PID: 1        CPU        IOs 
  1     RUN:cpu      READY          1            
  2     RUN:cpu      READY          1            
  3     RUN:cpu      READY          1            
  4     RUN:cpu      READY          1            
  5     RUN:cpu      READY          1            
  6        DONE    RUN:cpu          1            
  7        DONE    RUN:cpu          1            
  8        DONE    RUN:cpu          1            
  9        DONE    RUN:cpu          1            
 10        DONE    RUN:cpu          1            

Stats: Total Time 10
Stats: CPU Busy 10 (100.00%)
Stats: IO Busy  0 (0.00%)

```

* 2.Q:Now run with these flags: ._process-run.py -l 4:100,1:0. These flags specify one process with 4 instructions (all to use the CPU), and one that simply issues an I_O and waits for it to be done. How long does it take to complete both processes? Use -c and -p to find out if you were right.

A: 4:100 stands for a process with 4 instructions that use CPU, and 1:0 stands for a process that has 1 instruction and this instruction issues an I/O,
According to the README file default (without specifying the -L flag) I/O completion takes 5 ticks, and 1 tick is needed to start IO using CPU,  CPU is not used by this process during the rest 4 ticks, waiting IO to finish.So in this case, to finish the first process, the CPU must keep running for 4 ticks, and once finished, it continue to start the IO, which takes 1 tick, and the CPU is unused when waiting for IO completion in the rest 4 ticks.Obviously, it takes 9 ticks in all and the CPU is in use for 5 ticks, but according to the README file, when there is an IO request, an empty tick at which all the job is done must be added to the end, so the program will get 10 ticks.
```
➜  HW-CPU-Intro python process-run.py -l 4:100,1:0 -c -p
Time     PID: 0     PID: 1        CPU        IOs 
  1     RUN:cpu      READY          1            
  2     RUN:cpu      READY          1            
  3     RUN:cpu      READY          1            
  4     RUN:cpu      READY          1            
  5        DONE     RUN:io          1            
  6        DONE    WAITING                     1 
  7        DONE    WAITING                     1 
  8        DONE    WAITING                     1 
  9        DONE    WAITING                     1 
 10*       DONE       DONE                       

Stats: Total Time 10
Stats: CPU Busy 5 (50.00%)
Stats: IO Busy  4 (40.00%)
```

* 3.Q:Now switch the order of the processes: ./process-run.py -l 1:0,4:100. What happens now? Does switching the order matter? Why? (As always, use -c and -p to see if you were right)

A:It matters for sure because the cpu can be used by process 4:100 during the IO job.So 1 tick to start IO, and at the beginning of the second tick, the scheduler switch the CPU usage to process 4:100, both process 4:100 and the IO job takes 4 ticks and get finished.Adding the empty DONE tick, there are 6 ticks and CPU in usage for 5 ticks.
```
➜  HW-CPU-Intro python process-run.py -l 1:0,4:100 -c -p
Time     PID: 0     PID: 1        CPU        IOs 
  1      RUN:io      READY          1            
  2     WAITING    RUN:cpu          1          1 
  3     WAITING    RUN:cpu          1          1 
  4     WAITING    RUN:cpu          1          1 
  5     WAITING    RUN:cpu          1          1 
  6*       DONE       DONE                       

Stats: Total Time 6
Stats: CPU Busy 5 (83.33%)
Stats: IO Busy  4 (66.67%)

```

* 4.Q:We’ll now explore some of the other flags.One important flag is -S, which determines how the system reacts when a process issues an I_O. With the flag set to SWITCH ON END, the system will NOT switch to another process while one is doing I_O, instead waiting until the process is completely finished. What happens when you run the following two processes, one doing I/O and the other doing CPU work? (-l 1:0,4:100 -c -S SWITCH ON END)

A:1 tick using CPU to start IO, and this time, process 4:100 can’t use CPU during the coming 4 ticks because of the SWITCH ON END, so 4 ticks IO busy without CPU busy, after that, process 4:100 gets its turn and keeps the CPU busy for 4 ticks.Adding 1 empty tick, there are 10 ticks and CPU is busy for 5 ticks.
```
➜  HW-CPU-Intro python process-run.py -l 1:0,4:100 -c -S SWITCH_ON_END -p -c
Time     PID: 0     PID: 1        CPU        IOs 
  1      RUN:io      READY          1            
  2     WAITING      READY                     1 
  3     WAITING      READY                     1 
  4     WAITING      READY                     1 
  5     WAITING      READY                     1 
  6*       DONE    RUN:cpu          1            
  7        DONE    RUN:cpu          1            
  8        DONE    RUN:cpu          1            
  9        DONE    RUN:cpu          1            

Stats: Total Time 9
Stats: CPU Busy 5 (55.56%)
Stats: IO Busy  4 (44.44%)
```
Wow, looks like when the IO completion is not at the last position, there is no empty tick, so there are 9 ticks in all.

* 5.Q:Now, run the same processes, but with the switching behavior set to switch to another process whenever one is WAITING for I/O (-l 1:0,4:100 -c -S SWITCH ON IO).What happens now?Use-c and -p to confirm that you are right.

A:Obviously, it’s exactly the same situation of Question 3 because the scheduler switch on IO by default.
```
➜  HW-CPU-Intro python process-run.py -l 1:0,4:100 -c -S SWITCH_ON_IO -c -p 
Time     PID: 0     PID: 1        CPU        IOs 
  1      RUN:io      READY          1            
  2     WAITING    RUN:cpu          1          1 
  3     WAITING    RUN:cpu          1          1 
  4     WAITING    RUN:cpu          1          1 
  5     WAITING    RUN:cpu          1          1 
  6*       DONE       DONE                       

Stats: Total Time 6
Stats: CPU Busy 5 (83.33%)
Stats: IO Busy  4 (66.67%)

```

* 6.Q:One other important behavior is what to do when an I_O completes. With -I IO RUN LATER, when an I_O completes, the process that issued it is not necessarily run right away; rather, what ever was running at the time keeps running. What happens when you run this combination of processes? (./process-run.py -l 3:0,5:100,5:100,5:100 -S SWITCH ON IO -I IO RUN LATER -c -p)Are system resources being effectively utilized?

A: I’m not pretty sure about what will happen because the question only says that 
“when an I/O completes, the process that issued it is not necessarily run right away; rather, what ever was running at the time keeps running.” It’s vague what the scheduler will do when “what ever was running” finishes.In other words, in this case, whether Process 0 will continue or Process 2 will start when Process 1 finishes.So let’s see what will happen.
```
➜  HW-CPU-Intro python process-run.py -l 3:0,5:100,5:100,5:100 -S SWITCH_ON_IO -c -p -I IO_RUN_LATER
Time     PID: 0     PID: 1     PID: 2     PID: 3        CPU        IOs 
  1      RUN:io      READY      READY      READY          1            
  2     WAITING    RUN:cpu      READY      READY          1          1 
  3     WAITING    RUN:cpu      READY      READY          1          1 
  4     WAITING    RUN:cpu      READY      READY          1          1 
  5     WAITING    RUN:cpu      READY      READY          1          1 
  6*      READY    RUN:cpu      READY      READY          1            
  7       READY       DONE    RUN:cpu      READY          1            
  8       READY       DONE    RUN:cpu      READY          1            
  9       READY       DONE    RUN:cpu      READY          1            
 10       READY       DONE    RUN:cpu      READY          1            
 11       READY       DONE    RUN:cpu      READY          1            
 12       READY       DONE       DONE    RUN:cpu          1            
 13       READY       DONE       DONE    RUN:cpu          1            
 14       READY       DONE       DONE    RUN:cpu          1            
 15       READY       DONE       DONE    RUN:cpu          1            
 16       READY       DONE       DONE    RUN:cpu          1            
 17      RUN:io       DONE       DONE       DONE          1            
 18     WAITING       DONE       DONE       DONE                     1 
 19     WAITING       DONE       DONE       DONE                     1 
 20     WAITING       DONE       DONE       DONE                     1 
 21     WAITING       DONE       DONE       DONE                     1 
 22*     RUN:io       DONE       DONE       DONE          1            
 23     WAITING       DONE       DONE       DONE                     1 
 24     WAITING       DONE       DONE       DONE                     1 
 25     WAITING       DONE       DONE       DONE                     1 
 26     WAITING       DONE       DONE       DONE                     1 
 27*       DONE       DONE       DONE       DONE                       

Stats: Total Time 27
Stats: CPU Busy 18 (66.67%)
Stats: IO Busy  12 (44.44%)
```
Looks like the process with no IO has a higher priority than process 0 when “what ever was running” finishes, there is nothing special about the result other than this unclear point.

* 7.Q:Now run the same processes, but with -I IO RUN IMMEDIATE set, which immediately runs the process that issued the I_O. How does this behavior differ? Why might running a process that just completed an I_O again be a good idea?

A:When the IO completes,  the scheduler will switch back to the process that initiates the IO right now.So the first five ticks is the same with the situation of the previous case.But on the sixth tick,  the Process 0 will continue to run the next instruction of starting IO,Process 1 is not finished, it’s suspended and get into ready state, waiting the next chance to run.One tick later, the IO start complete, Process 0 is waiting IO completion, so Process 1 use this tick to finish its last instruction.With such analysis, we can easily know what will go on later.
```
➜  HW-CPU-Intro python process-run.py -l 3:0,5:100,5:100 -S SWITCH_ON_IO -c -p -I IO_RUN_IMMEDIATE
Time     PID: 0     PID: 1     PID: 2        CPU        IOs 
  1      RUN:io      READY      READY          1            
  2     WAITING    RUN:cpu      READY          1          1 
  3     WAITING    RUN:cpu      READY          1          1 
  4     WAITING    RUN:cpu      READY          1          1 
  5     WAITING    RUN:cpu      READY          1          1 
  6*     RUN:io      READY      READY          1            
  7     WAITING    RUN:cpu      READY          1          1 
  8     WAITING       DONE    RUN:cpu          1          1 
  9     WAITING       DONE    RUN:cpu          1          1 
 10     WAITING       DONE    RUN:cpu          1          1 
 11*     RUN:io       DONE      READY          1            
 12     WAITING       DONE    RUN:cpu          1          1 
 13     WAITING       DONE    RUN:cpu          1          1 
 14     WAITING       DONE       DONE                     1 
 15     WAITING       DONE       DONE                     1 
 16*       DONE       DONE       DONE                       

Stats: Total Time 16
Stats: CPU Busy 13 (81.25%)
Stats: IO Busy  12 (75.00%)

```

* 8.Q:Now run with some randomly generated processes, e.g., -s 1 -l 3:50,3:50, -s 2 -l 3:50,3:50, -s 3 -l 3:50,3:50. See if you can predict how the trace will turn out. What happens when you use-I IO_RUN_IMMEDIATE vs. -I IO_RUN_LATER?What happens when you use -S SWITCH ON IO vs. -S SWITCH ON END?
A:IO_RUN_IMMEDIATE may takes less ticks than IO_RUN_LATER, see Q6 and Q7, in Q6 the rest IO instructions of Process 0 run at last and no more CPU-using process can make use of CPU when Process 0 is waiting for IO completion, but in Q7 the CPU-using ones can exactly use CPU when IO completion is being waited.For the same reason, in case Q4 and Q5, CPU is not made better use of during IO waiting with SWITCH_ON_END, so SWITCH_ON_IO may takes less ticks than SWITCH_ON_END. 