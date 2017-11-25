/*
 * exec in child process
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  printf("starting... (pid:%d)\n", (int)getpid());

  int rc = fork();
  if (rc < 0)
  {
    // fork failed, exit
    fprintf(stderr, "fork failed\n");
    exit(1);
  }
  else if (rc == 0)
  {
    // child process
    char *args_ls[4];
    args_ls[0] = strdup("/bin/ls");
    args_ls[1] = strdup("-lha");
    args_ls[2] = strdup("/home/bob/");
    args_ls[3] = NULL;
    execv(args_ls[0], args_ls);
  }
  else
  {
    // parent process
    int status;
    waitpid(rc, &status, 0);
    printf("goodbye\n");
  }

  return 0;
}
