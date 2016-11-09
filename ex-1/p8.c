#include <stdio.h>
#include <unistd.h>

int main()
{
  int pid;
  if ((pid = fork()) != 0) {
    getchar();
    printf("parent: finish\n");
  } else {
    printf("child: finish\n");
  }
  return 0;
}

