# Ch17 Homework
### Q1:First run with the flags-n10-H0-p BEST -s 0 to generate a few random allocations and frees. Can you predict what alloc()/free() will return? Can you guess the state of the free list after each request? What do you notice about the free list over time?
A1:
```
➜  HW-Freespace python malloc.py -n 10 -H 0 -p BEST -s 0
seed 0
size 100
baseAddr 1000
headerSize 0
alignment -1
policy BEST
listOrder ADDRSORT
coalesce False
numOps 10
range 10
percentAlloc 50
allocList 
compute False

ptr[0] = Alloc(3)  returned ?
List? 
A: returned 1000
   Free List [Size 1]: [ addr:1003 sz:97]
Free(ptr[0]) returned ?
List? 
A: returned 0
	 Free List [Size 2]: [ addr:1000 sz:3], [ addr:1003,sz:97]
ptr[1] = Alloc(5)  returned ?
List? 
A: returned 1003
   Free List[Size 2]: [ addr:1000 sz:3],[ addr:1008,sz:92]
Free(ptr[1]) returned ?
List? 
A: returned 0
	 Free List [Size 3]:[ addr:1000 sz:3],[ addr:1003 sz:5],[ addr:1008,sz:92]
ptr[2] = Alloc(8)  returned ?
List? 
A: returned 1008
	 Free List [Size 3]:[ addr:1000 sz:3],[ addr:1003 sz:5],[ addr:1016,sz:84]
Free(ptr[2]) returned ?
List? 
A: returned 0
	 Free List [Size 4]:[ addr:1000 sz:3],[ addr:1003 sz:5],[ addr:1008 sz:8][ addr:1016,sz:84]

ptr[3] = Alloc(8)  returned ?
List? 
A: returned 1008
	 Free List [Size 3]:[ addr:1000 sz:3],[ addr:1003 sz:5],[ addr:1016,sz:84]
Free(ptr[3]) returned ?
List? 
A: returned 0
	 Free List [Size 4]:[ addr:1000 sz:3],[ addr:1003 sz:5],[ addr:1008 sz:8][ addr:1016,sz:84]
ptr[4] = Alloc(2)  returned ?
List? 
A: returned 1000
	 Free List [Size 4]:[ addr:1002 sz:1],[ addr:1003 sz:5],[ addr:1008 sz:8][ addr:1016,sz:84]
ptr[5] = Alloc(7)  returned ?
List?
A: returned 1008
	 Free List [Size 4]:[ addr:1002 sz:1],[ addr:1003 sz:5],[ addr:1015 sz:1][ addr:1016,sz:84]
```

### Q2:How are the results different when using a WORST fit policy to search the free list (-p WORST)? What changes?
A2: with worst-fit policy 
```
ptr[0] = Alloc(3)  returned 1000 (searched 1 elements)
Free List [ Size 1 ]:  [ addr:1003 sz:97 ] 

Free(ptr[0]) returned 0
Free List [ Size 2 ]:  [ addr:1000 sz:3 ] [ addr:1003 sz:97 ] 

ptr[1] = Alloc(5)  returned 1003 (searched 2 elements)
Free List [ Size 2 ]:  [ addr:1000 sz:3 ] [ addr:1008 sz:92 ] 

Free(ptr[1]) returned 0
Free List [ Size 3 ]:  [ addr:1000 sz:3 ] [ addr:1003 sz:5 ] [ addr:1008 sz:92 ] 

ptr[2] = Alloc(8)  returned 1008 (searched 3 elements)
Free List [ Size 3 ]:  [ addr:1000 sz:3 ] [ addr:1003 sz:5 ] [ addr:1016 sz:84 ] 

Free(ptr[2]) returned 0
Free List [ Size 4 ]:  [ addr:1000 sz:3 ] [ addr:1003 sz:5 ] [ addr:1008 sz:8 ] [ addr:1016 sz:84 ] 

ptr[3] = Alloc(8)  returned 1016 (searched 4 elements)
Free List [ Size 4 ]:  [ addr:1000 sz:3 ] [ addr:1003 sz:5 ] [ addr:1008 sz:8 ] [ addr:1024 sz:76 ] 

Free(ptr[3]) returned 0
Free List [ Size 5 ]:  [ addr:1000 sz:3 ] [ addr:1003 sz:5 ] [ addr:1008 sz:8 ] [ addr:1016 sz:8 ] [ addr:1024 sz:76 ] 

ptr[4] = Alloc(2)  returned 1024 (searched 5 elements)
Free List [ Size 5 ]:  [ addr:1000 sz:3 ] [ addr:1003 sz:5 ] [ addr:1008 sz:8 ] [ addr:1016 sz:8 ] [ addr:1026 sz:74 ] 

ptr[5] = Alloc(7)  returned 1026 (searched 5 elements)
Free List [ Size 5 ]:  [ addr:1000 sz:3 ] [ addr:1003 sz:5 ] [ addr:1008 sz:8 ] [ addr:1016 sz:8 ] [ addr:1033 sz:67 ] 
```
Using this policy in the example will lead to more severe fragmentation and time cost compared to the best-fit policy.

