# Ch7 Homework
### Q1:Compute the response time and turnaround time when running three jobs of length 200 with the SJF and FIFO schedulers.
A1:
	let’s assume the jobs are done in the order of job0, job1, and job2.
	SJF response time:
		job0:0
		job1:200
		job2:400
	SJF turnaround time:
		job0:200
		job1:400
		job2:600
	FIFO response time:
		job0:0
		job1:200
		job2:400
	FIFO turnaround time:
		job0:200
		job1:400
		job2:600
Check the answer:
```
➜  HW-Scheduler python scheduler.py -p SJF -l 200,200,200 -c
ARG policy SJF
ARG jlist 200,200,200

Here is the job list, with the run time of each job: 
  Job 0 ( length = 200.0 )
  Job 1 ( length = 200.0 )
  Job 2 ( length = 200.0 )


** Solutions **

Execution trace:
  [ time   0 ] Run job 0 for 200.00 secs ( DONE at 200.00 )
  [ time 200 ] Run job 1 for 200.00 secs ( DONE at 400.00 )
  [ time 400 ] Run job 2 for 200.00 secs ( DONE at 600.00 )

Final statistics:
  Job   0 -- Response: 0.00  Turnaround 200.00  Wait 0.00
  Job   1 -- Response: 200.00  Turnaround 400.00  Wait 200.00
  Job   2 -- Response: 400.00  Turnaround 600.00  Wait 400.00

  Average -- Response: 200.00  Turnaround 400.00  Wait 200.00
```
```
➜  HW-Scheduler python scheduler.py -p FIFO -l 200,200,200 -c
ARG policy FIFO
ARG jlist 200,200,200

Here is the job list, with the run time of each job: 
  Job 0 ( length = 200.0 )
  Job 1 ( length = 200.0 )
  Job 2 ( length = 200.0 )


** Solutions **

Execution trace:
  [ time   0 ] Run job 0 for 200.00 secs ( DONE at 200.00 )
  [ time 200 ] Run job 1 for 200.00 secs ( DONE at 400.00 )
  [ time 400 ] Run job 2 for 200.00 secs ( DONE at 600.00 )

Final statistics:
  Job   0 -- Response: 0.00  Turnaround 200.00  Wait 0.00
  Job   1 -- Response: 200.00  Turnaround 400.00  Wait 200.00
  Job   2 -- Response: 400.00  Turnaround 600.00  Wait 400.00

  Average -- Response: 200.00  Turnaround 400.00  Wait 200.00
```
### Q2:Now do the same but with jobs of different lengths: 100, 200, and 300.
A2:	
	SJF response time:
		job0:0
		job1:100
		job2:300
	SJF turnaround time:
		job0:100
		job1:300
		job2:600
	FIFO response time:
		job0:0
		job1:100
		job2:300
	FIFO turnaround time:
		job0:100
		job1:300
		job2:600
Check the answer:
```
➜  HW-Scheduler python scheduler.py -p SJF -l 100,200,300 -c
ARG policy SJF
ARG jlist 100,200,300

Here is the job list, with the run time of each job: 
  Job 0 ( length = 100.0 )
  Job 1 ( length = 200.0 )
  Job 2 ( length = 300.0 )


** Solutions **

Execution trace:
  [ time   0 ] Run job 0 for 100.00 secs ( DONE at 100.00 )
  [ time 100 ] Run job 1 for 200.00 secs ( DONE at 300.00 )
  [ time 300 ] Run job 2 for 300.00 secs ( DONE at 600.00 )

Final statistics:
  Job   0 -- Response: 0.00  Turnaround 100.00  Wait 0.00
  Job   1 -- Response: 100.00  Turnaround 300.00  Wait 100.00
  Job   2 -- Response: 300.00  Turnaround 600.00  Wait 300.00

  Average -- Response: 133.33  Turnaround 333.33  Wait 133.33
```

```
➜  HW-Scheduler python scheduler.py -p FIFO -l 100,200,300 -c
ARG policy FIFO
ARG jlist 100,200,300

Here is the job list, with the run time of each job: 
  Job 0 ( length = 100.0 )
  Job 1 ( length = 200.0 )
  Job 2 ( length = 300.0 )


** Solutions **

Execution trace:
  [ time   0 ] Run job 0 for 100.00 secs ( DONE at 100.00 )
  [ time 100 ] Run job 1 for 200.00 secs ( DONE at 300.00 )
  [ time 300 ] Run job 2 for 300.00 secs ( DONE at 600.00 )

Final statistics:
  Job   0 -- Response: 0.00  Turnaround 100.00  Wait 0.00
  Job   1 -- Response: 100.00  Turnaround 300.00  Wait 100.00
  Job   2 -- Response: 300.00  Turnaround 600.00  Wait 300.00

  Average -- Response: 133.33  Turnaround 333.33  Wait 133.33
```

### Q3:Now do the same, but also with the RR scheduler and a time-slice of 1.
A3:
	RR response time:
		job0:0
		job1:1
		job2:2
	RR turnaround time:
		job0:(200/1 - 1) * 3 + 1 = 598
		job1: job0 + 1 = 599
		job2: job1 + 1 = 600
Check the answer:
```
➜  HW-Scheduler python scheduler.py -p RR -l 200,200,200 -c -q 1

Final statistics:
  Job   0 -- Response: 0.00  Turnaround 598.00  Wait 398.00
  Job   1 -- Response: 1.00  Turnaround 599.00  Wait 399.00
  Job   2 -- Response: 2.00  Turnaround 600.00  Wait 400.00

  Average -- Response: 1.00  Turnaround 599.00  Wait 399.00
```

### Q4:For what types of workloads does SJF deliver the same turnaround times as FIFO?
A4:With the assumption that once started, each job runs to complete, the answer is when the jobs arrive in the order of nondecreasing length.(An answer can be found on https://stackoverflow.com/questions/26007987/cpu-scheduling-response-time)

### Q5:For what types of workloads and quantum lengths does SJF deliver the same response times as RR?
A5:When the jobs arrive in the order of nondecreasing length and quantum lengths >= the maximum length of all the jobs.

### Q6:What happens to response time with SJF as job lengths increase? Can you use the simulator to demonstrate the trend?
A6: the average response time will increase.A good solution can be found [ostep/answers.md at master · asnr/ostep · GitHub](https://github.com/asnr/ostep/blob/master/virtualisation/7_cpu_scheduling/answers.md) ,I can not write bash script : ) so take a look at his answer.

### Q7:What happens to response time with RR as quantum lengths increase? Can you write an equation that gives the worst-case response time, given N jobs?
A7: the response time with RR will increase.The worst-case response time will be the case where the quantum length >= the maximum length of N jobs, and the jobs are done in non increasing order of length.