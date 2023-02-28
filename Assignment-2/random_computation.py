import time
import os
result = 0

print("PID: ", os.getpid())
while True:
    # Perform a computation-intensive task
    result = result + 1
    # print("Result: ", result)
