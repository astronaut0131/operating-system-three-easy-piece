# Ch9 Homework
### Q1:Compute the solutions for simulations with 3 jobs and random seeds of 1, 2, and 3.
A1:
Seed1:
```
➜  HW-Lottery python lottery.py -j 3 -s 1
ARG jlist 
ARG jobs 3
ARG maxlen 10
ARG maxticket 100
ARG quantum 1
ARG seed 1

Here is the job list, with the run time of each job: 
  Job 0 ( length = 1, tickets = 84 )
  Job 1 ( length = 7, tickets = 25 )
  Job 2 ( length = 4, tickets = 44 )


Here is the set of random numbers you will need (at most):
Random 651593
Random 788724
Random 93859
Random 28347
Random 835765
Random 432767
Random 762280
Random 2106
Random 445387
Random 721540
Random 228762
Random 945271
```
Seed2:
```
➜  HW-Lottery python lottery.py -j 3 -s 2
ARG jlist 
ARG jobs 3
ARG maxlen 10
ARG maxticket 100
ARG quantum 1
ARG seed 2

Here is the job list, with the run time of each job: 
  Job 0 ( length = 9, tickets = 94 )
  Job 1 ( length = 8, tickets = 73 )
  Job 2 ( length = 6, tickets = 30 )


Here is the set of random numbers you will need (at most):
Random 605944
Random 606802
Random 581204
Random 158383
Random 430670
Random 393532
Random 723012
Random 994820
Random 949396
Random 544177
Random 444854
Random 268241
Random 35924
Random 27444
Random 464894
Random 318465
Random 380015
Random 891790
Random 525753
Random 560510
Random 236123
Random 23858
Random 325143
```
Seed3:
```
➜  HW-Lottery python lottery.py -j 3 -s 3
ARG jlist 
ARG jobs 3
ARG maxlen 10
ARG maxticket 100
ARG quantum 1
ARG seed 3

Here is the job list, with the run time of each job: 
  Job 0 ( length = 2, tickets = 54 )
  Job 1 ( length = 3, tickets = 60 )
  Job 2 ( length = 6, tickets = 6 )


Here is the set of random numbers you will need (at most):
Random 13168
Random 837469
Random 259354
Random 234331
Random 995645
Random 470263
Random 836462
Random 476353
Random 639068
Random 150616
Random 634861
```
Let’s write a python script to solve it :D
These are my input file
seed1.txt:
```
Here is the job list, with the run time of each job:
  Job 0 ( length = 1, tickets = 84 )
  Job 1 ( length = 7, tickets = 25 )
  Job 2 ( length = 4, tickets = 44 )


Here is the set of random numbers you will need (at most):
Random 651593
Random 788724
Random 93859
Random 28347
Random 835765
Random 432767
Random 762280
Random 2106
Random 445387
Random 721540
Random 228762
Random 945271
```
seed2.txt and seed3.txt are similar
This is my python script
```
def Q1():
    for i in range(1, 4):
        filename = 'seed' + str(i) + '.txt'
        trace(filename)


def trace(filename):
    with open(filename, 'r') as f:
        print('result of ' + filename)
        total_tickets = 0
        job_list = []
        random_list = []
        time_cnt = 1
        while 1:
            s = f.readline()
            if not s:
                break
            if s.strip().startswith('Job'):
                attr = {}
                s = s.strip().strip('\n')
                attr['length'] = int(s.split(',')[0].split(' = ')[1])
                attr['tickets'] = int(s.split(',')[1].split(' = ')[1].strip(' )'))
                attr['id'] = str(len(job_list))
                total_tickets += attr['tickets']
                job_list.append(attr)
            if s.startswith('Random'):
                random_list.append(int(s.strip('\n').split(' ')[1]))
    for random in random_list:
        winner = random % total_tickets
        counter = 0
        for job in job_list:
            counter += job['tickets']
            if counter > winner:
                print('Random {} winning ticket {} -> running job {}'.format(random, winner, job['id']))
                job['length'] -= 1
                if job['length'] == 0:
                    print('job {} is done at time {}'.format(job['id'], time_cnt))
                    total_tickets -= job['tickets']  # if a job is finished, total tickets should be reduced
                    job['tickets'] = 0  # its tickets should be set to 0
                time_cnt += 1
                break


if __name__ == '__main__':
    Q1()

```
Run result
```
result of seed1.txt
Random 651593 winning ticket 119 -> running job 2
Random 788724 winning ticket 9 -> running job 0
job 0 is done at time 2
Random 93859 winning ticket 19 -> running job 1
Random 28347 winning ticket 57 -> running job 2
Random 835765 winning ticket 37 -> running job 2
Random 432767 winning ticket 68 -> running job 2
job 2 is done at time 6
Random 762280 winning ticket 5 -> running job 1
Random 2106 winning ticket 6 -> running job 1
Random 445387 winning ticket 12 -> running job 1
Random 721540 winning ticket 15 -> running job 1
Random 228762 winning ticket 12 -> running job 1
Random 945271 winning ticket 21 -> running job 1
job 1 is done at time 12
result of seed2.txt
Random 605944 winning ticket 169 -> running job 2
Random 606802 winning ticket 42 -> running job 0
Random 581204 winning ticket 54 -> running job 0
Random 158383 winning ticket 192 -> running job 2
Random 430670 winning ticket 28 -> running job 0
Random 393532 winning ticket 123 -> running job 1
Random 723012 winning ticket 22 -> running job 0
Random 994820 winning ticket 167 -> running job 2
Random 949396 winning ticket 53 -> running job 0
Random 544177 winning ticket 63 -> running job 0
Random 444854 winning ticket 28 -> running job 0
Random 268241 winning ticket 124 -> running job 1
Random 35924 winning ticket 70 -> running job 0
Random 27444 winning ticket 61 -> running job 0
job 0 is done at time 14
Random 464894 winning ticket 55 -> running job 1
Random 318465 winning ticket 92 -> running job 2
Random 380015 winning ticket 48 -> running job 1
Random 891790 winning ticket 16 -> running job 1
Random 525753 winning ticket 41 -> running job 1
Random 560510 winning ticket 87 -> running job 2
Random 236123 winning ticket 47 -> running job 1
Random 23858 winning ticket 65 -> running job 1
job 1 is done at time 22
Random 325143 winning ticket 3 -> running job 2
job 2 is done at time 23
result of seed3.txt
Random 13168 winning ticket 88 -> running job 1
Random 837469 winning ticket 109 -> running job 1
Random 259354 winning ticket 34 -> running job 0
Random 234331 winning ticket 91 -> running job 1
job 1 is done at time 4
Random 995645 winning ticket 5 -> running job 0
job 0 is done at time 5
Random 470263 winning ticket 1 -> running job 2
Random 836462 winning ticket 2 -> running job 2
Random 476353 winning ticket 1 -> running job 2
Random 639068 winning ticket 2 -> running job 2
Random 150616 winning ticket 4 -> running job 2
Random 634861 winning ticket 1 -> running job 2
job 2 is done at time 11
```

