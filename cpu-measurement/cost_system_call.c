/*
 * measure the cost of a system call
 */

#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  const unsigned int ITERATIONS = 100000;
  struct timeval start, end;

  int read_fd;
  char *buf;
  gettimeofday(&start, NULL);
  for (int i = 0; i < ITERATIONS; i++)
  {
    // measure time for a system call
    read(read_fd, buf, 0);
  }
  gettimeofday(&end, NULL);

  unsigned long elapsed_us =
      1E6 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);

  float avg_time = (1.0 * elapsed_us) / ITERATIONS;
  printf("average system call time: %.2f usec for %u iterations", avg_time,
         ITERATIONS);

  return 0;
}
