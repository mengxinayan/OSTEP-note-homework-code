import random
import matplotlib.pyplot as plt
import numpy as np

def run_once(length):
    joblist = [(0, length, 100), (1, length, 100)]
    run_total = length * 2
    ticket_total = 200
    quantum = 1
    clock = 0
    for i in range(run_total):
        r = int(random.random() * 1000001)
        winner = int(r % ticket_total)
        current = 0
        for (job, runtime, tickets) in joblist:
            current += tickets
            if current > winner:
                (wjob, wrun, wtix) = (job, runtime, tickets)
                break
        if wrun >= quantum:
            wrun -= quantum
        else:
            wrun = 0

        clock += quantum
        if wrun == 0:
            # one job finish
            break
        
        joblist[wjob] = (wjob, wrun, wtix)
        
    return clock / run_total

def run_length_range(min_length, max_length):
    fairness_arr = []
    for length in range(min_length, max_length+1, 10):
        fairness_arr.append(run_once(length))
    return fairness_arr

min_length = 1
max_length = 1000
length_arr = [i for i in range(min_length, max_length+1, 10)]
fairness_total_arr = []
max_random_times = 100

for i in range(max_random_times):
    fairness_total_arr.append(run_length_range(min_length, max_length))

avg_fairness_arr = np.average(fairness_total_arr, axis=0)

plt.plot(length_arr, avg_fairness_arr)
plt.ylim(0, 1)
plt.xlabel('Job Length')
plt.ylabel('Fairness (Average)')
plt.title('Fairness with Job Length')
plt.savefig('9-5-length.png')
plt.show()