### Q2:Now run with two specific jobs: each of length 10, but one (job 0) with just 1 ticket and the other (job 1) with 100 (e.g., -l 10:1,10:100). What happens when the number of tickets is so imbalanced? Will job 0 ever run before job 1 completes? How often? In general, what does such a ticket imbalance do to the behavior of lottery scheduling?
Using the same script
```
def Q2():
    trace('Q2.txt')

if __name__ == '__main__':
    Q2()
```
Q2.txt
```
Here is the job list, with the run time of each job:
  Job 0 ( length = 10, tickets = 1 )
  Job 1 ( length = 10, tickets = 100 )


Here is the set of random numbers you will need (at most):
Random 844422
Random 757955
Random 420572
Random 258917
Random 511275
Random 404934
Random 783799
Random 303313
Random 476597
Random 583382
Random 908113
Random 504687
Random 281838
Random 755804
Random 618369
Random 250506
Random 909747
Random 982786
Random 810218
Random 902166
```

Run result:
```
result of Q2.txt
Random 844422 winning ticket 62 -> running job 1
Random 757955 winning ticket 51 -> running job 1
Random 420572 winning ticket 8 -> running job 1
Random 258917 winning ticket 54 -> running job 1
Random 511275 winning ticket 13 -> running job 1
Random 404934 winning ticket 25 -> running job 1
Random 783799 winning ticket 39 -> running job 1
Random 303313 winning ticket 10 -> running job 1
Random 476597 winning ticket 79 -> running job 1
Random 583382 winning ticket 6 -> running job 1
job 1 is done at time 10
Random 908113 winning ticket 0 -> running job 0
Random 504687 winning ticket 0 -> running job 0
Random 281838 winning ticket 0 -> running job 0
Random 755804 winning ticket 0 -> running job 0
Random 618369 winning ticket 0 -> running job 0
Random 250506 winning ticket 0 -> running job 0
Random 909747 winning ticket 0 -> running job 0
Random 982786 winning ticket 0 -> running job 0
Random 810218 winning ticket 0 -> running job 0
Random 902166 winning ticket 0 -> running job 0
job 0 is done at time 20
```
With such imbalanced tickets, job 0 are not likely to run before job 1, in that the possibility for job 0 to be the winning ticket is very low.Such a ticket imbalance will lead to long average response time and starvation of long run process with a few tickets.

