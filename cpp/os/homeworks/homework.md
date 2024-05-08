## Chapter 2 Homework
Questions
1. Run process-run.py with the following flags: -l 5:100,5:100.
What should the CPU utilization be (e.g., the percent of time the
CPU is in use?) Why do you know this? Use the -c and -p flags to
see if you were right.

`when running this we start 2 processes that run 100 instructions in 5 units of time. There are no i/o blockers or cpu waiting time so the cpu utilization should be 100%`

2. Now run with these flags: ./process-run.py -l 4:100,1:0.
These flags specify one process with 4 instructions (all to use the
CPU), and one that simply issues an I/O and waits for it to be done.
How long does it take to complete both processes? Use -c and -p
to find out if you were right.

`should take 11 units of time. 4 for the first process, one to issue the i/o 5 to preform the i/o then 1 to finish the i/o`

3. Switch the order of the processes: -l 1:0,4:100. What happens
now? Does switching the order matter? Why? (As always, use -c
and -p to see if you were right)
```
Yes, this will only take 7 units of time.
            1       2
time 1 start i/o READY
time 2 run       run
time 3 run       run
time 4 run       run
time 5 run       run
time 6 run       DONE
time 6 DONE      DONE
```
4. We’ll now explore some of the other flags. One important flag is
-S, which determines how the system reacts when a process issues an I/O. With the flag set to SWITCH ON END, the system
will NOT switch to another process while one is doing I/O, instead waiting until the process is completely finished. What happens when you run the following two processes (-l 1:0,4:100
-c -S SWITCH ON END), one doing I/O and the other doing CPU
work?

`This forces the scheduler to wait for the i/o to be done before continuing so it takes 11 units of time.`

5. Now, run the same processes, but with the switching behavior set
to switch to another process whenever one is WAITING for I/O (-l
1:0,4:100 -c -S SWITCH ON IO). What happens now? Use -c
and -p to confirm that you are right.

`This will schedule the i/o effectively`

6. One other important behavior is what to do when an I/O completes. With -I IO RUN LATER, when an I/O completes, the process that issued it is not necessarily run right away; rather, whatever was running at the time keeps running. What happens when
you run this combination of processes? (./process-run.py -l
3:0,5:100,5:100,5:100 -S SWITCH ON IO -c -p -I
IO RUN LATER) Are system resources being effectively utilized?

I think no, since there is a lot of idol time for the process that issued the i/o could be finished but is instead waiting for other processes to finish before issuing a done.

7. Now run the same processes, but with -I IO RUN IMMEDIATE set,
which immediately runs the process that issued the I/O. How does
this behavior differ? Why might running a process that just completed an I/O again be a good idea?

Here the done is ran right when we can so we can and also issues other i/o right after so we can utilize system resources better.


## Chapter 5 Homework
Questions
1. Write a program that calls fork(). Before calling fork(), have the
main process access a variable (e.g., x) and set its value to something (e.g., 100). What value is the variable in the child process?
What happens to the variable when both the child and parent change
the value of x?

```c++
#include <cstdlib>
#include <iostream>
#include <unistd.h>
using namespace std;
int main() {
    pid_t c_pid = fork();
    int x = 100;
    if (c_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (c_pid > 0) {
        cout << "child process" << '\n';
        x = 1000;
        cout << x << '\n';
    } else {
        cout << "parent process" << '\n';
        x = 10000;
        cout << x << '\n';
    }
}
```

The value of x that is printed is dependent on where we are in the process. in the child process it prints 1000, in the parent process it prints 10000

2. Write a program that opens a file (with the open() system call)
and then calls fork() to create a new process. Can both the child
and parent access the file descriptor returned by open()? What
happens when they are writing to the file concurrently, i.e., at the
same time?
```C
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

int main() {
    int fd;
    pid_t pid;

    // Open the file
    fd = open("output.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    // Fork a new process
    pid = fork();
    if (pid == -1) {
        perror("fork");
        close(fd);
        return 1;
    }

    if (pid == 0) { // Child process
        // Write to the file
        dprintf(fd, "This is the child process.\n");
        close(fd);
    } else { // Parent process
        // Wait for the child process to finish
        waitpid(pid, NULL, 0);

        // Write to the file
        dprintf(fd, "This is the parent process.\n");
        close(fd);
    }

    return 0;
}
```
The child and parent process can both access the file descriptor however the problem arrises with the order of rights. The order of writting is not guranteed.

