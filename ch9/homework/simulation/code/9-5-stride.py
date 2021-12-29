import matplotlib.pyplot as plt
import numpy as np

def run_once(job_1_ticket_ratio):
    # joblist = [(id, length, ticket, pass)]
    joblist = [(0, 100, 100*job_1_ticket_ratio, 0), (1, 100, 100*(1-job_1_ticket_ratio), 0)]
    stride_1 = 100000 / joblist[0][2]
    stride_2 = 100000 / joblist[1][2]
    run_total = 200
    quantum = 1
    clock = 0
    for i in range(run_total):
        if (joblist[0][3] <= joblist[1][3]):
            (wjob, wrun, wtix, wpass) = joblist[0]
            wpass += stride_1
        else:
            (wjob, wrun, wtix, wpass) = joblist[1]
            wpass += stride_2
        
        if wrun >= quantum:
            wrun -= quantum
        else:
            wrun = 0
        
        clock += quantum
        if wrun == 0:
            break
        
        joblist[wjob] = (wjob, wrun, wtix, wpass)

    return clock / run_total

def run_ticket_ratio_range(ratio_arr):
    fairness_arr = []
    for ratio in ratio_arr:
        fairness_arr.append(run_once(ratio))
    return fairness_arr

min_ratio = 0.01
max_ratio = 1.00
step = 0.01
ratio_arr = np.arange(min_ratio, max_ratio, step)

fairness_arr = run_ticket_ratio_range(ratio_arr)

print(run_once(0.05))

plt.plot(ratio_arr, fairness_arr)
plt.ylim(0, 1)
plt.xlabel('Ratio of Job 1 tickets in total tickets')
plt.ylabel('Fairness')
plt.title('Fairness with ticket ratio')
plt.savefig('9-5-stride.png')
plt.show()
