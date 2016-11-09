#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
  int pid;
  if ((pid = fork()) != 0) {
    printf("parent: pid = %d\n", getpid());
    sleep(1);
    printf("parent: finish\n");
  } else {
    printf("child: ppid = %d\n", getppid());
    sleep(2);
    printf("child: ppid = %d\n", getppid());
    printf("child: finish\n");
  }
  return 0;
}