3. Write another program using fork(). The child process should
print “hello”; the parent process should print “goodbye”. You should
try to ensure that the child process always prints first; can you do
this without calling wait() in the parent?

No you cannot gurantee that without `wait()`
```C
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid; 
    pid = fork();

    if (pid == 0) {
        printf("Hello\n");
    } else if (pid > 0) {
        int status;
        waitpid(pid, &status, 0);
        printf("goodbye\n");
    } else {
        perror("fork");
        return 1;
    }
    return 0;
}
```
4. Write a program that calls fork() and then calls some form of
exec() to run the program /bin/ls. See if you can try all of the
variants of exec(), including (on Linux) execl(), execle(),
execlp(), execv(), execvp(), and execvpe(). Why do
you think there are so many variants of the same basic call?

```C
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid; 
    pid = fork();

    if (pid == 0) {
        execl("/bin/ls", "ls", NULL);
    } else if (pid > 0) {
        int status;
        waitpid(pid, &status, 0);
        printf("goodbye\n");
    } else {
        perror("fork");
        return 1;
    }
    return 0;
}
```

5. Now write a program that uses wait() to wait for the child process
to finish in the parent. What does wait() return? What happens if
you use wait() in the child?
```C
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid; 
    pid = fork();

    if (pid == 0) {
        execl("/bin/ls", "ls", NULL);
    } else if (pid > 0) {
        int status;
        waitpid(pid, &status, 0);
        printf("goodbye\n");
    } else {
        perror("fork");
        return 1;
    }
    return 0;
}
```
`wait()` will return the PID of the terminated process or -1 if there was an error.

1. Write a slight modification of the previous program, this time using waitpid() instead of wait(). When would waitpid() be
useful?

`waitpid() provides more control over what process is being waited for`

1. Write a program that creates a child process, and then in the child
closes standard output (STDOUT FILENO). What happens if the child
calls printf() to print some output after closing the descriptor?

`the child process printf() will write to the open file descriptor`
```C
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid;

    pid = fork();

    if (pid == 0) { // Child process
        // Close the standard output (STDOUT_FILENO)
        close(STDOUT_FILENO);

        // Print some output
        printf("This is the child process.\n");
    } else if (pid > 0) { // Parent process
        int status;
        waitpid(pid, &status, 0);
    } else { // Fork failed
        perror("fork");
        return 1;
    }

    return 0;
}
```

If we opened a file descriptor in the main process it the child writes to that.

1. Write a program that creates two children, and connects the standard output of one to the standard input of the other, using the
pipe() system call

