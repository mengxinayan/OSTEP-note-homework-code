# ch5 Process API Homework and Solution

If you just want to read the answers, that is a simple version for solutions, please visit [here](../solution.md).

## Homework (Simulation)

This simulation homework focuses on [fork.py](../code/ch5/homework-simulation/fork.py), a simple process creation simulator that shows how processes are related in a single “familial” tree. Read the relevant [README](../code/ch5/homework-simulation/README.md) for details about how to run the simulator.

## Questions and Solutions

### 5.1

1. Run `./fork.py -s 10` and see which actions are taken. Can you predict what the process tree looks like at each step? Use the `-c` flag to check your answers. Try some different random seeds (`-s`) or add more actions (`-a`) to get the hang of it.

```console
$ python3 fork.py -s 10 -c

ARG seed 10
ARG fork_percentage 0.7
ARG actions 5
ARG action_list 
ARG show_tree False
ARG just_final False
ARG leaf_only False
ARG local_reparent False
ARG print_style fancy
ARG solve True

                           Process Tree:
                               a

Action: a forks b
                               a
                               └── b
Action: a forks c
                               a
                               ├── b
                               └── c
Action: c EXITS
                               a
                               └── b
Action: a forks d
                               a
                               ├── b
                               └── d
Action: a forks e
                               a
                               ├── b
                               ├── d
                               └── e
```

### 5.2

2. One control the simulator gives you is the `fork_percentage`, controlled by the `-f` flag. The higher it is, the more likely the next action is a fork; the lower it is, the more likely the action is an exit. Run the simulator with a large number of actions (e.g., `-a 100`) and vary the `fork_percentage` from 0.1 to 0.9. What do you think the resulting final process trees will look like as the percentage changes? Check your answer with `-c`.

The `fork_percentage` is more larger, there will more processes, the process tree will be more complex.

When `fork_percentage` equals 0.2:

```console
$ python3 fork.py -a 20 -f 0.2 -c

ARG seed -1
ARG fork_percentage 0.2
ARG actions 20
ARG action_list 
ARG show_tree False
ARG just_final False
ARG leaf_only False
ARG local_reparent False
ARG print_style fancy
ARG solve True

                           Process Tree:
                               a

Action: a forks b
                               a
                               └── b
Action: b EXITS
                               a
Action: a forks c
                               a
                               └── c
Action: c EXITS
                               a
Action: a forks d
                               a
                               └── d
Action: d EXITS
                               a
Action: a forks e
                               a
                               └── e
Action: e EXITS
                               a
Action: a forks f
                               a
                               └── f
Action: a forks g
                               a
                               ├── f
                               └── g
Action: f EXITS
                               a
                               └── g
Action: g EXITS
                               a
Action: a forks h
                               a
                               └── h
Action: h EXITS
                               a
Action: a forks i
                               a
                               └── i
Action: i EXITS
                               a
Action: a forks j
                               a
                               └── j
Action: j EXITS
                               a
Action: a forks k
                               a
                               └── k
Action: k EXITS
                               a
```

When `fork_percentage` equals 0.8:

```console
$ python3 fork.py -a 20 -f 0.8 -c

ARG seed -1
ARG fork_percentage 0.8
ARG actions 20
ARG action_list 
ARG show_tree False
ARG just_final False
ARG leaf_only False
ARG local_reparent False
ARG print_style fancy
ARG solve True

                           Process Tree:
                               a

Action: a forks b
                               a
                               └── b
Action: b forks c
                               a
                               └── b
                                   └── c
Action: a forks d
                               a
                               ├── b
                               │   └── c
                               └── d
Action: a forks e
                               a
                               ├── b
                               │   └── c
                               ├── d
                               └── e
Action: b forks f
                               a
                               ├── b
                               │   ├── c
                               │   └── f
                               ├── d
                               └── e
Action: e forks g
                               a
                               ├── b
                               │   ├── c
                               │   └── f
                               ├── d
                               └── e
                                   └── g
Action: c forks h
                               a
                               ├── b
                               │   ├── c
                               │   │   └── h
                               │   └── f
                               ├── d
                               └── e
                                   └── g
Action: d EXITS
                               a
                               ├── b
                               │   ├── c
                               │   │   └── h
                               │   └── f
                               └── e
                                   └── g
Action: f forks i
                               a
                               ├── b
                               │   ├── c
                               │   │   └── h
                               │   └── f
                               │       └── i
                               └── e
                                   └── g
Action: c EXITS
                               a
                               ├── b
                               │   └── f
                               │       └── i
                               ├── e
                               │   └── g
                               └── h
Action: a forks j
                               a
                               ├── b
                               │   └── f
                               │       └── i
                               ├── e
                               │   └── g
                               ├── h
                               └── j
Action: f EXITS
                               a
                               ├── b
                               ├── e
                               │   └── g
                               ├── h
                               ├── j
                               └── i
Action: j forks k
                               a
                               ├── b
                               ├── e
                               │   └── g
                               ├── h
                               ├── j
                               │   └── k
                               └── i
Action: j forks l
                               a
                               ├── b
                               ├── e
                               │   └── g
                               ├── h
                               ├── j
                               │   ├── k
                               │   └── l
                               └── i
Action: e EXITS
                               a
                               ├── b
                               ├── h
                               ├── j
                               │   ├── k
                               │   └── l
                               ├── i
                               └── g
Action: k forks m
                               a
                               ├── b
                               ├── h
                               ├── j
                               │   ├── k
                               │   │   └── m
                               │   └── l
                               ├── i
                               └── g
Action: h EXITS
                               a
                               ├── b
                               ├── j
                               │   ├── k
                               │   │   └── m
                               │   └── l
                               ├── i
                               └── g
Action: m forks n
                               a
                               ├── b
                               ├── j
                               │   ├── k
                               │   │   └── m
                               │   │       └── n
                               │   └── l
                               ├── i
                               └── g
Action: g forks o
                               a
                               ├── b
                               ├── j
                               │   ├── k
                               │   │   └── m
                               │   │       └── n
                               │   └── l
                               ├── i
                               └── g
                                   └── o
Action: l forks p
                               a
                               ├── b
                               ├── j
                               │   ├── k
                               │   │   └── m
                               │   │       └── n
                               │   └── l
                               │       └── p
                               ├── i
                               └── g
                                   └── o
```

