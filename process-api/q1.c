/*
 * modify a variable in both parent and child processes
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  printf("starting... (pid:%d)\n", (int)getpid());
  int x = 3;
  printf("x = %d (pid:%d)\n", x, (int)getpid());

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
    printf("child: x = %d (pid:%d)\n", x, (int)getpid());
    x = 4;
    printf("child: x = %d (pid:%d)\n", x, (int)getpid());
  }
  else
  {
    // parent process
    printf("parent of %d: x = %d (pid:%d)\n", rc, x, (int)getpid());
    x = 5;
    printf("parent of %d: x = %d (pid:%d)\n", rc, x, (int)getpid());
    wait(NULL);
  }

  return 0;
}
