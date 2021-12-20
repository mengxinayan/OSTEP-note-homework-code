# ch4 Processes Homework and Solution

If you just want to read the answers, that is a simple version for solutions, please visit [here](../solution.md).

## Homework (Simulation)

This program, [`process-run.py`](../code/ch4/homework-simulation/process-run.py), allows you to see how process states change as programs run and either use the CPU (e.g., perform an add instruction) or do I/O (e.g., send a request to a disk and wait for it to complete). See the [README](../code/ch4/homework-simulation/README.md) for details.

## Questions and Solutions

### 4.1

1. Run `process-run.py` with the following flags: `-l 5:100,5:100`. What should the CPU utilization be (e.g., the percent of time the CPU is in use?) Why do you know this? Use the `-c` and `-p` flags to
see if you were right.

Process 0: 50%; Process 1: 50%; Total : 100%

```console
$ python3 process-run.py -l 5:100,5:100 -c -p
Time        PID: 0        PID: 1           CPU           IOs
  1        RUN:cpu         READY             1          
  2        RUN:cpu         READY             1          
  3        RUN:cpu         READY             1          
  4        RUN:cpu         READY             1          
  5        RUN:cpu         READY             1          
  6           DONE       RUN:cpu             1          
  7           DONE       RUN:cpu             1          
  8           DONE       RUN:cpu             1          
  9           DONE       RUN:cpu             1          
 10           DONE       RUN:cpu             1          

Stats: Total Time 10
Stats: CPU Busy 10 (100.00%)
Stats: IO Busy  0 (0.00%)
```

### 4.2

2. Now run with these flags: `./process-run.py -l 4:100,1:0`. These flags specify one process with 4 instructions (all to use the CPU), and one that simply issues an I/O and waits for it to be done.How long does it take to complete both processes? Use `-c` and `-p` to find out if you were right.

Time is 11 = 4(RUN:cpu) + 1(RUN:io) + 5(WAITING) + 1(RUN:io_done). When process 0 finishes, then process 1 will start. And IO takes 5 tricks(default).

```console
$ python3 process-run.py -l 4:100,1:0 -c -p
Time        PID: 0        PID: 1           CPU           IOs
  1        RUN:cpu         READY             1          
  2        RUN:cpu         READY             1          
  3        RUN:cpu         READY             1          
  4        RUN:cpu         READY             1          
  5           DONE        RUN:io             1          
  6           DONE       WAITING                           1
  7           DONE       WAITING                           1
  8           DONE       WAITING                           1
  9           DONE       WAITING                           1
 10           DONE       WAITING                           1
 11*          DONE   RUN:io_done             1          

Stats: Total Time 11
Stats: CPU Busy 6 (54.55%)
Stats: IO Busy  5 (45.45%)
```

### 4.3

3. Switch the order of the processes: `-l 1:0,4:100`. What happens now? Does switching the order matter? Why? (As always, use `-c` and `-p` to see if you were right)

Time is 7 = 1(RUN:io) + 5(WAITING && RUN:cpu) + 1(RUN:io_done). Because process 1 will use CPU when process 0 waiting I/O.

```console
$ python3 process-run.py -l 1:0,4:100 -c -p                 
Time        PID: 0        PID: 1           CPU           IOs
  1         RUN:io         READY             1          
  2        WAITING       RUN:cpu             1             1
  3        WAITING       RUN:cpu             1             1
  4        WAITING       RUN:cpu             1             1
  5        WAITING       RUN:cpu             1             1
  6        WAITING          DONE                           1
  7*   RUN:io_done          DONE             1          

Stats: Total Time 7
Stats: CPU Busy 6 (85.71%)
Stats: IO Busy  5 (71.43%)
```

### 4.4

4. We’ll now explore some of the other flags. One important flag is `-S`, which determines how the system reacts when a process issues an I/O. With the flag set to SWITCH ON END, the system will NOT switch to another process while one is doing I/O, instead waiting until the process is completely finished. What happens when you run the following two processes (`-l 1:0,4:100 -c -S SWITCH_ON_END`), one doing I/O and the other doing CPU work?

Time will be 11 = 1(RUN:io) + 5(WAITING) + 1(RUN:io_done) + 4(RUN:cpu). Because the system will NOT switch to another process while one is doing I/O, it will start process 1 when process 0 ends.

