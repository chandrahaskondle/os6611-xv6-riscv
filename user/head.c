#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define LINE_BUFFER_SIZE 256

void
head_userprogam(int topN, int fd)
{
  int n = 0;
  int linesPrintedCount = 0;
  int linePointer = 0;
  char currentLine[LINE_BUFFER_SIZE];

  fprintf(1, "\nHead command is getting executed in user mode\n\n");

  while((linesPrintedCount != topN) && ((n = read(fd, &currentLine[linePointer], 1))) > 0)
  {
    if(currentLine[linePointer] == '\n')
    {
      currentLine[linePointer] = '\0';
      fprintf(2, "%s\n", currentLine);
      linesPrintedCount++;        
      linePointer = 0;
    }
    else
    {
      linePointer++;
    }
  }

  if(n < 0){
    fprintf(2, "head: read error\n");
    exit(1);
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  if(argc<=1) {
    head_userprogam(14,2);
  }

  if(strcmp(argv[1],"-n") == 0) {
    // ($head -n 4 abc.txt) pattern
    for (int i = 3; i < argc; i++)
    {
      // making a head command user call
      int fd;
      if((fd = open(argv[i], 0)) < 0){
        fprintf(2, "head: cannot open %s\n", argv[i]);
        exit(1);
      }
      fprintf(2, "\nPrinting the first %s lines of file: %s \n=====>", argv[2], argv[i]);
      head_userprogam(atoi(argv[2]), fd);
      close(fd);
      
      // making a head command system call
      fd = open(argv[i], 0);
      char buffer[1024];
      int bytescount = read(fd, buffer, 1024);
      head(atoi(argv[2]), bytescount,  buffer);
      memset(buffer, 0, sizeof(buffer));
      close(fd);
    }
  }
  else {
    // ($head abc.txt) pattern
    for (int i = 1; i < argc; i++)
    {
      // making a head command user call
      int fd;
      if((fd = open(argv[i], 0)) < 0){
        fprintf(2, "head: cannot open %s\n", argv[i]);
        exit(1);
      }
      fprintf(2, "\nPrinting the first 14 lines of file: %s \n=====>", argv[i]);
      head_userprogam(14, fd);
      close(fd);

      // making a head command system call
      fd = open(argv[i], 0);
      char buffer[2048];
      int bytescount = read(fd, buffer, 2048);
      head(14, bytescount, buffer);
      memset(buffer, 0, sizeof(buffer));
      close(fd);
    }
  }
  
  exit(0);
}
