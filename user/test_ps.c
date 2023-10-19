#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  // To test PS system call, I have created a child process.
  // And inside the child process, executed PS system call
  // It prints the new child process information aswell.
  
  ps();   // call 1
  int pid = fork();
  if (pid == 0) {
    ps();   // call 2
  } 
  else {
    wait(0);
    ps();   // call 3
  }

  exit(0);
}

