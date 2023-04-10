import matplotlib.pyplot as plt


shares_1=8192
periods=1000000
quota_1 = []
time_elapsed_1 = []
nr_periods_1 = []
nr_throttles_1 = []
throttles_time_1 = []

shares_2=512
quota_2 = []
time_elapsed_2 = []
nr_periods_2 = []
nr_throttles_2 = []
throttles_time_2 = []

# read data from "results.txt"
with open('result1.txt', 'r') as f:
    for line in f:
        line = line.split()
        if line[0] == 'PID' or line[0] == 'period':
            continue
        elif line[0] == 'Running':
            quota_1.append(int(line[3]))
        elif line[0] == 'Elapsed':
            time_elapsed_1.append(float(line[2]))
        elif line[0] == 'nr_periods':
            nr_periods_1.append(int(line[1]))
        elif line[0] == 'nr_throttled':
            nr_throttles_1.append(int(line[1]))
        elif line[0] == 'throttled_time':
            throttles_time_1.append(int(line[1])/1e9)

# read data from "results.txt"
with open('result2.txt', 'r') as f:
    for line in f:
        line = line.split()
        if line[0] == 'PID' or line[0] == 'period':
            continue
        elif line[0] == 'Running':
            quota_2.append(int(line[3]))
        elif line[0] == 'Elapsed':
            time_elapsed_2.append(float(line[2]))
        elif line[0] == 'nr_periods':
            nr_periods_2.append(int(line[1]))
        elif line[0] == 'nr_throttled':
            nr_throttles_2.append(int(line[1]))
        elif line[0] == 'throttled_time':
            throttles_time_2.append(int(line[1])/1e9)



# plot graphs
fig, ax = plt.subplots(2, 2, figsize=(16, 14))
ax[0, 0].plot(quota_1,  time_elapsed_1, 'o-', label='shares={}'.format(shares_1))
ax[0, 0].plot(quota_2,  time_elapsed_2, 'o-', label='shares={}'.format(shares_2))
ax[0, 0].set_title('Execution time vs quota(periods={} microseconds)'.format(periods))
ax[0, 0].set_xlabel('quota(microseconds)')
ax[0, 0].set_ylabel('Execution time(seconds)')
ax[0, 0].legend()
ax[0, 0].grid(True)

ax[0, 1].plot(quota_1, nr_periods_1, 'o-', label='shares={}'.format(shares_1))
ax[0, 1].plot(quota_2, nr_periods_2, 'o-', label='shares={}'.format(shares_2))
ax[0, 1].set_title('nr_periods vs quota(periods={} microseconds)'.format(periods))
ax[0, 1].set_xlabel('quota(microseconds)')
ax[0, 1].set_ylabel('nr_periods')
ax[0, 1].grid(True)
ax[0, 1].legend()

ax[1, 0].plot(quota_1, nr_throttles_1, 'o-', label='shares={}'.format(shares_1))
ax[1, 0].plot(quota_2, nr_throttles_2, 'o-', label='shares={}'.format(shares_2))
ax[1, 0].set_title('nr_throttles vs quota(periods={} microseconds)'.format(periods))
ax[1, 0].set_xlabel('quota(microseconds)')
ax[1, 0].set_ylabel('nr_throttles')
ax[1, 0].grid(True)
ax[1, 0].legend()

ax[1, 1].plot(quota_1, throttles_time_1, 'o-', label='shares={}'.format(shares_1))
ax[1, 1].plot(quota_2, throttles_time_2, 'o-', label='shares={}'.format(shares_2))
ax[1, 1].set_title('throttles_time vs quota(periods={} microseconds)'.format(periods))
ax[1, 1].set_xlabel('quota(microseconds)')
ax[1, 1].set_ylabel('throttles_time(seconds)')
ax[1, 1].grid(True)
ax[1, 1].legend()
plt.savefig('graph.png')