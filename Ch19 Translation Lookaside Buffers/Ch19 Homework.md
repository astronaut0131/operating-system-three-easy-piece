# Ch19 Homework
### Q1:For timing, you’ll need to use a timer such as that made available by gettimeofday(). How precise is such a timer? How long does an operation have to take in order for you to time it precisely? (this will help determine how many times, in a loop, you’ll have to repeat a page access in order to time it successfully)
A1: It can be accurate to microsecond.An operation have to take >= 1 ms.
### Q2:Write the program, called tlb.c, that can roughly measure the cost of accessing each page. Inputs to the program should be: the number of pages to touch and the number of trials.
A2: very good code from [ostep/tlb.c at master · asnr/ostep · GitHub](https://github.com/asnr/ostep/blob/master/virtualisation/19_tlb/tlb.c)
Q3:Now write a script in your favorite scripting language(csh,python, etc.) to run this program, while varying the number of pages accessed from 1 up to a few thousand, perhaps incrementing by a factor of two per iteration. Run the script on different machines and gather some data. How many trials are needed to get reliable measurements?
A3: on Mac OS X there is no sched.h to set CPU affinity for the process, so a better idea is to run the c program on Linux virtual machine.
Then write the python script
```
import os
for page_num in range(1,2000):
    output = os.popen(f'./tlb {page_num} 150').read()
    print(f"{page_num} pages with 150 trials gives a total time of {output}ms")
```
Gives

### Q4:Next, graph the results, making a graph that looks similar to the one above. Use a good tool like ploticus or even zplot. Visualization usually makes the data much easier to digest; why do you think that is?
A4: use python matplotlib
```

```