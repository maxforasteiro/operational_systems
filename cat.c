#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_BUFF_LENG 1024

int main(int argc, char **argv) {
  pid_t pid;
  int p1[2], p2[2];
  if(pipe(p1) < 0 || pipe(p2) < 0) {
    perror("pipe()");
    return -1;
  }

  pid = fork();

  if(pid == 0){
    close(p1[1]);
    close(p2[0]);
    char filepath[MAX_BUFF_LENG], fbuff[MAX_BUFF_LENG];
    char *error_message = "Error while reading the file\n";
    int f;

    bzero(filepath, MAX_BUFF_LENG);

    read(p1[0], filepath, sizeof(char) * MAX_BUFF_LENG);
    f = open(filepath, O_RDONLY);

    if (f < 0) {
      write(p2[1], error_message, strlen(error_message));
      return 0;
    }

    int n;
    bzero(fbuff, MAX_BUFF_LENG);
    while((n = read(f, fbuff, sizeof(char) * MAX_BUFF_LENG)) > 0) {
      write(p2[1], fbuff, n);
      bzero(fbuff, MAX_BUFF_LENG);
    }
    return 0;
  } else {
    close(p1[0]);
    close(p2[1]);
    char *param = argv[1], buff[MAX_BUFF_LENG];
    int n;

    if(write(p1[1], param, sizeof(char) * strlen(param)) > 0) {
      bzero(buff, MAX_BUFF_LENG);
      while((n = read(p2[0], buff, sizeof(char) * MAX_BUFF_LENG)) > 0) {
        printf("%s", buff);
        bzero(buff, MAX_BUFF_LENG);
      }
    }

    return 0;
  }
}
