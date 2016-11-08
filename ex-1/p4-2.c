#include <unistd.h>

int main()
{
  char *eargv[] = { "ls", "-l", NULL };
  execvp(eargv[0], eargv);
  return 0;
}