### Q3:When running with two jobs of length 100 and equal ticket allocations of 100 (-l 100:100,100:100), how unfair is the scheduler? Run with some different random seeds to determine the (probabilistic) answer; let unfairness be determined by how much earlier one job finishes than the other.
A3: using a python script to solve again.
```
import os
seeds_num = 100
U_total = 0
for i in range(seeds_num):
    result = os.popen('python lottery.py -l 100:100,100:100 -s {} -c'.format(str(i))).read().split('\n')
    job_time = []
    for item in result:
        if 'DONE' in item:
            job_time.append(int(item.split('time ')[1]))
    print('seed {} unfairness metric: {}'.format(i,job_time[0]/job_time[1]))
    U_total += job_time[0]/job_time[1]

print('{} seeds give an average unfairness metric of {}'.format(seeds_num,U_total/seeds_num))
```
Gives the result
```
seed 0 unfairness metric: 0.96
seed 1 unfairness metric: 0.98
seed 2 unfairness metric: 0.95
seed 3 unfairness metric: 0.98
seed 4 unfairness metric: 0.995
seed 5 unfairness metric: 0.905
seed 6 unfairness metric: 0.965
seed 7 unfairness metric: 0.925
seed 8 unfairness metric: 0.955
seed 9 unfairness metric: 0.96
seed 10 unfairness metric: 0.985
seed 11 unfairness metric: 0.98
seed 12 unfairness metric: 0.945
seed 13 unfairness metric: 0.985
seed 14 unfairness metric: 0.935
seed 15 unfairness metric: 0.92
seed 16 unfairness metric: 0.935
seed 17 unfairness metric: 0.9
seed 18 unfairness metric: 0.9
seed 19 unfairness metric: 0.875
seed 20 unfairness metric: 0.98
seed 21 unfairness metric: 0.875
seed 22 unfairness metric: 0.95
seed 23 unfairness metric: 0.98
seed 24 unfairness metric: 0.905
seed 25 unfairness metric: 0.91
seed 26 unfairness metric: 0.925
seed 27 unfairness metric: 0.99
seed 28 unfairness metric: 0.955
seed 29 unfairness metric: 0.905
seed 30 unfairness metric: 0.99
seed 31 unfairness metric: 0.905
seed 32 unfairness metric: 0.935
seed 33 unfairness metric: 0.91
seed 34 unfairness metric: 0.885
seed 35 unfairness metric: 0.955
seed 36 unfairness metric: 0.98
seed 37 unfairness metric: 0.95
seed 38 unfairness metric: 0.92
seed 39 unfairness metric: 0.89
seed 40 unfairness metric: 0.95
seed 41 unfairness metric: 0.96
seed 42 unfairness metric: 0.985
seed 43 unfairness metric: 0.98
seed 44 unfairness metric: 0.92
seed 45 unfairness metric: 0.985
seed 46 unfairness metric: 0.99
seed 47 unfairness metric: 0.92
seed 48 unfairness metric: 0.885
seed 49 unfairness metric: 0.94
seed 50 unfairness metric: 0.94
seed 51 unfairness metric: 0.9
seed 52 unfairness metric: 0.975
seed 53 unfairness metric: 0.95
seed 54 unfairness metric: 0.975
seed 55 unfairness metric: 0.985
seed 56 unfairness metric: 0.99
seed 57 unfairness metric: 0.975
seed 58 unfairness metric: 0.91
seed 59 unfairness metric: 0.99
seed 60 unfairness metric: 0.955
seed 61 unfairness metric: 0.85
seed 62 unfairness metric: 0.995
seed 63 unfairness metric: 0.96
seed 64 unfairness metric: 0.94
seed 65 unfairness metric: 0.975
seed 66 unfairness metric: 0.985
seed 67 unfairness metric: 0.995
seed 68 unfairness metric: 0.955
seed 69 unfairness metric: 0.865
seed 70 unfairness metric: 0.89
seed 71 unfairness metric: 0.985
seed 72 unfairness metric: 0.975
seed 73 unfairness metric: 0.885
seed 74 unfairness metric: 0.995
seed 75 unfairness metric: 0.985
seed 76 unfairness metric: 0.965
seed 77 unfairness metric: 0.955
seed 78 unfairness metric: 0.915
seed 79 unfairness metric: 0.955
seed 80 unfairness metric: 0.97
seed 81 unfairness metric: 0.895
seed 82 unfairness metric: 0.995
seed 83 unfairness metric: 0.91
seed 84 unfairness metric: 0.925
seed 85 unfairness metric: 0.935
seed 86 unfairness metric: 0.89
seed 87 unfairness metric: 0.94
seed 88 unfairness metric: 0.9
seed 89 unfairness metric: 0.965
seed 90 unfairness metric: 0.96
seed 91 unfairness metric: 0.9
seed 92 unfairness metric: 0.905
seed 93 unfairness metric: 0.985
seed 94 unfairness metric: 0.96
seed 95 unfairness metric: 0.91
seed 96 unfairness metric: 0.91
seed 97 unfairness metric: 0.9
seed 98 unfairness metric: 0.965
seed 99 unfairness metric: 0.915
100 seeds give an average unfairness metric of 0.9434999999999999
```

