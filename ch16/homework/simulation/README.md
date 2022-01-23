# Chapter 16.

## Homework (Simulation)

This program([`segmentation.py`](./code/segmentation.py)) allows you to see how address translations are performed in a system with segmentation. See the [README](./code/README.md) for details.

## Questions and Solutions

### 16.1

> 1. First let’s use a tiny address space to translate some addresses. Here’s a simple set of parameters with a few different random seeds; can you translate the addresses?
>
> ```console
> segmentation.py -a 128 -p 512 -b 0 -l 20 -B 512 -L 20 -s 0
> segmentation.py -a 128 -p 512 -b 0 -l 20 -B 512 -L 20 -s 1
> segmentation.py -a 128 -p 512 -b 0 -l 20 -B 512 -L 20 -s 2
> ```

`segmentation.py -a 128 -p 512 -b 0 -l 20 -B 512 -L 20 -s 0` solution:

```console
$ python3 segmentation.py -a 128 -p 512 -b 0 -l 20 -B 512 -L 20 -s 0
ARG seed 0
ARG address space size 128
ARG phys mem size 512

Segment register information:

  Segment 0 base  (grows positive) : 0x00000000 (decimal 0)
  Segment 0 limit                  : 20

  Segment 1 base  (grows negative) : 0x00000200 (decimal 512)
  Segment 1 limit                  : 20

Virtual Address Trace
  VA  0: 0x0000006c (decimal:  108) --> SEG1: (108 - 128) + 512 = 492 
  VA  1: 0x00000061 (decimal:   97) --> SEG1: segmentation violation (abs(97-128) > 20)
  VA  2: 0x00000035 (decimal:   53) --> SEG0: segmentation violation (53 > 20)
  VA  3: 0x00000021 (decimal:   33) --> SEG0: segmentation violation (33 > 20)
  VA  4: 0x00000041 (decimal:   65) --> SEG1: segmentation violation (abs(65-128) > 20)
```

`segmentation.py -a 128 -p 512 -b 0 -l 20 -B 512 -L 20 -s 1` solution:

```console
$ python3 segmentation.py -a 128 -p 512 -b 0 -l 20 -B 512 -L 20 -s 1   
ARG seed 1
ARG address space size 128
ARG phys mem size 512

Segment register information:

  Segment 0 base  (grows positive) : 0x00000000 (decimal 0)
  Segment 0 limit                  : 20

  Segment 1 base  (grows negative) : 0x00000200 (decimal 512)
  Segment 1 limit                  : 20

Virtual Address Trace
  VA  0: 0x00000011 (decimal:   17) --> SEG0: 17 + 0 = 17
  VA  1: 0x0000006c (decimal:  108) --> SEG1: (108 - 128) + 512 = 492 
  VA  2: 0x00000061 (decimal:   97) --> SEG1: segmentation violation (abs(97-128) > 20)
  VA  3: 0x00000020 (decimal:   32) --> SEG0: segmentation violation (32 > 20)
  VA  4: 0x0000003f (decimal:   63) --> SEG0: segmentation violation (63 > 20)
```

`segmentation.py -a 128 -p 512 -b 0 -l 20 -B 512 -L 20 -s 2` solution:

```console
$ python3 segmentation.py -a 128 -p 512 -b 0 -l 20 -B 512 -L 20 -s 2   
ARG seed 2
ARG address space size 128
ARG phys mem size 512

Segment register information:

  Segment 0 base  (grows positive) : 0x00000000 (decimal 0)
  Segment 0 limit                  : 20

  Segment 1 base  (grows negative) : 0x00000200 (decimal 512)
  Segment 1 limit                  : 20

Virtual Address Trace
  VA  0: 0x0000007a (decimal:  122) --> SEG1: (122 - 128) + 512 = 506
  VA  1: 0x00000079 (decimal:  121) --> SEG1: (121 - 128) + 512 = 505
  VA  2: 0x00000007 (decimal:    7) --> SEG0: 7 + 0 = 7
  VA  3: 0x0000000a (decimal:   10) --> SEG0: 10 + 0 = 10
  VA  4: 0x0000006a (decimal:  106) --> SEG1: segmentation violation (abs(106-128) > 20)
```

### 16.2

> 2. Now, let’s see if we understand this tiny address space we’ve constructed (using the parameters from the question above). What is the highest legal virtual address in segment 0? What about the lowest legal virtual address in segment 1? What are the lowest and highest *illegal* addresses in this entire address space? Finally, how would you run `segmentation.py` with the `-A` flag to test if you are right?

In the condition of question 1, the answer in the question is as follows 

- The highest legal virtual address in segment 0 is 20
- The lowest legal virtual address in segment 1 is 108(=128-20)
- The lowest *illegal* virtual address in segment 0 is 21(=20+1)
- The highest *illegal* virtual address in segment 1 is 107(=128-20-1)

### 16.3

> 3. Let’s say we have a tiny 16-byte address space in a 128-byte physical memory. What base and bounds would you set up so as to get the simulator to generate the following translation results for the specified address stream: valid, valid, violation, ..., violation, valid, valid? Assume the following parameters:
> 
> ```console
> segmentation.py -a 16 -p 128 -A 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 --b0 ? --l0 ? --b1 ? --l1 ?
> ```

Here is one of correct answers:

```console
$ python3 segmentation.py -a 16 -p 128 -A 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 -b 0 -l 2 -B 128 -L 2 -c
```

In fact, correct answer just needs to be met the following conditions:

1. SEG0 limit = SEG1 limit = 2
2. The address between SGE1 and SEG2 is greater than or equal to 4.

### 16.4

> 4. Assume we want to generate a problem where roughly 90% of the randomly-generated virtual addresses are valid (not segmentation violations). How should you configure the simulator to do so? Which parameters are important to getting this outcome?

When the parameters meet **(SEG0_length + SEG1_length) = 0.9 * address_space_size**,  roughly 90% of the randomly-generated virtual addresses are valid in the simulator

### 16.5

> 5. Can you run the simulator such that no virtual addresses are valid? How?

You just need to set each segment length equals 0, that is SEG0_length = SEG1_length = 0.
