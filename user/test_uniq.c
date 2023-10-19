#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int fd = open(argv[1], 0);
  char buffer[2048];
  int bytescount = read(fd, buffer, 2048);
  
  // creating a child process to run uniq system call as a process
  int pid = fork();

  if(pid == 0){
      // making a uniq system call
      // first argument of head system call is mode (pass "-i", "-d","-c", "" to test options)
      uniq("-i", bytescount, buffer);
  }
  else{
    // parent wait for the child process to be exited
    // then creation, end and total times of above process is printed 
    wait(0);

    // closing the file descriptor
    close(fd);
  }
  exit(0);
}
