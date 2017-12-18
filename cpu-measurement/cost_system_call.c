/*
 * measure the cost of a system call
 */

#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  const unsigned int ITERATIONS = 100000;
  unsigned long total_sec = 0, total_usec = 0;

  struct timeval before, after;

  int read_fd;
  char *buf;
  long diff_sec, diff_usec;
  for (int i = 0; i < ITERATIONS; i++)
  {
    // measure time for a system call
    gettimeofday(&before, NULL);
    read(read_fd, buf, 0);
    gettimeofday(&after, NULL);

    // add time taken to total time for all measurements
    diff_sec = after.tv_sec - before.tv_sec;
    diff_usec = after.tv_usec - before.tv_usec;
    while (diff_usec > 1E6)
    {
      diff_sec += 1;
      diff_usec -= 1000000;
    }

    total_sec += diff_sec;
    total_usec += diff_usec;
    while (total_usec > 1E6)
    {
      total_sec += 1;
      total_usec -= 1000000;
    }
  }

  float avg_sec = (1.0 * total_sec / ITERATIONS);
  float avg_usec = (1.0 * total_usec / ITERATIONS);

  printf(
      "average system call time: %.2f sec %.2f usec for %u iterations (total "
      "%lu "
      "sec %lu usec)\n",
      avg_sec, avg_usec, ITERATIONS, total_sec, total_usec);

  return 0;
}
