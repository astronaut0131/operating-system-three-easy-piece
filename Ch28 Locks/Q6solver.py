
import os
command_list = ['python x86.py -p test-and-set.s -a bx=10,bx=10 -i '+str(i) for i in range(1,24)]
command_list.append('python x86.py -p test-and-set.s -a bx=10,bx=10')
command_list.append('python x86.py -p test-and-set.s -a bx=20,bx=0')
for command in command_list:
	output = os.popen(command).read().strip().split('\n')
	output = [item.strip() for item in output]
	output = list(filter(lambda line:'Interrupt' not in line and 'ARG' not in line and line != '',output))
	print(command+' gives '+str(len(output)))
