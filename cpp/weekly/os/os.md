# Introduction
OS **virtualization** is making physical resources of a computer like processor, memory, etc into a virtual easy to use form of itself.

OS provides a "standard library" of methods applications can call. It is also considered a "resource manager" do to its role as managing and allocating resources.

An example of CPU **virtualization** is running multiple programs at once and having them run concurrently. Even though there is only 1 CPU due to virtualization it is able to handle everything.

Policies are used in lots of places in the OS, they essentially serve as a way for the OS to know what to do and when to handle certain scenarios.
ex, two programs are ran at once, which does the OS run first? 

```C
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "common.h"
int main(int argc, char *argv[])
  {
    int *p = malloc(sizeof(int)); // a1
    assert(p != NULL);
    printf("(%d) address pointed to by p: %p\n",
    getpid(), p); // a2
    *p = 0; // a3
    while (1) {
      Spin(1);
      *p = *p + 1;
      printf("(%d) p: %d\n", getpid(), *p); // a4
      }
  return 0;
}
```
this code block here allocates memory using `malloc()`.
Traditionally memory is an array of bytes. It accessed when programs ran for things like writting/reading from datastructures or really anything in programming.

If this code was ran twice, both programs would allocate the same memory address. This is due to the physical memory being virtualized and having different address spaces.

**Concurrency** is an issue mostly because even if you have independent threads, the order of which each program reads, updates and stores the memory its pointing to is not atomic, so it will behave weirdly. 

**Persistence** is another problem in OS. Most data needs to be stored somewhere safe and most things store things in a volatile manner.

```C
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
  int fd = open("/tmp/file",
                O_WRONLY|O_CREAT|O_TRUNC,
                S_IRWXU);
  assert(fd > -1);
  int rc = write(fd, "hello world\n", 13);
  assert(rc == 13);
  close(fd);
return 0;
}
```

This code block creates a file in `tmp/file` that contains the string `hello world`. In order for the OS to do this it makes seperate calls such as `open()` `write()` `close()`. 

The process of actually writting involves the file system. It employs **journaling** and **copy-on-write** when writting so that if the system crashes it can be restored to a reasonable point after. 

**Isolation** is the idea that a process running on an OS does not effect another process and needed for **protection**.

# Processes
a process is simply a running program. The CPU gives the illusion of virtualization by running multiple processes with **time sharing**, or stopping a process and starting another. 

**time sharing**: allowing the resource to be used for a little while, then again by another, etc
**space sharing**: the same with memory. Dividing space among the processes wishing to use it.
**context switch**: gives the OS ability to stop running one program and start running another. 
**polices**: algorithims that make a decision within the OS. For example a **scheduling policy** will make a decision on which program to run when time sharing based off something like which program has been running the longest.

A process contains a **machine state** which tells the OS what a program can read of update while running. Part of this state is memory, since the instructions for the program are stored in memory in **registers**. It also includes a **program counter** (states which instruction of the program will execute next) and a **stack pointer** and **frame pointer**. Those are used to manage the stack for function parameters, local variables and return addresses. 
****
The OS loads any code from a program onto memory. Then a stack for runtime is allocated. The OS also may allocate memory for a heap, depending on the language the heap is used differently but in C the heap is used to request dynamically allocated data. 

when the program is loaded it is ready for execution and can be described by 3 states
![alt text](image.png)
![alt text](image-1.png)
the **OS scheduler** here decides how and when to schedule these processes. In this cased after process 0 was blocked it started process 1.
```c
// the registers xv6 will save and restore
// to stop and subsequently restart a process
struct context {
int eip;
int esp;
int ebx;
int ecx;
int edx;
int esi;
int edi;
int ebp;
};
// the different states a process can be in
enum proc_state { UNUSED, EMBRYO, SLEEPING,
RUNNABLE, RUNNING, ZOMBIE };
// the information xv6 tracks about each process
// including its register context and state
struct proc {
char *mem; // Start of process memory
uint sz; // Size of process memory
char *kstack; // Bottom of kernel stack
// for this process
enum proc_state state; // Process state
int pid; // Process ID
struct proc *parent; // Parent process
void *chan; // If !zero, sleeping on chan
int killed; // If !zero, has been killed
struct file *ofile[NOFILE]; // Open files
struct inode *cwd; // Current directory
struct context context; // Switch here to run process
struct trapframe *tf; // Trap frame for the
// current interrupt
};
```
An example of the data that a process will need to store. When a process stops, the registers are stored in memory and when starting up again the values are placed in the physical registers agian to start up.

**process list**: contains information about all processes in the system
**process control block**: basically a term for the data that is associated with a process.