```console
$ python3 process-run.py -l 1:0,4:100 -c -p -S SWITCH_ON_END
Time        PID: 0        PID: 1           CPU           IOs
  1         RUN:io         READY             1          
  2        WAITING         READY                           1
  3        WAITING         READY                           1
  4        WAITING         READY                           1
  5        WAITING         READY                           1
  6        WAITING         READY                           1
  7*   RUN:io_done         READY             1          
  8           DONE       RUN:cpu             1          
  9           DONE       RUN:cpu             1          
 10           DONE       RUN:cpu             1          
 11           DONE       RUN:cpu             1          

Stats: Total Time 11
Stats: CPU Busy 6 (54.55%)
Stats: IO Busy  5 (45.45%)
```

### 4.5

5. Now, run the same processes, but with the switching behavior set to switch to another process whenever one is WAITING for I/O (`-l 1:0,4:100 -c -S SWITCH_ON_IO`). What happens now? Use `-c` and `-p` to confirm that you are right.

Time is 7 = 1(RUN:io) + 5(WAITING && RUN:cpu) + 1(RUN:io_done). Because the system will switch another process while one is doing I/O, so process 1 will run during process doing I/O.

```console
$ python3 process-run.py -l 1:0,4:100 -c -p -S SWITCH_ON_IO 
Time        PID: 0        PID: 1           CPU           IOs
  1         RUN:io         READY             1          
  2        WAITING       RUN:cpu             1             1
  3        WAITING       RUN:cpu             1             1
  4        WAITING       RUN:cpu             1             1
  5        WAITING       RUN:cpu             1             1
  6        WAITING          DONE                           1
  7*   RUN:io_done          DONE             1          

Stats: Total Time 7
Stats: CPU Busy 6 (85.71%)
Stats: IO Busy  5 (71.43%)
```

### 4.6

6. One other important behavior is what to do when an I/O completes. With `-I IO_RUN_LATER`, when an I/O completes, the process that issued it is not necessarily run right away; rather, whatever was running at the time keeps running. What happens when you run this combination of processes? (Run `./process-run.py -l 3:0,5:100,5:100,5:100 -S SWITCH_ON_IO -I IO_RUN_LATER -c -p`) Are system resources being effectively utilized?

Time is 31. When process 0 I/O done, the system will not start next I/O immediately. No, Because process 0 need wait to start next I/O.

```console
$ python3 process-run.py -l 3:0,5:100,5:100,5:100 -S SWITCH_ON_IO -I IO_RUN_LATER -c -p 
Time        PID: 0        PID: 1        PID: 2        PID: 3           CPU           IOs
  1         RUN:io         READY         READY         READY             1          
  2        WAITING       RUN:cpu         READY         READY             1             1
  3        WAITING       RUN:cpu         READY         READY             1             1
  4        WAITING       RUN:cpu         READY         READY             1             1
  5        WAITING       RUN:cpu         READY         READY             1             1
  6        WAITING       RUN:cpu         READY         READY             1             1
  7*         READY          DONE       RUN:cpu         READY             1          
  8          READY          DONE       RUN:cpu         READY             1          
  9          READY          DONE       RUN:cpu         READY             1          
 10          READY          DONE       RUN:cpu         READY             1          
 11          READY          DONE       RUN:cpu         READY             1          
 12          READY          DONE          DONE       RUN:cpu             1          
 13          READY          DONE          DONE       RUN:cpu             1          
 14          READY          DONE          DONE       RUN:cpu             1          
 15          READY          DONE          DONE       RUN:cpu             1          
 16          READY          DONE          DONE       RUN:cpu             1          
 17    RUN:io_done          DONE          DONE          DONE             1          
 18         RUN:io          DONE          DONE          DONE             1          
 19        WAITING          DONE          DONE          DONE                           1
 20        WAITING          DONE          DONE          DONE                           1
 21        WAITING          DONE          DONE          DONE                           1
 22        WAITING          DONE          DONE          DONE                           1
 23        WAITING          DONE          DONE          DONE                           1
 24*   RUN:io_done          DONE          DONE          DONE             1          
 25         RUN:io          DONE          DONE          DONE             1          
 26        WAITING          DONE          DONE          DONE                           1
 27        WAITING          DONE          DONE          DONE                           1
 28        WAITING          DONE          DONE          DONE                           1
 29        WAITING          DONE          DONE          DONE                           1
 30        WAITING          DONE          DONE          DONE                           1
 31*   RUN:io_done          DONE          DONE          DONE             1          

Stats: Total Time 31
Stats: CPU Busy 21 (67.74%)
Stats: IO Busy  15 (48.39%)
```

### 4.7

7. Now run the same processes, but with `-I IO_RUN_IMMEDIATE` set, which immediately runs the process that issued the I/O. How does this behavior differ? Why might running a process that just completed an I/O again be a good idea?

