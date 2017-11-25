/*
 * pipe stdout of one child to stdin of another child process
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define PIPE_READ 0
#define PIPE_WRITE 1

int main(int argc, char *argv[])
{
  printf("starting... (pid:%d)\n", (int)getpid());
  int pipefd[2];
  pid_t cpid_src, cpid_dest;

  if (pipe(pipefd) == -1)
  {
    fprintf(stderr, "pipe failed\n");
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  cpid_src = fork();
  if (cpid_src < 0)
  {
    fprintf(stderr, "fork failed\n");
    exit(EXIT_FAILURE);
  }
  else if (cpid_src == 0)
  {
    // child process - pipe src
    char *args_echo[3];

    close(pipefd[PIPE_READ]);
    if (dup2(pipefd[PIPE_WRITE], STDOUT_FILENO) == -1)
    {
      fprintf(stderr, "stdout fd dup2 failed");
      exit(EXIT_FAILURE);
    }

    args_echo[0] = strdup("/bin/echo");
    args_echo[1] = strdup("foo");
    args_echo[2] = NULL;
    execv(args_echo[0], args_echo);

    close(pipefd[PIPE_WRITE]);
  }
  else
  {
    // parent process
    cpid_dest = fork();
    if (cpid_dest < 0)
    {
      fprintf(stderr, "fork failed\n");
      exit(EXIT_FAILURE);
    }
    else if (cpid_dest == 0)
    {
      // child process - pipe dest
      char *args_wc[2];

      close(pipefd[PIPE_WRITE]);
      if (dup2(pipefd[PIPE_READ], STDIN_FILENO) == -1)
      {
        fprintf(stderr, "stdin fd dup2 failed");
        exit(EXIT_FAILURE);
      }

      args_wc[0] = strdup("/usr/bin/wc");
      args_wc[1] = NULL;
      execv(args_wc[0], args_wc);

      close(pipefd[PIPE_READ]);
    }
    else
    {
      // parent process
      // close parent's pipes
      close(pipefd[PIPE_READ]);
      close(pipefd[PIPE_WRITE]);
      int status;
      waitpid(cpid_src, &status, 0);
      waitpid(cpid_dest, &status, 0);
    }
  }

  return 0;
}
