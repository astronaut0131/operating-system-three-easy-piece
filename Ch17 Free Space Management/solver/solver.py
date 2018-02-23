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