### Q3:What about when using FIRST fit (-p FIRST)? What speeds up when you use first fit?
A3:
```
ptr[0] = Alloc(3)  returned 1000 (searched 1 elements)
Free List [ Size 1 ]:  [ addr:1003 sz:97 ] 

Free(ptr[0]) returned 0
Free List [ Size 2 ]:  [ addr:1000 sz:3 ] [ addr:1003 sz:97 ] 

ptr[1] = Alloc(5)  returned 1003 (searched 2 elements)
Free List [ Size 2 ]:  [ addr:1000 sz:3 ] [ addr:1008 sz:92 ] 

Free(ptr[1]) returned 0
Free List [ Size 3 ]:  [ addr:1000 sz:3 ] [ addr:1003 sz:5 ] [ addr:1008 sz:92 ] 

ptr[2] = Alloc(8)  returned 1008 (searched 3 elements)
Free List [ Size 3 ]:  [ addr:1000 sz:3 ] [ addr:1003 sz:5 ] [ addr:1016 sz:84 ] 

Free(ptr[2]) returned 0
Free List [ Size 4 ]:  [ addr:1000 sz:3 ] [ addr:1003 sz:5 ] [ addr:1008 sz:8 ] [ addr:1016 sz:84 ] 

ptr[3] = Alloc(8)  returned 1008 (searched 3 elements)
Free List [ Size 3 ]:  [ addr:1000 sz:3 ] [ addr:1003 sz:5 ] [ addr:1016 sz:84 ] 

Free(ptr[3]) returned 0
Free List [ Size 4 ]:  [ addr:1000 sz:3 ] [ addr:1003 sz:5 ] [ addr:1008 sz:8 ] [ addr:1016 sz:84 ] 

ptr[4] = Alloc(2)  returned 1000 (searched 1 elements)
Free List [ Size 4 ]:  [ addr:1002 sz:1 ] [ addr:1003 sz:5 ] [ addr:1008 sz:8 ] [ addr:1016 sz:84 ] 

ptr[5] = Alloc(7)  returned 1008 (searched 3 elements)
Free List [ Size 4 ]:  [ addr:1002 sz:1 ] [ addr:1003 sz:5 ] [ addr:1015 sz:1 ] [ addr:1016 sz:84 ] 

```
Element searching is speeded up when using first-fit policy, but the fragmentation is similar to the case with best-fit policy.

