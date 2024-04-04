OS virtualization is making physical resources of a computer like processor, memory, etc into a virtual easy to use form of itself.

OS provides a "standard library" of methods applications can call. It is also considered a "resource manager" do to its role as managing and allocating resources.

An example of CPU virtualization is running multiple programs at once and having them run concurrently. Even though there is only 1 CPU due to virtualization it is able to handle everything.

Policies are used in lots of places in the OS, they essentially serve as a way for the OS to know what to do and when to handle certain scenarios.
ex, two programs are ran at once, which does the OS run first? 

```
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
