# Ch16 Homework
### Q1:1. First let’s use a tiny address space to translate some addresses.Here’s a simple set of parameters with a few different random seeds; can you translate the addresses?
A1:
	write a python script to solve it:
```
import os


def get_top(number, bit_length):
    binary_length = len(bin(number)) - 2
    top_mask = int("1" + "0" * (bit_length - 2), 2)
    top_bit = (number & top_mask) >> (binary_length - 1)
    return top_bit


def get_address_space_size(output_list):
    size = output_list[1].split('ARG address space size ')[1].strip()
    if 'k' in size:
        size = int(size.split('k')[0]) * 1024
    else:
        size = int(size)
    return size


def get_segment_dict(output_list):
    seg_dict = {}
    temp = {}
    for item in output_list:
        if item.startswith('  Segment '):
            if 'base' in item:
                seg_num = int(item.split('Segment ')[1].split(' base')[0])
                seg_base = int(item.split('decimal ')[1].split(')')[0])
                temp['base'] = seg_base
            elif 'limit' in item:
                seg_num = int(item.split('Segment ')[1].split(' limit')[0])
                seg_limit = int(item.split(': ')[1])
                temp['limit'] = seg_limit
                seg_dict[seg_num] = temp
                temp = {}
    return seg_dict


def get_va_problems(output_list):
    return [int(item.strip().split('decimal:')[1].split(')')[0].strip()) for item in output_list if
            item.strip().startswith("VA")]


def get_output_list(seed_num):
    output_list = os.popen(
        'python segmentation.py -a 128 -p 512 -b 0 -l 20 -B 512 -L 20 -s {}'.format(seed_num)).read().strip().split(
        '\n')
    return list(filter(lambda x: x != '', output_list))


def translate_virtual_address(seg, val, seg_dict, max_address_space_size):
    if seg == 0:
        if val > seg_dict[0]['limit']:
            return "segmentation violation"
        else:
            return val + seg_dict[0]['base']
    else:
        val = max_address_space_size - val
        if val > seg_dict[1]['limit']:
            return "segmentation violation"
        else:
            return seg_dict[1]['base'] - val


for seed in range(3):
    print("seed {} solution".format(seed))
    output = get_output_list(seed)
    segment_dict = get_segment_dict(output)
    address_space_size = get_address_space_size(output)
    bin_address_space_len = len(bin(address_space_size)) - 2
    VA = get_va_problems(output)
    segment_num = [get_top(item, bin_address_space_len) for item in VA]
    result = []
    for i in range(len(segment_num)):
        result.append(translate_virtual_address(segment_num[i], VA[i], segment_dict, address_space_size))
    for index, item in enumerate(result):
        print("Physical Address " + str(index) + ":     " + str(item) + (lambda x: "(hex:" + hex(x)+")" if type(x) == int else "")(item))
```
Gives
```
seed 0 solution
Physical Address 0:     492(hex:0x1ec)
Physical Address 1:     segmentation violation
Physical Address 2:     segmentation violation
Physical Address 3:     segmentation violation
Physical Address 4:     segmentation violation
seed 1 solution
Physical Address 0:     17(hex:0x11)
Physical Address 1:     492(hex:0x1ec)
Physical Address 2:     segmentation violation
Physical Address 3:     segmentation violation
Physical Address 4:     segmentation violation
seed 2 solution
Physical Address 0:     506(hex:0x1fa)
Physical Address 1:     505(hex:0x1f9)
Physical Address 2:     7(hex:0x7)
Physical Address 3:     10(hex:0xa)
Physical Address 4:     segmentation violation
```

### Q2:Now, let’s see if we understand this tiny address space we’ve constructed (using the parameters from the question above). What is the highest legal virtual address in segment 0? What about the lowest legal virtual address in segment 1? What are the lowest and highest illegal addresses in this entire address space? Finally, how would you run segmentation.py with the -A flag to test if you are right?
A2: the highest legal virtual address in segment 0 is base(seg0) + limit(seg0)  -1 = 0 + 20 -1= 19, the lowest legal virtual address in segment 1 is address space size - limit(seg1) +1= 128 - 20 +1 = 109.But after test 108 is the lowest legal virtual address is segment1
```
➜  HW-Segmentation python segmentation.py -a 128 -p 512 -b 0 -l 20 -B 512 -L 20 -A19,20,107,108,109 -c
ARG seed 0
ARG address space size 128
ARG phys mem size 512

Segment register information:

  Segment 0 base  (grows positive) : 0x00000000 (decimal 0)
  Segment 0 limit                  : 20

  Segment 1 base  (grows negative) : 0x00000200 (decimal 512)
  Segment 1 limit                  : 20

Virtual Address Trace
  VA  0: 0x00000013 (decimal:   19) --> VALID in SEG0: 0x00000013 (decimal:   19)
  VA  1: 0x00000014 (decimal:   20) --> SEGMENTATION VIOLATION (SEG0)
  VA  2: 0x0000006b (decimal:  107) --> SEGMENTATION VIOLATION (SEG1)
  VA  3: 0x0000006c (decimal:  108) --> VALID in SEG1: 0x000001ec (decimal:  492)
  VA  4: 0x0000006d (decimal:  109) --> VALID in SEG1: 0x000001ed (decimal:  493)
```
currently I don’t know what caused the asymmetry.
In the perspective of the entire address space, the lowest illegal address is 20 and the highest illegal address is 107.

