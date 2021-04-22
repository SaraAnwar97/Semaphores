# Semaphores
Semaphore is simply a variable that is non-negative and shared between threads.
A semaphore is a signaling mechanism, and a thread that is waiting on a semaphore can be signaled by another thread.
It uses two atomic operations, 1)wait, and 2) signal for the process synchronization.

## About
A C program to solve the following synchronization problem using
POSIX and “semaphore.h” libraries.
N mcounter threads count independent incoming messages in a system, and another thread
mmonitor gets the count of threads at time intervals of size t1, and then resets the counter to
0. The mmonitor then places this value in a buffer of size b, and a mmcollector thread reads
the values from the buffer.
Any thread will have to wait if the counter is being locked by any other thread. Also, the
mmonitor and mcollecttor threads will not be able to access the buffer at the same time or to
add another entry if the buffer is full.
Assume that the messages come randomly to the system, this can be realized if the mcounter
threads sleep for random times, and their activation (sleep time ends) corresponds to an email
arrival. Similarly, the mmonitor and mcollector will be activated at random time intervals.

<img width="729" alt="Screen Shot 2021-04-22 at 7 11 11 PM" src="https://user-images.githubusercontent.com/38399357/115756492-82d3a200-a39e-11eb-8201-50598e7addb5.png">


## Prerequisites

install Linux OS.

## Compiling

gcc -o semaphore semaphore.c -pthread

## Running

./semaphore

