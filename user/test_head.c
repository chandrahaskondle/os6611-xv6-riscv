#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int fd = open(argv[1], 0);
  char buffer[2048];
  int bytescount = read(fd, buffer, 2048);
  
  // creating a child process to run head command.
  int pid = fork();

  if(pid == 0){
      // making a head command system call
      head(14, bytescount, buffer);
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
