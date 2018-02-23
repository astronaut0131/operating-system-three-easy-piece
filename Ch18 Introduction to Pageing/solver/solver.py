import os
from collections import namedtuple

def split_VA_decimal(decimals):
    bins = list(map(lambda x: bin(x)[2:], decimals))
    VPN = list(map(lambda x: int(x[:4], 2), bins))
    offset = list(map(lambda x: x[4:], bins))
    return offset, VPN


def split_page_table(table):
    valid_bit = list(map(lambda x: x[0], table))
    PFN = list(map(lambda x: x[1:] if len(x) > 1 else x[0], table))
    return valid_bit, PFN


for u in range(5):
    output = os.popen(f'python paging-linear-translate.py -P 1k -a 16k -p 32k -v -u {25*u}').read().split('\n')
    find_page_table = lambda line: bin(int(line.split(']')[1].strip()[2:], 16))[2:]
    page_table = [find_page_table(line) for line in output if line.strip().startswith('[')]
    valid_bit, PFN = split_page_table(page_table)
    page_table_entry = namedtuple('Page_Table_Entry','valid_bit PFN')
    page_table_dict = {index:page_table_entry(valid_bit,PFN) for index,(valid_bit,PFN) in enumerate(zip(valid_bit,PFN))}
    find_VA_decimal = lambda line: int(line.split(':')[1].split(')')[0].strip())
    VA_decimal = [find_VA_decimal(line) for line in output if line.strip().startswith('VA')]
    offset, VPN = split_VA_decimal(VA_decimal)
    virtual_address_entry = namedtuple('Virtual_Address','offset VPN')
    virtual_addresses = [virtual_address_entry(item[0],item[1]) for item in zip(offset,VPN)]