Time is 21. Time will be less compared with `IO_RUN_LATER`. This will improve time utilization. Because one process can start I/O immediately, while run another process can run instructions.

```console
$ python3 process-run.py -l 3:0,5:100,5:100,5:100 -S SWITCH_ON_IO -I IO_RUN_IMMEDIATE -c -p                130 ↵
Time        PID: 0        PID: 1        PID: 2        PID: 3           CPU           IOs
  1         RUN:io         READY         READY         READY             1          
  2        WAITING       RUN:cpu         READY         READY             1             1
  3        WAITING       RUN:cpu         READY         READY             1             1
  4        WAITING       RUN:cpu         READY         READY             1             1
  5        WAITING       RUN:cpu         READY         READY             1             1
  6        WAITING       RUN:cpu         READY         READY             1             1
  7*   RUN:io_done          DONE         READY         READY             1          
  8         RUN:io          DONE         READY         READY             1          
  9        WAITING          DONE       RUN:cpu         READY             1             1
 10        WAITING          DONE       RUN:cpu         READY             1             1
 11        WAITING          DONE       RUN:cpu         READY             1             1
 12        WAITING          DONE       RUN:cpu         READY             1             1
 13        WAITING          DONE       RUN:cpu         READY             1             1
 14*   RUN:io_done          DONE          DONE         READY             1          
 15         RUN:io          DONE          DONE         READY             1          
 16        WAITING          DONE          DONE       RUN:cpu             1             1
 17        WAITING          DONE          DONE       RUN:cpu             1             1
 18        WAITING          DONE          DONE       RUN:cpu             1             1
 19        WAITING          DONE          DONE       RUN:cpu             1             1
 20        WAITING          DONE          DONE       RUN:cpu             1             1
 21*   RUN:io_done          DONE          DONE          DONE             1          

Stats: Total Time 21
Stats: CPU Busy 21 (100.00%)
Stats: IO Busy  15 (71.43%)
```

### 4.8

8. Now run with some randomly generated processes: `-s 1 -l 3:50,3:50` or `-s 2 -l 3:50,3:50` or `-s 3 -l 3:50,3:50`. See if you can predict how the trace will turn out. What happens when you use the flag `-I IO_RUN_IMMEDIATE` vs. `-I IO_RUN_LATER`? What happens when you use `-S SWITCH_ON_IO` vs. `-S SWITCH_ON_END`?

`IO_RUN_IMMEDIATE`: take less time
`IO_RUN_LATER`: take more time

`SWITCH_ON_IO`: take less time
`SWITCH_ON_END`: take more time

Take `-s 3 -l 3:50,3:50` as an example and run with different flag. The time taken for the four cases is shown in the table below.

|  | `IO_RUN_LATER` | `IO_RUN_IMMEDIATE` |
| :-: | :-: | :-: |
| `SWITCH_ON_IO` | 18 | 17 |
| `SWITCH_ON_END` | 24 | 24 |

The default flag is `-I IO_RUN_LATER` and `-S SWITCH_ON_IO`.

```console
$ python3 process-run.py -s 3 -l 3:50,3:50 -c -p
Time        PID: 0        PID: 1           CPU           IOs
  1        RUN:cpu         READY             1          
  2         RUN:io         READY             1          
  3        WAITING        RUN:io             1             1
  4        WAITING       WAITING                           2
  5        WAITING       WAITING                           2
  6        WAITING       WAITING                           2
  7        WAITING       WAITING                           2
  8*   RUN:io_done       WAITING             1             1
  9*       RUN:cpu         READY             1          
 10           DONE   RUN:io_done             1          
 11           DONE        RUN:io             1          
 12           DONE       WAITING                           1
 13           DONE       WAITING                           1
 14           DONE       WAITING                           1
 15           DONE       WAITING                           1
 16           DONE       WAITING                           1
 17*          DONE   RUN:io_done             1          
 18           DONE       RUN:cpu             1          

Stats: Total Time 18
Stats: CPU Busy 9 (50.00%)
Stats: IO Busy  11 (61.11%)
```

Run with `-I IO_RUN_IMMEDIATE` and `-S SWITCH_ON_IO`.

