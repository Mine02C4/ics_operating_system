#include <stdio.h>
#include <unistd.h>

int main()
{
  int pid;
  if ((pid = fork()) != 0) {
    printf("parent: pid = %d, ppid = %d\n", getpid(), getppid());
  } else {
    printf("child: pid = %d, ppid = %d\n", getpid(), getppid());
  }
  return 0;
}