![alt text](image-7.png)
```C
#include<stdio.h>
#include<unistd.h>

int main() {
   int pipefds1[2], pipefds2[2];
   int returnstatus1, returnstatus2;
   int pid;
   char pipe1writemessage[20] = "Hi";
   char pipe2writemessage[20] = "Hello";
   char readmessage[20];
   returnstatus1 = pipe(pipefds1);
   
   if (returnstatus1 == -1) {
      printf("Unable to create pipe 1 \n");
      return 1;
   }
   returnstatus2 = pipe(pipefds2);
   
   if (returnstatus2 == -1) {
      printf("Unable to create pipe 2 \n");
      return 1;
   }
   pid = fork();
   
   if (pid != 0) // Parent process {
      close(pipefds1[0]); // Close the unwanted pipe1 read side
      close(pipefds2[1]); // Close the unwanted pipe2 write side
      printf("In Parent: Writing to pipe 1 – Message is %s\n", pipe1writemessage);
      write(pipefds1[1], pipe1writemessage, sizeof(pipe1writemessage));
      read(pipefds2[0], readmessage, sizeof(readmessage));
      printf("In Parent: Reading from pipe 2 – Message is %s\n", readmessage);
   } else { //child process
      close(pipefds1[1]); // Close the unwanted pipe1 write side
      close(pipefds2[0]); // Close the unwanted pipe2 read side
      read(pipefds1[0], readmessage, sizeof(readmessage));
      printf("In Child: Reading from pipe 1 – Message is %s\n", readmessage);
      printf("In Child: Writing to pipe 2 – Message is %s\n", pipe2writemessage);
      write(pipefds2[1], pipe2writemessage, sizeof(pipe2writemessage));
   }
   return 0;
}
```
Basically we can establish 2 way communication by creating 2 pipes, 1 pipe is used for writing to the parent process and reading from the child process while the other is used to read from the parent process and write from the child process. To do this we have to close the sides of the pipe that we do not need, and write/read to the sides that we do need. As you can see we preform closes on the unneeded sides, writes on the side accepting writes and reads on the side accepting reads.
## Chapter 6 Homework
In this homework, you’ll measure the costs of a system call and context
switch. Measuring the cost of a system call is relatively easy. For example,
you could repeatedly call a simple system call (e.g., performing a 0-byte
read), and time how long it takes; dividing the time by the number of
iterations gives you an estimate of the cost of a system call.
One thing you’ll have to take into account is the precision and accuracy of your timer. A typical timer that you can use is gettimeofday();
read the man page for details. What you’ll see there is that gettimeofday()
returns the time in microseconds since 1970; however, this does not mean
that the timer is precise to the microsecond. Measure back-to-back calls
to gettimeofday() to learn something about how precise the timer really is; this will tell you how many iterations of your null system-call
test you’ll have to run in order to get a good measurement result. If
gettimeofday() is not precise enough for you, you might look into
using the rdtsc instruction available on x86 machines.
Measuring the cost of a context switch is a little trickier. The lmbench
benchmark does so by running two processes on a single CPU, and setting up two UNIX pipes between them; a pipe is just one of many ways
processes in a UNIX system can communicate with one another. The first
process then issues a write to the first pipe, and waits for a read on the
second; upon seeing the first process waiting for something to read from
the second pipe, the OS puts the first process in the blocked state, and
switches to the other process, which reads from the first pipe and then
writes to the second. When the second process tries to read from the first
pipe again, it blocks, and thus the back-and-forth cycle of communication
continues. By measuring the cost of communicating like this repeatedly,
lmbench can make a good estimate of the cost of a context switch. You
can try to re-create something similar here, using pipes, or perhaps some
other communication mechanism such as UNIX sockets.
One difficulty in measuring context-switch cost arises in systems with
more than one CPU; what you need to do on such a system is ensure that
your context-switching processes are located on the same processor. Fortunately, most operating systems have calls to bind a process to a particular processor; on Linux, for example, the sched setaffinity() call
is what you’re looking for. By ensuring both processes are on the same
processor, you are making sure to measure the cost of the OS stopping
one process and restoring another on the same CPU

### takeaways
Measuring a process, instruction, system call, etc can be done using the rdtsc instruction available on x86 machines

For context switches, an accurate way is to set up 2 processes with communication via pipes and measure the cost of communication 

a key note is that when measuring a context switch with this method it is important both processes are executing on the same cpu. This can be done by binding