```console
╰─$ python3 process-run.py -s 3 -l 3:50,3:50 -I IO_RUN_IMMEDIATE -S SWITCH_ON_IO -c -p
Time        PID: 0        PID: 1           CPU           IOs
  1        RUN:cpu         READY             1          
  2         RUN:io         READY             1          
  3        WAITING        RUN:io             1             1
  4        WAITING       WAITING                           2
  5        WAITING       WAITING                           2
  6        WAITING       WAITING                           2
  7        WAITING       WAITING                           2
  8*   RUN:io_done       WAITING             1             1
  9*         READY   RUN:io_done             1          
 10          READY        RUN:io             1          
 11        RUN:cpu       WAITING             1             1
 12           DONE       WAITING                           1
 13           DONE       WAITING                           1
 14           DONE       WAITING                           1
 15           DONE       WAITING                           1
 16*          DONE   RUN:io_done             1          
 17           DONE       RUN:cpu             1          

Stats: Total Time 17
Stats: CPU Busy 9 (52.94%)
Stats: IO Busy  11 (64.71%)
```

Run with `-I IO_RUN_IMMEDIATE` and `-S SWITCH_ON_END`.

```console
╰─$ python3 process-run.py -s 3 -l 3:50,3:50 -I IO_RUN_IMMEDIATE -S SWITCH_ON_END -c -p
Time        PID: 0        PID: 1           CPU           IOs
  1        RUN:cpu         READY             1          
  2         RUN:io         READY             1          
  3        WAITING         READY                           1
  4        WAITING         READY                           1
  5        WAITING         READY                           1
  6        WAITING         READY                           1
  7        WAITING         READY                           1
  8*   RUN:io_done         READY             1          
  9        RUN:cpu         READY             1          
 10           DONE        RUN:io             1          
 11           DONE       WAITING                           1
 12           DONE       WAITING                           1
 13           DONE       WAITING                           1
 14           DONE       WAITING                           1
 15           DONE       WAITING                           1
 16*          DONE   RUN:io_done             1          
 17           DONE        RUN:io             1          
 18           DONE       WAITING                           1
 19           DONE       WAITING                           1
 20           DONE       WAITING                           1
 21           DONE       WAITING                           1
 22           DONE       WAITING                           1
 23*          DONE   RUN:io_done             1          
 24           DONE       RUN:cpu             1          

Stats: Total Time 24
Stats: CPU Busy 9 (37.50%)
Stats: IO Busy  15 (62.50%)
```

Run with `-I IO_RUN_LATER` and `-S SWITCH_ON_IO`.

```console
╰─$ python3 process-run.py -s 3 -l 3:50,3:50 -I IO_RUN_LATER -S SWITCH_ON_IO -c -p     
Time        PID: 0        PID: 1           CPU           IOs
  1        RUN:cpu         READY             1          
  2         RUN:io         READY             1          
  3        WAITING        RUN:io             1             1
  4        WAITING       WAITING                           2
  5        WAITING       WAITING                           2
  6        WAITING       WAITING                           2
  7        WAITING       WAITING                           2
  8*   RUN:io_done       WAITING             1             1
  9*       RUN:cpu         READY             1          
 10           DONE   RUN:io_done             1          
 11           DONE        RUN:io             1          
 12           DONE       WAITING                           1
 13           DONE       WAITING                           1
 14           DONE       WAITING                           1
 15           DONE       WAITING                           1
 16           DONE       WAITING                           1
 17*          DONE   RUN:io_done             1          
 18           DONE       RUN:cpu             1          

Stats: Total Time 18
Stats: CPU Busy 9 (50.00%)
Stats: IO Busy  11 (61.11%)
```

Run with `-I IO_RUN_LATER` and `-S SWITCH_ON_END`.

```console
$ python3 process-run.py -s 3 -l 3:50,3:50 -I IO_RUN_LATER -S SWITCH_ON_END -c -p
Time        PID: 0        PID: 1           CPU           IOs
  1        RUN:cpu         READY             1          
  2         RUN:io         READY             1          
  3        WAITING         READY                           1
  4        WAITING         READY                           1
  5        WAITING         READY                           1
  6        WAITING         READY                           1
  7        WAITING         READY                           1
  8*   RUN:io_done         READY             1          
  9        RUN:cpu         READY             1          
 10           DONE        RUN:io             1          
 11           DONE       WAITING                           1
 12           DONE       WAITING                           1
 13           DONE       WAITING                           1
 14           DONE       WAITING                           1
 15           DONE       WAITING                           1
 16*          DONE   RUN:io_done             1          
 17           DONE        RUN:io             1          
 18           DONE       WAITING                           1
 19           DONE       WAITING                           1
 20           DONE       WAITING                           1
 21           DONE       WAITING                           1
 22           DONE       WAITING                           1
 23*          DONE   RUN:io_done             1          
 24           DONE       RUN:cpu             1          

Stats: Total Time 24
Stats: CPU Busy 9 (37.50%)
Stats: IO Busy  15 (62.50%)
```