### Q4: For the above questions, how the list is kept ordered can affect the time it takes to find a free location for some of the policies. Use the different free list orderings (-l ADDRSORT, -l SIZESORT+, -l SIZESORT-) to see how the policies and the list orderings interact.
A4: well, let’s write a python script to solve it.     
```
import os
import itertools
basic_command = "python3 malloc.py -n 10 -H 0 -s 0 -c"
command_option_1 = [
    " -p BEST",
    " -p WORST",
    " -p FIRST"
]
command_option_2 = [
    " -l ADDRSORT",
    " -l SIZESORT+",
    " -l SIZESORT-"
]
commands = []
for item1 in itertools.permutations(command_option_1,1):
    for item2 in itertools.permutations(command_option_2,1):
        commands.append(basic_command + item1[0] + item2[0])

for command in commands:
    run_result = os.popen(command).read().strip().split('\n')
    search_elements = [item.split('searched')[1].split('elements')[0].strip() for item in run_result if('searched' in item)]
    avg_search = sum([int(item) for item in search_elements])/len(search_elements)
    print(f'{command}\ngives search elements result:\n{search_elements}\naverage = {avg_search}\n')
```
Gives:
```
python malloc.py -n 10 -H 0 -s 0 -c -p BEST -l ADDRSORT
gives search elements result:
['1', '2', '3', '4', '4', '4']
average = 3.0

python malloc.py -n 10 -H 0 -s 0 -c -p BEST -l SIZESORT+
gives search elements result:
['1', '2', '3', '4', '4', '4']
average = 3.0

python malloc.py -n 10 -H 0 -s 0 -c -p BEST -l SIZESORT-
gives search elements result:
['1', '2', '3', '4', '4', '4']
average = 3.0

python malloc.py -n 10 -H 0 -s 0 -c -p WORST -l ADDRSORT
gives search elements result:
['1', '2', '3', '4', '5', '5']
average = 3.3333333333333335

python malloc.py -n 10 -H 0 -s 0 -c -p WORST -l SIZESORT+
gives search elements result:
['1', '2', '3', '4', '5', '5']
average = 3.3333333333333335

python malloc.py -n 10 -H 0 -s 0 -c -p WORST -l SIZESORT-
gives search elements result:
['1', '2', '3', '4', '5', '5']
average = 3.3333333333333335

python malloc.py -n 10 -H 0 -s 0 -c -p FIRST -l ADDRSORT
gives search elements result:
['1', '2', '3', '3', '1', '3']
average = 2.1666666666666665

python malloc.py -n 10 -H 0 -s 0 -c -p FIRST -l SIZESORT+
gives search elements result:
['1', '2', '3', '3', '1', '3']
average = 2.1666666666666665

python malloc.py -n 10 -H 0 -s 0 -c -p FIRST -l SIZESORT-
gives search elements result:
['1', '1', '1', '1', '1', '1']
average = 1.0
```
From the result, we can conclude that best-fit and worst-fit policy ’s average efficiency
has nothing to do with the order of the free list, because they have to traverse over the whole free list regardless of what’s the order of the free list is, but the first-fit policy performs better under SIZESORT- in that the free chunk of maximum size is always put on the first position.

### Q5:Coalescing of a free list can be quite important.Increase the number of random allocations (say to -n 1000). What happens to larger allocation requests over time? Run with and without coalescing (i.e., without and with the -C flag). What differences in outcome do you see? How big is the free list over time in each case? Does the ordering of the list matter in this case?
A5: write another python script
```
import os
import itertools

basic_command = "python3 malloc.py -n 1000 -H 0 -s 0 -c"
command_option_1 = [
    " -p BEST",
    " -p WORST",
    " -p FIRST"
]
command_option_2 = [
    " -l ADDRSORT",
    " -l SIZESORT+",
    " -l SIZESORT-"
]
command_option_3 = [
    " -C",
    ""
]
commands = []
for item1 in itertools.permutations(command_option_1, 1):
    for item2 in itertools.permutations(command_option_2, 1):
        for item3 in itertools.permutations(command_option_3, 1):
            commands.append(basic_command + item1[0] + item2[0] + item3[0])

for command in commands:
    run_result = os.popen(command).read().strip().split('\n')
    free_list_size = [item.split('Size')[1].split(']')[0].strip() for item in run_result if 'Free List' in item]
    avg_free_list_size = sum(map(int,free_list_size)) / len(free_list_size)
    print(f'{command}\ngives average free list size = {avg_free_list_size}\n')
```

