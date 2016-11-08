#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_LINE 256

void getargs(char*, int, int*, char**);

int main()
{
  char line[MAX_LINE];
  while (1) {
    printf("%% ");
    fflush(stdout);
    if (fgets(line, MAX_LINE, stdin) == NULL) {
      fprintf(stderr, "Input error when fgets.\n");
      return 1;
    }
    int argc;
    char *argv[MAX_LINE];
    int pid;
    if ((pid = fork()) != 0) {
      int status;
      wait(&status);
    } else {
      getargs(line, MAX_LINE, &argc, argv);
      argv[argc] = NULL;
      execvp(argv[0], argv);
    }
  }
  return 0;
}

char *skipcs = " \n\t";

void
getargs(char *input, int max, int *argc, char *argv[])
{
  int i;
  *argc = 0;
  int npos = 0, prev_skip = 1;
  for (i = 0; i < max; i++) {
    if (input[i] == '\0') {
      if (prev_skip == 0) {
        argv[(*argc)++] = input + npos;
      }
      break;
    }
    int skip = 0, j;
    for (j = 0; skipcs[j] != '\0'; j++) {
      if (input[i] == skipcs[j]) {
        skip = 1;
        break;
      }
    }
    if (skip != 0 && prev_skip == 0) {
      argv[(*argc)++] = input + npos;
      input[i] = '\0';
    } else if (skip == 0 && prev_skip != 0) {
      npos = i;
    }
    prev_skip = skip;
  }
}

