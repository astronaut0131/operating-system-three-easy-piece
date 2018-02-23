import os
import matplotlib.pyplot as plt
trials = 500
time_list = []
for index in range(1, 11):
    page_num = 2 ** index
    output = os.popen(f'./test {page_num} {trials}').read()
    time_per_access = int(output) / page_num / trials
    time_list.append(time_per_access * (10 ** 6))
    #print(f"with {page_num} pages, time per access is {time_per_access * (10 ** 6)}ns")
plt.plot([2 ** index for index in range(1,11)],time_list)
plt.show()