```
python3 malloc.py -n 1000 -H 0 -s 0 -c -p BEST -l ADDRSORT -C
gives average free list size = 3.222

python3 malloc.py -n 1000 -H 0 -s 0 -c -p BEST -l ADDRSORT
gives average free list size = 29.734

python3 malloc.py -n 1000 -H 0 -s 0 -c -p BEST -l SIZESORT+ -C
gives average free list size = 26.349

python3 malloc.py -n 1000 -H 0 -s 0 -c -p BEST -l SIZESORT+
gives average free list size = 29.734

python3 malloc.py -n 1000 -H 0 -s 0 -c -p BEST -l SIZESORT- -C
gives average free list size = 33.547

python3 malloc.py -n 1000 -H 0 -s 0 -c -p BEST -l SIZESORT-
gives average free list size = 29.734

python3 malloc.py -n 1000 -H 0 -s 0 -c -p WORST -l ADDRSORT -C
gives average free list size = 4.131

python3 malloc.py -n 1000 -H 0 -s 0 -c -p WORST -l ADDRSORT
gives average free list size = 78.066

python3 malloc.py -n 1000 -H 0 -s 0 -c -p WORST -l SIZESORT+ -C
gives average free list size = 75.946

python3 malloc.py -n 1000 -H 0 -s 0 -c -p WORST -l SIZESORT+
gives average free list size = 78.066

python3 malloc.py -n 1000 -H 0 -s 0 -c -p WORST -l SIZESORT- -C
gives average free list size = 70.974

python3 malloc.py -n 1000 -H 0 -s 0 -c -p WORST -l SIZESORT-
gives average free list size = 78.066

python3 malloc.py -n 1000 -H 0 -s 0 -c -p FIRST -l ADDRSORT -C
gives average free list size = 3.723

python3 malloc.py -n 1000 -H 0 -s 0 -c -p FIRST -l ADDRSORT
gives average free list size = 40.042

python3 malloc.py -n 1000 -H 0 -s 0 -c -p FIRST -l SIZESORT+ -C
gives average free list size = 26.349

python3 malloc.py -n 1000 -H 0 -s 0 -c -p FIRST -l SIZESORT+
gives average free list size = 29.734

python3 malloc.py -n 1000 -H 0 -s 0 -c -p FIRST -l SIZESORT- -C
gives average free list size = 68.293

python3 malloc.py -n 1000 -H 0 -s 0 -c -p FIRST -l SIZESORT-
gives average free list size = 77.016
```
We can infer that best-fit policy with ADDRSORT and coalescing gives the least fragmentation, and all policies with ADDRSORT have relatively low fragmentation, in some cases, coalescing will make fragmentation worse.