```C
#include <stdio.h>
#include <unistd.h>
#include <time.h>

int main() {
    int pipefds1[2], pipefds2[2];
    int returnstatus1, returnstatus2;
    int pid;
    char pipe1writemessage[20] = "Hi";
    char pipe2writemessage[20] = "Hello";
    char readmessage[20];
    struct timespec start_time, end_time;

    returnstatus1 = pipe(pipefds1);

    if (returnstatus1 == -1) {
        printf("Unable to create pipe 1 \n");
        return 1;
    }

    returnstatus2 = pipe(pipefds2);

    if (returnstatus2 == -1) {
        printf("Unable to create pipe 2 \n");
        return 1;
    }

    pid = fork();

    if (pid != 0) { // Parent process
        close(pipefds1[0]); // Close the unwanted pipe1 read side
        close(pipefds2[1]); // Close the unwanted pipe2 write side

        // Get the start time before writing to pipe 1
        clock_gettime(CLOCK_MONOTONIC, &start_time);

        printf("In Parent: Writing to pipe 1 – Message is %s\n", pipe1writemessage);
        write(pipefds1[1], pipe1writemessage, sizeof(pipe1writemessage));

        read(pipefds2[0], readmessage, sizeof(readmessage));

        // Get the end time after reading from pipe 2
        clock_gettime(CLOCK_MONOTONIC, &end_time);

        printf("In Parent: Reading from pipe 2 – Message is %s\n", readmessage);

        // Calculate the elapsed time in nanoseconds
        long long elapsed_ns = (end_time.tv_sec - start_time.tv_sec) * 1000000000LL +
                               (end_time.tv_nsec - start_time.tv_nsec);

        printf("Elapsed time between writing to pipe 1 and reading from pipe 2: %lld nanoseconds\n", elapsed_ns);
    } else { // Child process
        close(pipefds1[1]); // Close the unwanted pipe1 write side
        close(pipefds2[0]); // Close the unwanted pipe2 read side

        read(pipefds1[0], readmessage, sizeof(readmessage));
        printf("In Child: Reading from pipe 1 – Message is %s\n", readmessage);

        printf("In Child: Writing to pipe 2 – Message is %s\n", pipe2writemessage);
        write(pipefds2[1], pipe2writemessage, sizeof(pipe2writemessage));
    }

    return 0;
}
```
Meassuring the time it takes for two pipes to read and write
## Chapter 7 Homework

Questions
1. Compute the response time and turnaround time when running
three jobs of length 200 with the SJF and FIFO schedulers.
```
SJF
ARG policy SJF
ARG jobs 3
ARG maxlen 10
ARG seed 200

Here is the job list, with the run time of each job:
  Job 0 ( length = 1 )
  Job 1 ( length = 3 )
  Job 2 ( length = 8 )


** Solutions **

Execution trace:
  [ time   0 ] Run job 0 for 1.00 secs ( DONE at 1.00 )
  [ time   1 ] Run job 1 for 3.00 secs ( DONE at 4.00 )
  [ time   4 ] Run job 2 for 8.00 secs ( DONE at 12.00 )

Final statistics:
  Job   0 -- Response: 0.00  Turnaround 1.00  Wait 0.00
  Job   1 -- Response: 1.00  Turnaround 4.00  Wait 1.00
  Job   2 -- Response: 4.00  Turnaround 12.00  Wait 4.00

  Average -- Response: 1.67  Turnaround 5.67  Wait 1.67
```

```FIFO
ARG policy FIFO
ARG jobs 3
ARG maxlen 10
ARG seed 200

Here is the job list, with the run time of each job:
  Job 0 ( length = 1 )
  Job 1 ( length = 3 )
  Job 2 ( length = 8 )


** Solutions **

Execution trace:
  [ time   0 ] Run job 0 for 1.00 secs ( DONE at 1.00 )
  [ time   1 ] Run job 1 for 3.00 secs ( DONE at 4.00 )
  [ time   4 ] Run job 2 for 8.00 secs ( DONE at 12.00 )

Final statistics:
  Job   0 -- Response: 0.00  Turnaround 1.00  Wait 0.00
  Job   1 -- Response: 1.00  Turnaround 4.00  Wait 1.00
  Job   2 -- Response: 4.00  Turnaround 12.00  Wait 4.00

  Average -- Response: 1.67  Turnaround 5.67  Wait 1.67
```
both are identical since they both have the same lengths for their jobs

2. Now do the same but with jobs of different lengths: 100, 200, and 300.
`Same output since they arrive at the same order`
3. Now do the same, but also with the RR scheduler and a time-slice
of 1.
`Same output`
1. For what types of workloads does SJF deliver the same turnaround
times as FIFO?
`if the jobs are the same length`
1. For what types of workloads and quantum lengths does SJF deliver
the same response times as RR?
`If both SJF and RR recieve similar length jobs at the same time they both essentially act as FIFO schedulers`
1. What happens to response time with SJF as job lengths increase?
Can you use the simulator to demonstrate the trend?
`assuming we have 5 jobs that can all take 1000 seconds, while we are doing 1 job we could potentially get a job that takes 1 second while executing so we are forced to wait.` 
1. What happens to response time with RR as quantum lengths increase? Can you write an equation that gives the worst-case response time, given N jobs?
`Worst-case Response Time = (N - 1) * q`