### Q4:How does your answer to the previous question change as the quantum size (-q) gets larger?
A4: just change the script a little.
```
import os
seeds_num = 100
q_num = 100
for q in range(10,q_num,10):
    U_total = 0
    for i in range(seeds_num):
        result = os.popen('python lottery.py -l 100:100,100:100 -s {} -c -q {}'.format(str(i),str(q))).read().split('\n')
        job_time = []
        for item in result:
            if 'DONE' in item:
                job_time.append(int(item.split('time ')[1]))
        #    print('seed {} unfairness metric: {}'.format(i,job_time[0]/job_time[1]))
        U_total += job_time[0] / job_time[1]
    print('{} seeds with {} quantum length give an average unfairness metric of {}'.format(seeds_num,q,U_total/seeds_num))
```
Gives
```
100 seeds with 10 quantum length give an average unfairness metric of 0.8319999999999997
100 seeds with 20 quantum length give an average unfairness metric of 0.7490000000000002
100 seeds with 30 quantum length give an average unfairness metric of 0.72625
100 seeds with 40 quantum length give an average unfairness metric of 0.6983333333333335
100 seeds with 50 quantum length give an average unfairness metric of 0.6375
100 seeds with 60 quantum length give an average unfairness metric of 0.6375
100 seeds with 70 quantum length give an average unfairness metric of 0.6375
100 seeds with 80 quantum length give an average unfairness metric of 0.6375
100 seeds with 90 quantum length give an average unfairness metric of 0.6375
```
The last 5 numbers may be inaccurate, but the trend is obvious, the unfairness metric gets smaller(i.e. the more severe unfairness) as the quantum length grows.

### Q5:Can you make a version of the graph that is found in the chapter? What else would be worth exploring? How would the graph look with a stride scheduler?
A5: cannot understand the question so far : (