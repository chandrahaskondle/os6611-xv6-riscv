#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#include <ctype.h>
#include <stdbool.h>

#define MAX_LINE_LENGTH 1024

int custom_tolower(int c) {
    if (c >= 'A' && c <= 'Z') {
        return c + 'a' - 'A';
    }
    return c;
}

int custom_strcasecmp(const char *s1, const char *s2) {
    while (*s1 && custom_tolower(*s1) == custom_tolower(*s2)) {
        s1++;
        s2++;
    }

    return custom_tolower(*s1) - custom_tolower(*s2);
}

void
uniq(const char* mode, const char* fileName)
{
  int fd;

  if((fd = open(fileName, 0)) < 0){
    fprintf(2, "uniq: cannot open %s\n", fileName);
    exit(1);
  }

  fprintf(2, "Uniq command is getting executed in user mode\n\n");

  if(strcmp(mode,"-i")==0) {
    int n;
    int length = 0;
    char currentLine[MAX_LINE_LENGTH];
    char previousLine[MAX_LINE_LENGTH] = "";

    while((n = read(fd, &currentLine[length], 1)) > 0)
    {
      if(currentLine[length] == '\n')
      {
        currentLine[length] = '\0';

        if(custom_strcasecmp(previousLine,currentLine)!=0) {
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
    char currentLine[MAX_LINE_LENGTH];
    char previousLine[MAX_LINE_LENGTH] = "";

    while((n = read(fd, &currentLine[length], 1)) > 0)
    {
      if(currentLine[length] == '\n')
      {
        currentLine[length] = '\0';
        if(strcmp(previousLine,currentLine) == 0){
          duplicateFound = true;
          count++;
        }
        else
        {
          if(duplicateFound == true){
            fprintf(2, "%d - %s\n", count, previousLine);
            duplicateFound = false;
            count = 1;
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
    char currentLine[MAX_LINE_LENGTH];
    char previousLine[MAX_LINE_LENGTH] = "";

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
    char currentLine[MAX_LINE_LENGTH];
    char previousLine[MAX_LINE_LENGTH] = "";

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

}

int
main(int argc, char *argv[])
{
  //int fd, indexI;
  // uniq -i example.txt
  // uniq example.txt

  if(argc <= 1) {
    fprintf(2, "Usage: No file name passed...\n");
    exit(1);
  }

  if(strcmp(argv[1],"-i") == 0 || strcmp(argv[1],"-d") == 0 || strcmp(argv[1],"-c") == 0)
    uniq(argv[1], argv[2]);
  else
    uniq("", argv[1]);

  exit(0);
}
