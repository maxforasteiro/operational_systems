#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char **argv) {
  pid_t pid;
  int s;
  int fd[2];

  if(pipe(fd) < 0) {
    perror("pipe()");
    return -1;
  }

  pid = fork();

  if(pid == 0) {
    int nr, rs;
    nr = read(fd[0], &rs, sizeof(int));
    printf("processo filho: pid %d gid %d pfvr dormindo %d s\n", (int)getpid(), (int)getpgrp(), rs);
    sleep(rs);
    printf("wake up\n");
    return 0;
  } else {
    pid_t p;
    int status, nw;
    printf("Pai. entre tempo da soneca\n");
    scanf("%d", &s);
    nw = write(fd[1], &s, sizeof(int));
    p = waitpid(-1, &status, 0);
    printf("filho %d acabou com status %d\n", (int)p, status);
  }
  return 0;
}