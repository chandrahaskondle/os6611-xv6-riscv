#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

#include <stdbool.h>

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}



uint64 chartolower(int chr) {
    if (chr >= 'A' && chr <= 'Z') {
        return chr + 32;
    }
    return chr;
}

uint64 strcasecmp(const char *s1, const char *s2) {
    while (*s1 && chartolower(*s2) == chartolower(*s1)) {
        s2=s2+1;
        s1=s1+1;
    }

    return chartolower(*s1) - chartolower(*s2);
}


// copied uniq.c strcpy function.
char*
strcpy(char *s, const char *t)
{
  char *os;

  os = s;
  while((*s++ = *t++) != 0)
    ;
  return os;
}

uint64
sys_uniq(void) {
  char mode[10];
  char buffer[2048];
  int maxBytes;
  argstr(0, mode, 2);
  argint(1, &maxBytes);
  argstr(2, buffer, maxBytes);

  printf("\nUniq command is getting executed in kernel mode\n\n");

  if(strncmp(mode,"-i",2)==0) {
    int length = 0;
    char currentLine[256];
    char previousLine[256] = "";

    for(int charIndex=0; buffer[charIndex] !='\0'; charIndex++ ) {
      if(buffer[charIndex] == '\n')
      {
        currentLine[length] = '\0';

        if(strcasecmp(previousLine,currentLine)!=0) {
          printf("%s\n", currentLine);
        }
        
        strcpy(previousLine, currentLine);
        length = 0;
      }
      else
      {
        currentLine[length] = buffer[charIndex];
        length++;
      }
    }

  } else if(strncmp(mode,"-c",2)==0) {
    int count = 1, length = 0;
    bool duplicateFound = false;
    char currentLine[256];
    char previousLine[256] = "";

    for(int charIndex=0; buffer[charIndex] !='\0'; charIndex++ ) {
      if(buffer[charIndex] == '\n' ) {
        currentLine[length] = '\0';
        if(strncmp(previousLine,currentLine,256) == 0){
          duplicateFound = true;
          count++;
        }
        else {
					if(duplicateFound == true) {
            printf("%d - %s\n", count, previousLine);
            duplicateFound = false;
          }
					else {
						if(strncmp(previousLine,"",256)!=0)
							printf("%d - %s\n", 1, previousLine);
					}
					count = 1;
        }
        
        strcpy(previousLine, currentLine);
        length = 0;
      }
      else {
        currentLine[length] = buffer[charIndex];
        length++;
      }
    }
    
    printf("%d - %s\n", count, previousLine);

  } 
  else if(strncmp(mode,"-d",2)==0) {
    int length = 0;
    bool duplicateFound = false;
    char currentLine[256];
    char previousLine[256] = "";

    for (int charIndex = 0; buffer[charIndex] != '\0'; charIndex++) 
    {
      if(buffer[charIndex] == '\n')
      {
        currentLine[length] = '\0';
        if(strncmp(previousLine,currentLine, 256)==0)
          duplicateFound = true;
        else
        {
          if(duplicateFound == true){
            printf("%s\n", previousLine);
            duplicateFound = false;
          }
        }
        
        strcpy(previousLine, currentLine);
        length = 0;
      }
      else
      {
        currentLine[length] = buffer[charIndex];
        length++;
      }
    }
  } 
  else {
    int length = 0;
    char currentLine[256];
    char previousLine[256] = "";

    for (int charIndex = 0; buffer[charIndex] != '\0'; charIndex++) {
      if (buffer[charIndex] == '\n') {
        currentLine[length] = '\0';
        if(strncmp(previousLine,currentLine, 256)!=0) {
          printf("%s\n", currentLine);
        }
        
        strcpy(previousLine, currentLine);
        length = 0;
      }
      else
      {
        currentLine[length] = buffer[charIndex];
        length++;
      }
    }
  }

  memset(buffer, 0, sizeof(buffer));
  return 0;
}


uint64
sys_head(void) {
  int topN;
  int maxBytes;
  int linesPrintedCount = 0;
  char buffer[2048];

  printf("\nHead command is getting executed in kernel mode\n\n");
  
  argint(0, &topN);
  argint(1, &maxBytes);
  argstr(2, buffer, maxBytes);

  for(int charIndex=0; buffer[charIndex] !='\0'; charIndex++ ) {
    if(buffer[charIndex] == '\n'){
      linesPrintedCount++;
    }
    consputc(buffer[charIndex]);
    if(linesPrintedCount == topN) {
      break;
    }
  }

  memset(buffer, 0, sizeof(buffer));

  return 0;
}

uint64
sys_ps(void)
{
  ps();
  return 0;
}

uint64
sys_putil(void)
{
  int pid, pr, flag;
  char pname[16];
  argint(0, &pid);
  argstr(1, pname, 16);
  argint(2, &pr);
  argint(3, &flag);

  return putil(pid, pname, pr, flag);
}

