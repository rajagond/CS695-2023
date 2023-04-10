import matplotlib.pyplot as plt

shares=1024
periods=1000000
quota = []
time_elapsed = []
nr_periods = []
nr_throttles = []
throttles_time = []

# read data from "results.txt"
with open('result_part2.txt', 'r') as f:
    for line in f:
        line = line.split()
        if line[0] == 'Running':
            quota.append(int(line[3]))
        elif line[0] == 'Elapsed':
            time_elapsed.append(float(line[1]))
        elif line[0] == 'nr_periods':
            nr_periods.append(int(line[1]))
        elif line[0] == 'nr_throttled':
            nr_throttles.append(int(line[1]))
        elif line[0] == 'throttled_time':
            throttles_time.append(int(line[1])/1e9)



# plot graphs
fig, ax = plt.subplots(2, 2, figsize=(16, 14))
ax[0, 0].plot(quota,  time_elapsed, 'o-', label='time_elapsed')
ax[0, 0].set_title('Execution time vs quota(shares={}, periods={} microseconds)'.format(shares, periods))
ax[0, 0].set_xlabel('quota(microseconds)')
ax[0, 0].set_ylabel('Execution time(seconds)')
ax[0, 0].grid(True)

ax[0, 1].plot(quota, nr_periods, 'o-', label='nr_periods')
ax[0, 1].set_title('nr_periods vs quota(shares={}, periods={} microseconds)'.format(shares, periods))
ax[0, 1].set_xlabel('quota(microseconds)')
ax[0, 1].set_ylabel('nr_periods')
ax[0, 1].grid(True)

ax[1, 0].plot(quota, nr_throttles, 'o-', label='nr_throttles')
ax[1, 0].set_title('nr_throttles vs quota(shares={}, periods={} microseconds)'.format(shares, periods))
ax[1, 0].set_xlabel('quota(microseconds)')
ax[1, 0].set_ylabel('nr_throttles')
ax[1, 0].grid(True)

ax[1, 1].plot(quota, throttles_time, 'o-', label='throttles_time')
ax[1, 1].set_title('throttles_time vs quota(shares={}, periods={} microseconds)'.format(shares, periods))
ax[1, 1].set_xlabel('quota(microseconds)')
ax[1, 1].set_ylabel('throttles_time(seconds)')
ax[1, 1].grid(True)
plt.savefig('graph.png')