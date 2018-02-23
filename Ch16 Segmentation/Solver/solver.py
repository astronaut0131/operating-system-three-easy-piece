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
