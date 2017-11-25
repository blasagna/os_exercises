/*
 * access a file descriptor concurrently in both parent and child processes
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  printf("starting... (pid:%d)\n", (int)getpid());
  close(STDOUT_FILENO);
  open("./q2.output", O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);

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
    int iter_c = 3;
    for (int i = 0; i < iter_c; i++)
    {
      printf("child: %d/%d (pid:%d)\n", i + 1, iter_c, (int)getpid());
    }
  }
  else
  {
    // parent process
    int iter_p = 5;
    for (int j = 0; j < iter_p; j++)
    {
      printf("parent of %d: %d/%d (pid:%d)\n", rc, j + 1, iter_p,
             (int)getpid());
    }
    wait(NULL);
  }

  return 0;
}
