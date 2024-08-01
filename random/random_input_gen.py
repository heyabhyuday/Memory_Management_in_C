from time import time
from numpy import random
random.seed(int(time()))


NUM_PROCESS = 256

################
p_id = []
count = 0
while count < NUM_PROCESS:
    id = ""
    for _ in range(3):
        id += str(random.randint(0, 10000))

    p_id.append(id)
    count += 1

################
process_name = []
count = 0
while count < NUM_PROCESS:
    length = random.randint(6, 11)
    name = ""
    for _ in range(length):
        name += chr(random.randint(97, 123))

    process_name.append(name)
    count += 1

################
process_size = []
count = 0
while count < NUM_PROCESS:
    process_size.append(str(random.randint(16,1000)))
    count += 1

################
dummy = [0] * NUM_PROCESS

################
priority = []
count = 0
while count < NUM_PROCESS:
    priority.append(str(random.randint(0,101)))
    count += 1

################
segment = []
for size in process_size:

    seg = []
    
    num_segments = random.randint(1, 9)
    
    total = 0
    count = 0
    while True:
        try:
            curr_segment = random.randint(17, int(size)-total)
        except:
            curr_segment = int(size) - total

        total += curr_segment
        count += 1

        if total == int(size):
            num_segments = count
            seg.append(curr_segment)
            break
        
        if count == num_segments:
            if total != int(size):
                curr_segment += int(size) - total
            seg.append(curr_segment)
            break
            
        seg.append(curr_segment)

    random.shuffle(seg)

    final = ""
    seg_count = 0
    while True:
        final += str(seg[seg_count])
        seg_count += 1
        
        if seg_count == num_segments:
            break
        
        final += ','
    
    segment.append(final)
    
################
input1 = open("input1.txt", 'w')
for i in range(NUM_PROCESS):
    input1.write(f"{p_id[i]} {process_name[i]} {process_size[i]} {dummy[i]}\n")
input1.close()

input2 = open("input2.txt", 'w')
for i in range(NUM_PROCESS):
    input2.write(f"{p_id[i]} {process_name[i]} {process_size[i]} {dummy[i]}\n")
input2.close()

input3 = open("input3.txt", 'w')
for i in range(NUM_PROCESS):
    input3.write(f"{p_id[i]} {process_name[i]} {process_size[i]} {priority[i]}\n")
input3.close()

input4 = open("input4.txt", 'w')
for i in range(NUM_PROCESS):
    input4.write(f"{p_id[i]} {process_name[i]} {process_size[i]} {segment[i]}\n")
input4.close()