#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define MAX_BUFF (4*1024)
#define MAX_FNAME (256)

void server(int in, int out) {
  int fd, nr;
  char fname[MAX_FNAME], buff[MAX_BUFF];

  bzero(fname, MAX_FNAME);
  read(in, fname, MAX_FNAME);
  fd = open(fname, O_RDONLY);

  if(fd < 0) {
    bzero(buff, MAX_BUFF);
    strerror_r(errno, buff, MAX_BUFF);
    write(out, buff, strlen(buff));
    return;
  }

  nr = 0;

  do {
    bzero(buff, MAX_BUFF);
    nr = read(fd, buff, MAX_BUFF);
    if(nr > 0) {
      write(out, buff, nr);
    }
  } while(nr > 0);
  return;
}

void client(int in, int out, char *fname) {
  int nr;
  char buff[MAX_BUFF];

  write(out, fname, strlen(fname));
  nr = 0;

  do {
    nr = read(in, buff, MAX_BUFF);
    if (nr > 0) {
      write(STDOUT_FILENO, buff, nr);
    }
  } while(nr > 0);
}

int main(int argc, char **argv) {
  pid_t pid;
  int p_pf[2], p_fp[2];

  if(argc != 2) {
    printf("Usage: %s <filename>\n", argv[0]);
    return 0;
  }

  if(pipe(p_pf) < 0) {
    perror("pipe(p_pf)");
    return -1;
  }

  if(pipe(p_fp) < 0) {
    perror("pipe(p_fp)");
    return -1;
  }

  pid = fork();

  if(pid == 0){ // filho
    close(p_pf[1]);
    close(p_fp[0]);
    server(p_pf[0], p_fp[1]);
    return 0;
  } else { // pai
    close(p_pf[0]);
    close(p_fp[1]);
    client(p_fp[0], p_pf[1], argv[1]);
    waitpid(-1, NULL, 0);
    return 0;
  }
  return 0;
}
