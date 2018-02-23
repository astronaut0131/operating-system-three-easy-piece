# Ch18 Homework
### Q1:Before doing any translations, let’s use the simulator to study how linear page tables change size given different parameters. Compute the size of linear page tables as different parameters change. Some suggested inputs are below; by using the -v flag, you can see how many page-table entries are filled.
### First, to understand how linear page table size changes as the address space grows:
```
paging-linear-translate.py -P 1k -a 1m -p 512m -v -n 0paging-linear-translate.py -P 1k -a 2m -p 512m -v -n 0paging-linear-translate.py -P 1k -a 4m -p 512m -v -n 0
```
### Then, to understand how linear page table size changes as page size grows:
```
paging-linear-translate.py -P 1k -a 1m -p 512m -v -n 0paging-linear-translate.py -P 2k -a 1m -p 512m -v -n 0paging-linear-translate.py -P 4k -a 1m -p 512m -v -n 0
```
### Before running any of these, try to think about the expected trends. How should page-table size change as the address space grows? As the page size grows? Why shouldn’t we just use really big pages in general?

A1: Linear page table size grows as the address space grows, in that the larger the address space is, the more pages there will be if the page size stays unchanged.Linear page table size decrease as the page size grows, in that page numbers = address space size / page size, so when the page size grows , the number of pages will decrease, thus leads to a smaller page table. I think using really big pages will lead to too large offset bits, fragmentation.
`paging-linear-translate.py -P 1k -a 1m -p 512m -v -n 0`
Gives 1024 entries
`paging-linear-translate.py -P 1k -a 2m -p 512m -v -n 0`
Gives 2048 entries
`paging-linear-translate.py -P 1k -a 4m -p 512m -v -n 0`
Gives 4096 entries
`paging-linear-translate.py -P 1k -a 1m -p 512m -v -n 0`
Gives 1024 entries
`paging-linear-translate.py -P 2k -a 1m -p 512m -v -n 0`
Gives 512 entries
`paging-linear-translate.py -P 4k -a 1m -p 512m -v -n 0`
Gives 256 entries

### Q2:Now let’s do some translations. Start with some small examples, and change the number of pages that are allocated to the address space with the -u flag. For example:
```
paging-linear-translate.py -P 1k -a 16k -p 32k -v -u 0paging-linear-translate.py -P 1k -a 16k -p 32k -v -u 25paging-linear-translate.py -P 1k -a 16k -p 32k -v -u 50paging-linear-translate.py -P 1k -a 16k -p 32k -v -u 75paging-linear-translate.py -P 1k -a 16k -p 32k -v -u 100
```
### What happens as you increase the percentage of pages that are allocated in each address space?
A2: in that all the address space is 16k and page size is 1k, we need 16 pages, thus 2 bits (0~15) to stand for each page, page size = 1k = 1024, thus  10 bits offset (0 ~1023) to stand for every byte in a page.
write a python script to do the job!
```

```