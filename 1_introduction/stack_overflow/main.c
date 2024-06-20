#include <stdio.h>
#include <sys/resource.h>
#include <unistd.h>

/*
This program explores the stack in a process and the stack frame

The stack is both a data structure and memory region, storing data such as:
- Function return address (i.e. the address of the next instruction after the function returns)
- Local variables
- Function parameters

The stack frame is the organization of this data per function
Thus, every function invocation has its own stack frame
This means that the stack frame varies with each function and its data

In the below example:
- `frame` is the stack frame iteration of `recurse`
- `addr` is the approximate address of the current stack frame
- `bytes used in recursion` is the number of stack bytes used up to said frame

`bytes used in recursion` is possible by getting the delta of
i) the address of the first stack frame
ii) the address of the current stack frame

*/

void recurse(int depth, long int stack_top) {
  if (depth % 10000 == 0) {
    fprintf(stdout, "frame: %d, addr %p, bytes used in recursion: %ld\n", depth, &depth, stack_top - (long)&depth);
  }
  recurse(depth+1, stack_top);
  return;
}

void start() {
  int stack_top = 0;
  recurse(stack_top, (long)&stack_top);
  RLIM_INFINITY;
  return;
}

int main() {
  long page_size = sysconf(_SC_PAGESIZE);
  printf("Memory page size: %ld bytes\n", page_size); // 4096

  struct rlimit rlim;
  getrlimit(RLIMIT_STACK, &rlim);
  printf("Current max: %llu, hard limit: %llu\n", rlim.rlim_cur, rlim.rlim_max);

  rlim.rlim_max = 10000000;
  int err = setrlimit(RLIMIT_STACK, &rlim);
  if (err == -1) {
    perror("setrlimit error:");
  }

  getrlimit(RLIMIT_STACK, &rlim);
  printf("Current max: %llu, hard limit: %llu\n", rlim.rlim_cur, rlim.rlim_max);

  start();
  return 0;
}
