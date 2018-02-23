# Ch8 Homework
### Q1:Run a few randomly-generated problems with just two jobs and two queues; compute the MLFQ execution trace for each. Make your life easier by limiting the length of each job and turning off I/Os.
A1:
```
➜  HW-MLFQ python mlfq.py -n 2 -j 2 -m 20 -M 0 -s 1
Here is the list of inputs:
OPTIONS jobs 2
OPTIONS queues 2
OPTIONS quantum length for queue  1 is  10
OPTIONS quantum length for queue  0 is  10
OPTIONS boost 0
OPTIONS ioTime 5
OPTIONS stayAfterIO False
OPTIONS iobump False


For each job, three defining characteristics are given:
  startTime : at what time does the job enter the system
  runTime   : the total CPU time needed by the job to finish
  ioFreq    : every ioFreq time units, the job issues an I/O
              (the I/O takes ioTime units to complete)

Job List:
  Job  0: startTime   0 - runTime   3 - ioFreq   0
  Job  1: startTime   0 - runTime  15 - ioFreq   0
```
For this case, the execution trace will be:
Time 0~2 Run Job0 at PRIORITY 1
Time 3~12 Run Job1 at PRIORITY 1
Time 13~17 Run Job1 at PRIORITY 0

```
➜  HW-MLFQ python mlfq.py -n 2 -j 2 -m 20 -M 0 -s 2 -c
Here is the list of inputs:
OPTIONS jobs 2
OPTIONS queues 2
OPTIONS quantum length for queue  1 is  10
OPTIONS quantum length for queue  0 is  10
OPTIONS boost 0
OPTIONS ioTime 5
OPTIONS stayAfterIO False
OPTIONS iobump False


For each job, three defining characteristics are given:
  startTime : at what time does the job enter the system
  runTime   : the total CPU time needed by the job to finish
  ioFreq    : every ioFreq time units, the job issues an I/O
              (the I/O takes ioTime units to complete)

Job List:
  Job  0: startTime   0 - runTime  19 - ioFreq   0
  Job  1: startTime   0 - runTime   2 - ioFreq   0
```
For this case, the execution trace will be:
Time 0~9 Run Job0 at PRIORITY 1
Time 10~11 Run Job1 at PRIORITY 1
Time 12~20 Run Job0 at PRIORITY 0

```
➜  HW-MLFQ python mlfq.py -n 2 -j 2 -m 20 -M 0 -s 3 -c
Here is the list of inputs:
OPTIONS jobs 2
OPTIONS queues 2
OPTIONS quantum length for queue  1 is  10
OPTIONS quantum length for queue  0 is  10
OPTIONS boost 0
OPTIONS ioTime 5
OPTIONS stayAfterIO False
OPTIONS iobump False


For each job, three defining characteristics are given:
  startTime : at what time does the job enter the system
  runTime   : the total CPU time needed by the job to finish
  ioFreq    : every ioFreq time units, the job issues an I/O
              (the I/O takes ioTime units to complete)

Job List:
  Job  0: startTime   0 - runTime   5 - ioFreq   0
  Job  1: startTime   0 - runTime   8 - ioFreq   0
```
For this case, the execution trace will be:
Time 0~4 Run Job0 at PRIORITY 1
Time 5~12 Run Job1 at PRIORITY 1
### Q2:How would you run the scheduler to reproduce each of the examples in the chapter?
A2:
Example1: In this example, there are three queues,of which the length of time slice is 10ms, i.e. the default value, the job is a single long-running job which takes 200 ms, and its priority is reduced once every 10ms.
```
➜  HW-MLFQ python mlfq.py -n 3 -l 0,200,0 -c
```
Example2: Same queues as example 1, according to the example figure, job0 is in the lowest priority since time 0, so we can use an extra 20 ms to reduce the priority of job0 to the lowest, in this time line, job1 begins at time 120 and takes 20ms. 
```
➜  HW-MLFQ python mlfq.py -n 3 -l 0,200,0:120,20,0 -c
```
Example3: Same queues and same way to use extra 20ms to reduce the priority of job0 as before, don’t forget to add the —stay flag to ensure the job is at same priority when issuing I/O.
```
➜  HW-MLFQ python mlfq.py -n 3 -l 0,200,0:20,20,1 -c -i 9 --stay
```
Example4: 
Without Priority Boost, Job 0 will starve, only when both Job 1 and Job 2 have finished will Job 0 have access to CPU.
```
➜  HW-MLFQ python mlfq.py -n 3 -l 0,200,0:100,50,5:105,50,5 -c -i 5 --stay
```
With Priority Boost,Job 0 will be boosted into the highest priority once every 50ms, and will never starve.
```
➜  HW-MLFQ python mlfq.py -n 3 -l 0,200,0:100,50,5:105,50,5 -c -i 5 --stay -B 50
```
Example5:
Without Gaming Tolerance, a process can dominate CPU time by issuing an I/O just before a time slice ends.
```
➜  HW-MLFQ python mlfq.py -n 3 -l 0,40,0:21,180,9 -c -i 1 --stay
```
With Gaming Tolerance, once a job uses up its time allotment at a given level, it will be degraded regardless of its I/O behavior.This way, when Job 1 uses up its given 10ms allotment, its priority is reduced.
```
➜  HW-MLFQ python mlfq.py -n 3 -l 0,40,0:21,180,9 -c -i 1
```
Example6:
In this case, Q2 time slice length = 10ms ,Q1 time slice length = 20ms, Q0 time slice length = 50ms.
```
➜  HW-MLFQ python mlfq.py -Q 10,20,50  -l 0,130,0:0,130,0  -c
```

