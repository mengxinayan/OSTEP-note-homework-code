## ch4

In this charter homework, we should run `process-run.py` simulator([here](./code/ch4/process-run.py)). Before run it, you should read its README([here](./code/ch4/README.md)).

### 4.1

Process 0: 50%; Process 1: 50%; Total : 100%

### 4.2

Time is 11 = 4(RUN:cpu) + 1(RUN:io) + 5(WAITING) + 1(RUN:io_done)

When process 0 finishes, then process 1 will start. And IO takes 5 tricks(default).

### 4.3

Time is 7 = 1(RUN:io) + 5(WAITING && RUN:cpu) + 1(RUN:io_done)

Because process 1 will use CPU when process 0 waiting I/O.

### 4.4

Time will be 11 = 1(RUN:io) + 5(WAITING) + 1(RUN:io_done) + 4(RUN:cpu)

Because the system will NOT switch to another process while one is doing I/O, it will start process 1 when process 0 ends.

### 4.5

Time is 7 = 1(RUN:io) + 5(WAITING && RUN:cpu) + 1(RUN:io_done)

Because the system will switch another process while one is doing I/O, so process 1 will run during process doing I/O.

### 4.6

Time is 31.

When process 0 I/O done, the system will not start next I/O immediately.

No, Because process 0 need wait to start next I/O.

### 4.7

Time is 21. Time will be less compared with `IO_RUN_LATER`.

This will improve time utilization. Because one process can start I/O immediately, while run another process can run instructions.

### 4.8

`IO_RUN_IMMEDIATE`: take less time
`IO_RUN_LATER`: take more time

`SWITCH_ON_IO`: take less time
`SWITCH_ON_END`: take more time