### Q6:What happens when you change the percent allocated fraction -P to higher than 50? What happens to allocations as it nears 100? What about as it nears 0?
A6: change the script a bit
```
import os
import itertools

basic_command = "python3 malloc.py -n 1000 -H 0 -s 0 -c"
command_option_1 = [
    " -p BEST",
    " -p WORST",
    " -p FIRST"
]
command_option_2 = [
    " -P 1",
    " -P 50",
    " -P 100"
]
commands = []
for item1 in itertools.permutations(command_option_1, 1):
    for item2 in itertools.permutations(command_option_2, 1):
            commands.append(basic_command + item1[0] + item2[0])

for command in commands:
    run_result = os.popen(command).read().strip().split('\n')
    free_list_size = [item.split('Size')[1].split(']')[0].strip() for item in run_result if 'Free List' in item]
    avg_free_list_size = sum(map(int,free_list_size)) / len(free_list_size) if len(free_list_size) != 0 else 0
    print(f'{command}\ngives average free list size = {avg_free_list_size}\n')
```
```
python3 malloc.py -n 1000 -H 0 -s 0 -c -p BEST -P 1
gives average free list size = 18.019

python3 malloc.py -n 1000 -H 0 -s 0 -c -p BEST -P 50
gives average free list size = 29.734

python3 malloc.py -n 1000 -H 0 -s 0 -c -p BEST -P 100
gives average free list size = 0.035

python3 malloc.py -n 1000 -H 0 -s 0 -c -p WORST -P 1
gives average free list size = 80.137

python3 malloc.py -n 1000 -H 0 -s 0 -c -p WORST -P 50
gives average free list size = 78.066

python3 malloc.py -n 1000 -H 0 -s 0 -c -p WORST -P 100
gives average free list size = 0.035

python3 malloc.py -n 1000 -H 0 -s 0 -c -p FIRST -P 1
gives average free list size = 25.013

python3 malloc.py -n 1000 -H 0 -s 0 -c -p FIRST -P 50
gives average free list size = 40.042

python3 malloc.py -n 1000 -H 0 -s 0 -c -p FIRST -P 100
gives average free list size = 0.035
```
It’s clear that the less the -P, the worst the fragmentation, in that the lower P leads to more free ops, which will tear the memory space into many smaller free chunks, causing severe fragmentation.
### Q7:What kind of specific requests can you make to generate a highly- fragmented free space? Use the -A flag to create fragmented free lists, and see how different policies and options change the organization of the free list.
A7: the frequent size ascending allocation and free will lead to a highly fragmented free space.
e.g. -A +1,-0,+2,-1,+3,-2,+4,-3,+5,-4
```
import os
import itertools

basic_command = "python3 malloc.py -A +1,-0,+2,-1,+3,-2,+4,-3,+5,-4 -H 0 -c"
command_option_1 = [
    " -p BEST",
    " -p WORST",
    " -p FIRST"
]

command_option_2 = [
    " -l ADDRSORT",
    " -l SIZESORT+",
    " -l SIZESORT-"
]

commands = []

for item1 in itertools.permutations(command_option_1, 1):
    for item2 in itertools.permutations(command_option_2, 1):
            commands.append(basic_command + item1[0] + item2[0])

for command in commands:
    run_result = os.popen(command).read().strip().split('\n')
    free_list_size = [item.split('Size')[1].split(']')[0].strip() for item in run_result if 'Free List' in item]
    avg_free_list_size = sum(map(int,free_list_size)) / len(free_list_size) if len(free_list_size) != 0 else 0
    print(f'{command}\ngives average free list size = {avg_free_list_size}\n')

```
gives
```
python3 malloc.py -A +1,-0,+2,-1,+3,-2,+4,-3,+5,-4 -H 0 -c -p BEST -l ADDRSORT
gives average free list size = 3.5

python3 malloc.py -A +1,-0,+2,-1,+3,-2,+4,-3,+5,-4 -H 0 -c -p BEST -l SIZESORT+
gives average free list size = 3.5

python3 malloc.py -A +1,-0,+2,-1,+3,-2,+4,-3,+5,-4 -H 0 -c -p BEST -l SIZESORT-
gives average free list size = 3.5

python3 malloc.py -A +1,-0,+2,-1,+3,-2,+4,-3,+5,-4 -H 0 -c -p WORST -l ADDRSORT
gives average free list size = 3.5

python3 malloc.py -A +1,-0,+2,-1,+3,-2,+4,-3,+5,-4 -H 0 -c -p WORST -l SIZESORT+
gives average free list size = 3.5

python3 malloc.py -A +1,-0,+2,-1,+3,-2,+4,-3,+5,-4 -H 0 -c -p WORST -l SIZESORT-
gives average free list size = 3.5

python3 malloc.py -A +1,-0,+2,-1,+3,-2,+4,-3,+5,-4 -H 0 -c -p FIRST -l ADDRSORT
gives average free list size = 3.5

python3 malloc.py -A +1,-0,+2,-1,+3,-2,+4,-3,+5,-4 -H 0 -c -p FIRST -l SIZESORT+
gives average free list size = 3.5

python3 malloc.py -A +1,-0,+2,-1,+3,-2,+4,-3,+5,-4 -H 0 -c -p FIRST -l SIZESORT-
gives average free list size = 3.5
```
It seems policy and free list order have no effect on the average free list size given by the previous -A operation sequences, but it can be thought of that with -C flag the fragmentation will be improved.