### Q3:Let’s say we have a tiny 16-byte address space in a 128-byte physical memory. What base and bounds would you set up so as to get the simulator to generate the following translation results for the specified address stream: valid, valid, violation, ..., violation, valid, valid? Assume the following parameters:      segmentation.py -a 16 -p 128        -A 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15        --b0 ? --l0 ? --b1 ? --l1 ?
A3: —b0 0 —l0 2 —b1 16 —l1 2
```
➜  HW-Segmentation python segmentation.py -a 16 -p 128 -A 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 --b0 0 --l0 2 --b1 16 --l1 2 -c
```
Gives
```
Virtual Address Trace
  VA  0: 0x00000000 (decimal:    0) --> VALID in SEG0: 0x00000000 (decimal:    0)
  VA  1: 0x00000001 (decimal:    1) --> VALID in SEG0: 0x00000001 (decimal:    1)
  VA  2: 0x00000002 (decimal:    2) --> SEGMENTATION VIOLATION (SEG0)
  VA  3: 0x00000003 (decimal:    3) --> SEGMENTATION VIOLATION (SEG0)
  VA  4: 0x00000004 (decimal:    4) --> SEGMENTATION VIOLATION (SEG0)
  VA  5: 0x00000005 (decimal:    5) --> SEGMENTATION VIOLATION (SEG0)
  VA  6: 0x00000006 (decimal:    6) --> SEGMENTATION VIOLATION (SEG0)
  VA  7: 0x00000007 (decimal:    7) --> SEGMENTATION VIOLATION (SEG0)
  VA  8: 0x00000008 (decimal:    8) --> SEGMENTATION VIOLATION (SEG1)
  VA  9: 0x00000009 (decimal:    9) --> SEGMENTATION VIOLATION (SEG1)
  VA 10: 0x0000000a (decimal:   10) --> SEGMENTATION VIOLATION (SEG1)
  VA 11: 0x0000000b (decimal:   11) --> SEGMENTATION VIOLATION (SEG1)
  VA 12: 0x0000000c (decimal:   12) --> SEGMENTATION VIOLATION (SEG1)
  VA 13: 0x0000000d (decimal:   13) --> SEGMENTATION VIOLATION (SEG1)
  VA 14: 0x0000000e (decimal:   14) --> VALID in SEG1: 0x0000000e (decimal:   14)
  VA 15: 0x0000000f (decimal:   15) --> VALID in SEG1: 0x0000000f (decimal:   15)
```

### Q4:Assuming we want to generate a problem where roughly 90% of the randomly-generated virtual addresses are valid (i.e., not segmentation violations). How should you configure the simulator to do so? Which parameters are important?
A4: the unallocated (illegal) space must <= 10% of the address space size. Combined with Q2, low illegal address is base(seg0) + limit , highest illegal address is address space size - limit(seg1) - 1, so (highest illegal address - low illegal address) / address space size must <= 10%.

### Q5:Can you run the simulator such that no virtual addresses are valid? How?
A5: by getting the limit = 0
e.g.
```
➜  HW-Segmentation python segmentation.py -b 0 -l 0 -B 512 -L 0 -c
ARG seed 0
ARG address space size 1k
ARG phys mem size 16k

Segment register information:

  Segment 0 base  (grows positive) : 0x00000000 (decimal 0)
  Segment 0 limit                  : 0

  Segment 1 base  (grows negative) : 0x00000200 (decimal 512)
  Segment 1 limit                  : 0

Virtual Address Trace
  VA  0: 0x00000360 (decimal:  864) --> SEGMENTATION VIOLATION (SEG1)
  VA  1: 0x00000308 (decimal:  776) --> SEGMENTATION VIOLATION (SEG1)
  VA  2: 0x000001ae (decimal:  430) --> SEGMENTATION VIOLATION (SEG0)
  VA  3: 0x00000109 (decimal:  265) --> SEGMENTATION VIOLATION (SEG0)
  VA  4: 0x0000020b (decimal:  523) --> SEGMENTATION VIOLATION (SEG1)
```