### Q3:How would you configure the scheduler parameters to behave just like a round-robin scheduler?
A3:By creating one queue and  set the —quantum of it as the time slice of RR.
e.g.
```
➜  HW-MLFQ python mlfq.py -n 1 -q 10 -j 2 -l 0,100,0:0,100,0 -c
```

### Q4:Craft a workload with two jobs and scheduler parameters so that one job takes advantage of the older Rules 4a and 4b (turned on with the -S flag) to game the scheduler and obtain 99% of the CPU over a particular time interval.
A4: it’s just the case in Q1 example5 without gaming tolerance.
```
➜  HW-MLFQ python mlfq.py -n 3 -l 0,40,0:21,180,9 -c -i 1 --stay
```

### Q5:Given a system with a quantum length of 10 ms in its highest queue, how often would you have to boost jobs back to the highest priority level (with the -B flag) in order to guarantee that a single long- running (and potentially-starving) job gets at least 5% of the CPU?
A5: quantum length / boost time >= 5%, so boost time must <= 200 ms.

### Q6:One question that arises in scheduling is which end of a queue to add a job that just finished I/O; the -I flag changes this behavior for this scheduling simulator. Play around with some workloads and see if you can see the effect of this flag.
A6:
```
➜  HW-MLFQ python mlfq.py -n 1 -l 0,50,5:0,50,0 -c
```
Gives:
```
[ time 0 ] JOB BEGINS by JOB 0
[ time 0 ] JOB BEGINS by JOB 1
[ time 0 ] Run JOB 0 at PRIORITY 0 [ TICKSLEFT 9 RUNTIME 50 TIMELEFT 49 ]
[ time 1 ] Run JOB 0 at PRIORITY 0 [ TICKSLEFT 8 RUNTIME 50 TIMELEFT 48 ]
[ time 2 ] Run JOB 0 at PRIORITY 0 [ TICKSLEFT 7 RUNTIME 50 TIMELEFT 47 ]
[ time 3 ] Run JOB 0 at PRIORITY 0 [ TICKSLEFT 6 RUNTIME 50 TIMELEFT 46 ]
[ time 4 ] Run JOB 0 at PRIORITY 0 [ TICKSLEFT 5 RUNTIME 50 TIMELEFT 45 ]
[ time 5 ] IO_START by JOB 0
IO DONE
[ time 5 ] Run JOB 1 at PRIORITY 0 [ TICKSLEFT 9 RUNTIME 50 TIMELEFT 49 ]
[ time 6 ] Run JOB 1 at PRIORITY 0 [ TICKSLEFT 8 RUNTIME 50 TIMELEFT 48 ]
[ time 7 ] Run JOB 1 at PRIORITY 0 [ TICKSLEFT 7 RUNTIME 50 TIMELEFT 47 ]
[ time 8 ] Run JOB 1 at PRIORITY 0 [ TICKSLEFT 6 RUNTIME 50 TIMELEFT 46 ]
[ time 9 ] Run JOB 1 at PRIORITY 0 [ TICKSLEFT 5 RUNTIME 50 TIMELEFT 45 ]
[ time 10 ] IO_DONE by JOB 0
[ time 10 ] Run JOB 1 at PRIORITY 0 [ TICKSLEFT 4 RUNTIME 50 TIMELEFT 44 ]
[ time 11 ] Run JOB 1 at PRIORITY 0 [ TICKSLEFT 3 RUNTIME 50 TIMELEFT 43 ]
[ time 12 ] Run JOB 1 at PRIORITY 0 [ TICKSLEFT 2 RUNTIME 50 TIMELEFT 42 ]
[ time 13 ] Run JOB 1 at PRIORITY 0 [ TICKSLEFT 1 RUNTIME 50 TIMELEFT 41 ]
[ time 14 ] Run JOB 1 at PRIORITY 0 [ TICKSLEFT 0 RUNTIME 50 TIMELEFT 40 ]
```
While
```
➜  HW-MLFQ python mlfq.py -n 1 -l 0,50,5:0,50,0 -c -I
```
Gives
```
[ time 0 ] JOB BEGINS by JOB 0
[ time 0 ] JOB BEGINS by JOB 1
[ time 0 ] Run JOB 0 at PRIORITY 0 [ TICKSLEFT 9 RUNTIME 50 TIMELEFT 49 ]
[ time 1 ] Run JOB 0 at PRIORITY 0 [ TICKSLEFT 8 RUNTIME 50 TIMELEFT 48 ]
[ time 2 ] Run JOB 0 at PRIORITY 0 [ TICKSLEFT 7 RUNTIME 50 TIMELEFT 47 ]
[ time 3 ] Run JOB 0 at PRIORITY 0 [ TICKSLEFT 6 RUNTIME 50 TIMELEFT 46 ]
[ time 4 ] Run JOB 0 at PRIORITY 0 [ TICKSLEFT 5 RUNTIME 50 TIMELEFT 45 ]
[ time 5 ] IO_START by JOB 0
IO DONE
[ time 5 ] Run JOB 1 at PRIORITY 0 [ TICKSLEFT 9 RUNTIME 50 TIMELEFT 49 ]
[ time 6 ] Run JOB 1 at PRIORITY 0 [ TICKSLEFT 8 RUNTIME 50 TIMELEFT 48 ]
[ time 7 ] Run JOB 1 at PRIORITY 0 [ TICKSLEFT 7 RUNTIME 50 TIMELEFT 47 ]
[ time 8 ] Run JOB 1 at PRIORITY 0 [ TICKSLEFT 6 RUNTIME 50 TIMELEFT 46 ]
[ time 9 ] Run JOB 1 at PRIORITY 0 [ TICKSLEFT 5 RUNTIME 50 TIMELEFT 45 ]
[ time 10 ] IO_START by JOB 1
IO DONE
[ time 10 ] IO_DONE by JOB 0
[ time 10 ] Run JOB 0 at PRIORITY 0 [ TICKSLEFT 4 RUNTIME 50 TIMELEFT 44 ]
[ time 11 ] Run JOB 0 at PRIORITY 0 [ TICKSLEFT 3 RUNTIME 50 TIMELEFT 43 ]
[ time 12 ] Run JOB 0 at PRIORITY 0 [ TICKSLEFT 2 RUNTIME 50 TIMELEFT 42 ]
[ time 13 ] Run JOB 0 at PRIORITY 0 [ TICKSLEFT 1 RUNTIME 50 TIMELEFT 41 ]
[ time 14 ] Run JOB 0 at PRIORITY 0 [ TICKSLEFT 0 RUNTIME 50 TIMELEFT 40 ]
```
It’s clear that with -I flag. The job that completes an I/O will be moved to the head of the queue.