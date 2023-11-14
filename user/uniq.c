#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#include <ctype.h>
#include <stdbool.h>

#define LINE_BUFFER_SIZE 256

int chartolower(int chr) {
    if (chr >= 'A' && chr <= 'Z') {
        return chr + 32;
    }
    return chr;
}

int strcasecmp(const char *s1, const char *s2) {
    while (*s1 && chartolower(*s2) == chartolower(*s1)) {
        s2=s2+1;
        s1=s1+1;
    }

    return chartolower(*s1) - chartolower(*s2);
}

void
uniq_userfunction(const char* mode, int fd)
{
  fprintf(2, "\nUniq command is getting executed in user mode\n\n");

  if(strcmp(mode,"-i")==0) {
    int n;
    int length = 0;
    char currentLine[LINE_BUFFER_SIZE];
    char previousLine[LINE_BUFFER_SIZE] = "";

    while((n = read(fd, &currentLine[length], 1)) > 0)
    {
      if(currentLine[length] == '\n')
      {
        currentLine[length] = '\0';

        if(strcasecmp(previousLine,currentLine)!=0) {
          fprintf(2, "%s\n", currentLine);
        }
        
        strcpy(previousLine, currentLine);
        length = 0;
      }
      else
      {
        length++;
      }
    }

    if(n < 0){
      fprintf(2, "uniq: read error\n");
      exit(1);
    }

  } 
  else if(strcmp(mode,"-c")==0) {
    int n, count = 1, length = 0;
    bool duplicateFound = false;
    char currentLine[LINE_BUFFER_SIZE];
    char previousLine[LINE_BUFFER_SIZE] = "";

    while((n = read(fd, &currentLine[length], 1)) > 0) {
      if(currentLine[length] == '\n') {
        currentLine[length] = '\0';
        if(strcmp(previousLine,currentLine) == 0){
          duplicateFound = true;
          count++;
        }
        else {
					if(duplicateFound == true) {
            fprintf(2, "%d - %s\n", count, previousLine);
            duplicateFound = false;
          }
					else {
						if(strcmp(previousLine,"")!=0)
							fprintf(2, "%d - %s\n", 1, previousLine);
					}
					count = 1;
        }
        
        strcpy(previousLine, currentLine);
        length = 0;
      }
      else {
        length++;
      }
    }
    
    fprintf(2, "%d - %s\n", count, currentLine);

    if(n < 0){
      fprintf(2, "uniq: read error\n");
      exit(1);
    }
  } 
  else if(strcmp(mode,"-d")==0) {
    int n;
    int length = 0;
    bool duplicateFound = false;
    char currentLine[LINE_BUFFER_SIZE];
    char previousLine[LINE_BUFFER_SIZE] = "";

    while((n = read(fd, &currentLine[length], 1)) > 0)
    {
      if(currentLine[length] == '\n')
      {
        currentLine[length] = '\0';
        if(strcmp(previousLine,currentLine)==0)
          duplicateFound = true;
        else
        {
          if(duplicateFound == true){
            fprintf(2, "%s\n", previousLine);
            duplicateFound = false;
          }
        }
        
        strcpy(previousLine, currentLine);
        length = 0;
      }
      else
      {
        length++;
      }
    }

    if(n < 0){
      fprintf(2, "uniq: read error\n");
      exit(1);
    }
  } 
  else {
    int n;
    int length = 0;
    char currentLine[LINE_BUFFER_SIZE];
    char previousLine[LINE_BUFFER_SIZE] = "";

    while((n = read(fd, &currentLine[length], 1)) > 0)
    {
      if(currentLine[length] == '\n')
      {
        currentLine[length] = '\0';
        if(strcmp(previousLine,currentLine)!=0) {
          fprintf(2, "%s\n", currentLine);
        }
        
        strcpy(previousLine, currentLine);
        length = 0;
      }
      else
      {
        length++;
      }
    }

    if(n < 0){
      fprintf(2, "uniq: read error\n");
      exit(1);
    }
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  // (cat example.txt | uniq) pattern  
  if(argc == 1) {
    uniq_userfunction("", 0);
  } 
  else if(strcmp(argv[1],"-i") == 0 || strcmp(argv[1],"-d") == 0 || strcmp(argv[1],"-c") == 0) {
    // making a head system call ($uniq -i example.txt) pattern
    int fd;
    if((fd = open(argv[2], 0)) < 0){
      fprintf(2, "uniq: cannot open %s\n", argv[2]);
      exit(1);
    }
    uniq_userfunction(argv[1], fd);

    // // making a uniq system call
    // fd = open(argv[2], 0);
    // char buffer[2048];
    // int bytescount = read(fd, buffer, 2048);
    // uniq(argv[1], bytescount, buffer);
    close(fd);
  } 
  else{
    // making a head system call
    int fd;
    if((fd = open(argv[1], 0)) < 0){
      fprintf(2, "uniq: cannot open %s\n", argv[1]);
      exit(1);
    }
    uniq_userfunction("", fd);

    // // making a uniq system call
    // fd = open(argv[1], 0);
    // char buffer[2048];
    // int bytescount = read(fd, buffer, 2048);
    // uniq("", bytescount, buffer);
    close(fd);
  }

  exit(0);
}
