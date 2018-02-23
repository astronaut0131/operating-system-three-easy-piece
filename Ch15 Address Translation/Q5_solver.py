import os
import matplotlib.pyplot as plt
for seed in range(1):
    valid_rate = []
    limit_list = []
    result = os.popen('python relocation.py -s {}'.format(seed)).read().split('\n')
    result = [item for item in result if item]
    limit_range = [int(item.split('ARG address space size ')[1].replace('k','')) for item in result if item.startswith(
        "ARG address space size ")][0] * 1024
    for limit in range(limit_range+1):
        command_result = os.popen('python relocation.py -s {} -l {} -c'.format(seed,limit)).read().split('\n')
        VA_result = [item for item in command_result if item and 'VA' in item]
        valid = len([item for item in VA_result if 'VALID' in item])
        valid_rate.append(valid/len(VA_result))
        limit_list.append(limit)
        print(len(valid_rate))
    plt.plot(valid_rate,limit_list)
    plt.xlabel("valid rate")
    plt.ylabel("limit")
    plt.show()