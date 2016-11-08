#include <unistd.h>

int main()
{
  char *eargv[] = { "ls", NULL };
  execvp(eargv[0], eargv);
  return 0;
}

