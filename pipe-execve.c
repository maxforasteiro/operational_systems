#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
  char *cmd1[] = {"/bin/ls", "-la", NULL};
  char *cmd2[] = {"/bin/grep", "execve", NULL};
  pid_t pid;
  int fp[2];

  if(pipe(fp) < 0) {
    perror("pipe(fp)");
    return -1;
  }

  pid = fork();

  if(pid == 0) { // filho
    close(fp[1]);
    close(STDIN_FILENO);
    dup(fp[0]);
    close(fp[0]);
    execve(cmd2[0], cmd2, NULL);
  } else { // pai
    close(fp[0]);
    close(STDOUT_FILENO);
    dup(fp[1]);
    close(fp[1]);
    execve(cmd1[0], cmd1, NULL);
  }

  return 0;
}