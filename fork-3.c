#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_CHILD 10

int main(int argc, char **argv) {
  pid_t pid[MAX_CHILD];
  int i;

  for(i=0; i<MAX_CHILD; i++) {
    pid[i] = fork();

    if(pid[i] == 0) {
      int s = MAX_CHILD - i;
      printf("processo filho: pid %d gid %d\n", (int)getpid(), (int)getpgrp());
      sleep(s);
      printf("wake up\n");
      return 0;
    }
  }
  i = 0;

  while(i < MAX_CHILD) {
    pid_t p;
    int status;
    printf("Pai! pid %d\n", (int)getpid());
    p = waitpid(pid[i], &status, 0);
    printf("Pai(pid %d): filho %d acabou com status %d\n", (int)getpid(), (int)p, status);
    i++;
  }

  // if(pid == 0) {
  //   pid_t pid2;
  //   printf("processo filho: pid %d gid %d\n", (int)getpid(), (int)getpgrp());

  //   pid2 = fork();


  //   if(pid2 == 0) {
  //     int s = 5;
  //     printf("processo filho: pid %d gid %d\n", (int)getpid(), (int)getpgrp());
  //     sleep(s);
  //     printf("wake up\n");
  //     return 0;
  //   } else {
  //     pid_t p;
  //     int status;
  //     printf("Pai! pid %d\n", (int)getpid());
  //     p = waitpid(-1, &status, 0);
  //     printf("Pai(pid %d): filho %d acabou com status %d\n", (int)getpid(), (int)p, status);
  //   }
  // } else {
  //   pid_t p;
  //   int status;
  //   printf("Pai! pid %d\n", (int)getpid());
  //   p = waitpid(-1, &status, 0);
  //   printf("Pai(pid %d): filho %d acabou com status %d\n", (int)getpid(), (int)p, status);
  // }
  return 0;
}