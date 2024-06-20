#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

volatile uint64_t handled = 0;

void handle(int sig) {
  handled |= (1 << sig);
  printf("Caught %d: %s (%d total)\n", sig, sys_siglist[sig],
         __builtin_popcount(handled));
}

int main(int argc, char* argv[]) {
  int pid = getpid();
  printf("Starting process %d\n", pid);

  // Register all valid signals
  for (int i = 0; i < NSIG; i++) {
      signal(i, handle);
  }

  for (int i = 1; i < NSIG; i++) {
    if (i == 9 || i == 17) continue;
    kill(pid, i);
  }

  printf("\nKilling the process for my grand total of 30 triggered signals...\n");
  kill(pid, 9);
}
