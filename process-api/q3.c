/*
 * coordinate child process output before parent process output
 */

#include <stdio.h>
#include <stdlib.h>
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
    printf("hello\n");
  }
  else
  {
    // parent process
    // wait(NULL);
    // sleep(1);
    int status;
    waitpid(rc, &status, 0);
    printf("goodbye\n");
  }

  return 0;
}
