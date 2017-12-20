/*
 * measure the cost of a context switch between processes
 */

#define _GNU_SOURCE
#include <fcntl.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  const unsigned int ITERATIONS = 100000;

  // bind process to CPU 0
  // NOTE: a child created via fork inherits its parent's CPU affinity
  cpu_set_t set;
  CPU_ZERO(&set);
  CPU_SET(0, &set);
  if (sched_setaffinity(getpid(), sizeof(cpu_set_t), &set) != 0)
  {
    perror("sched_setaffinity parent");
    exit(EXIT_FAILURE);
  }

  // set up pipes between processes
  int pipe_p_to_c[2], pipe_c_to_p[2];

  if (pipe(pipe_p_to_c) != 0)
  {
    perror("pipe parent to child");
    exit(EXIT_FAILURE);
  }
  if (pipe(pipe_c_to_p) != 0)
  {
    perror("pipe child to parent");
    exit(EXIT_FAILURE);
  }

  char rxbuf[1], txbuf[1];

  pid_t rc = fork();
  if (rc < 0)
  {
    perror("fork");
    exit(EXIT_FAILURE);
  }
  else if (rc == 0)
  {
    // child process
    txbuf[0] = 'c';

    for (int i = 0; i < ITERATIONS; i++)
    {
      read(pipe_p_to_c[0], rxbuf, sizeof(rxbuf));
      write(pipe_c_to_p[1], txbuf, 1);
    }
  }
  else
  {
    // parent process
    struct timeval start, end;
    txbuf[0] = 'p';

    gettimeofday(&start, NULL);

    for (int i = 0; i < ITERATIONS; i++)
    {
      write(pipe_p_to_c[1], txbuf, 1);
      read(pipe_c_to_p[0], rxbuf, sizeof(rxbuf));
    }

    int status;
    waitpid(rc, &status, 0);

    gettimeofday(&end, NULL);
    unsigned long elapsed_us =
        1E6 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
    float avg_switch_time = (1.0 * elapsed_us) / ITERATIONS / 2;
    printf("average context switch time: %.2f usec for %u iterations",
           avg_switch_time, ITERATIONS);
  }

  return 0;
}
