import random
import time

with open('output.txt', 'w') as f:
    num = 1
    while True:
        f.write('Hello World ' + str(num) + '\n')
        f.flush()  # flush the buffer to ensure immediate writing to file
        time.sleep(1)
        num = num + 1
