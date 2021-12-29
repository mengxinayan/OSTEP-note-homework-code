import random
import matplotlib.pyplot as plt
import numpy as np
import math

def run_once(quantum):
    joblist = [[0, 100, 100], [1, 100, 100]]
    run_total = math.ceil(200/quantum) * quantum
    ticket_total = 200
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

def run_quantum_range(quantum_arr):
    fairness_arr = []
    for quantum in quantum_arr:
        fairness_arr.append(run_once(quantum))
    return fairness_arr

min_quantum = 1
max_quantum = 100
quantum_arr = [i for i in range(min_quantum, max_quantum+1)]
fairness_total_arr = []
max_random_times = 5000

for i in range(max_random_times):
    fairness_total_arr.append(run_quantum_range(quantum_arr))

avg_fairness_arr = np.average(fairness_total_arr, axis=0)

plt.plot(quantum_arr, avg_fairness_arr)
plt.ylim(0, 1)
plt.xlabel('Quantum Length')
plt.ylabel('Fairness (Average)')
plt.title('Fairness with Quantum')
plt.savefig('9-4-quantum.png')
plt.show()