### 5.3

3. Now, switch the output by using the `-t` flag (e.g., run `./fork.py -t`). Given a set of process trees, can you tell which actions were taken?

```console
$ python3 fork.py -t -c          

ARG seed -1
ARG fork_percentage 0.7
ARG actions 5
ARG action_list 
ARG show_tree True
ARG just_final False
ARG leaf_only False
ARG local_reparent False
ARG print_style fancy
ARG solve True

                           Process Tree:
                               a

Action: a forks b
                               a
                               └── b
Action: b forks c
                               a
                               └── b
                                   └── c
Action: b forks d
                               a
                               └── b
                                   ├── c
                                   └── d
Action: a forks e
                               a
                               ├── b
                               │   ├── c
                               │   └── d
                               └── e
Action: b forks f
                               a
                               ├── b
                               │   ├── c
                               │   ├── d
                               │   └── f
                               └── e
```

### 5.4

4. One interesting thing to note is what happens when a child exits; what happens to its children in the process tree? To study this, let’s create a specific example: `./fork.py -A a+b,b+c,c+d,c+e,c-`. This example has process ’a’ create ’b’, which in turn creates ’c’, which then creates ’d’ and ’e’. However, then, ’c’ exits. What do you think the process tree should like after the exit? What if you use the `-R` flag? Learn more about what happens to orphaned processes on your own to add more context.

Without `-R` flag, when a process's parent exits, its parent will change to initial process.

```console
$ python3 fork.py -A a+b,b+c,c+d,c+e,c- -c

ARG seed -1
ARG fork_percentage 0.7
ARG actions 5
ARG action_list a+b,b+c,c+d,c+e,c-
ARG show_tree False
ARG just_final False
ARG leaf_only False
ARG local_reparent False
ARG print_style fancy
ARG solve True

                           Process Tree:
                               a

Action: a forks b
                               a
                               └── b
Action: b forks c
                               a
                               └── b
                                   └── c
Action: c forks d
                               a
                               └── b
                                   └── c
                                       └── d
Action: c forks e
                               a
                               └── b
                                   └── c
                                       ├── d
                                       └── e
Action: c EXITS
                               a
                               ├── b
                               ├── d
                               └── e
```

With `-R` flag, when a process's parent exits, its parent will change to its grandfather process.

```console
╰─$ python3 fork.py -A a+b,b+c,c+d,c+e,c- -R -c

ARG seed -1
ARG fork_percentage 0.7
ARG actions 5
ARG action_list a+b,b+c,c+d,c+e,c-
ARG show_tree False
ARG just_final False
ARG leaf_only False
ARG local_reparent True
ARG print_style fancy
ARG solve True

                           Process Tree:
                               a

Action: a forks b
                               a
                               └── b
Action: b forks c
                               a
                               └── b
                                   └── c
Action: c forks d
                               a
                               └── b
                                   └── c
                                       └── d
Action: c forks e
                               a
                               └── b
                                   └── c
                                       ├── d
                                       └── e
Action: c EXITS
                               a
                               └── b
                                   ├── d
                                   └── e
```

### 5.5

5. One last flag to explore is the `-F` flag, which skips intermediate steps and only asks to fill in the final process tree. Run `./fork.py -F` and see if you can write down the final tree by looking at the series of actions generated. Use different random seeds to try this a few times.

```console
$ python3 fork.py -F -c

ARG seed -1
ARG fork_percentage 0.7
ARG actions 5
ARG action_list 
ARG show_tree False
ARG just_final True
ARG leaf_only False
ARG local_reparent False
ARG print_style fancy
ARG solve True

                           Process Tree:
                               a

Action: a forks b
Action: b EXITS
Action: a forks c
Action: a forks d
Action: c forks e

                        Final Process Tree:
                               a
                               ├── c
                               │   └── e
                               └── d
```

### 5.6

6. Finally, use both `-t` and `-F` together. This shows the final process tree, but then asks you to fill in the actions that took place. By looking at the tree, can you determine the exact actions that took place? In which cases can you tell? In which can’t you tell? Try some different random seeds to delve into this question.

```console
$ python3 fork.py -t -F -c

ARG seed -1
ARG fork_percentage 0.7
ARG actions 5
ARG action_list a+b,b+c,a+d,b+e,c-
ARG show_tree True
ARG just_final True
ARG leaf_only False
ARG local_reparent False
ARG print_style fancy
ARG solve True

                           Process Tree:
                               a

Action: a forks b
Action: ? forks c (Can't determine who forks c)
Action: a forks d
Action: b forks e
Action: C EXITS ?(Can't determine when C exit)

                        Final Process Tree:
                               a
                               ├── b
                               │   └── e
                               └── d
```
