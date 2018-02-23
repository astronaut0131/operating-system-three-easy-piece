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
