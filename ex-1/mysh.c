#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_LINE 256

void getargs(char*, int, int*, char**);
const char *getenv(const char *[], const char *);

int
main(const int margc, const char *margv[], const char *menvp[])
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
    getargs(line, MAX_LINE, &argc, argv);
    if (argc == 0) {
      continue;
    }
    if (strcmp(argv[0], "cd") == 0) {
      if (argc > 1) {
        chdir(argv[1]);
      } else {
        const char *home = getenv(menvp, "HOME");
        printf("%s\n", home);
        chdir(home);
      }
      continue;
    }
    if (strcmp(argv[0], "exit") == 0) {
      return 0;
    }
    int background = 0;
    if (argv[argc - 1][strlen(argv[argc - 1]) - 1] == '&') {
      background = 1;
      if (strlen(argv[argc - 1]) == 1) {
        argv[argc - 1] = NULL;
      } else {
        argv[argc - 1][strlen(argv[argc - 1]) - 1] = '\0';
        argv[argc] = NULL;
      }
    } else {
      argv[argc] = NULL;
    }
    int pid;
    if ((pid = fork()) != 0) {
      if (background == 0) {
        int status;
        wait(&status);
      }
    } else {
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

const char *
getenv(const char *envp[], const char *key)
{
  int i, j;
  for (i = 0; envp[i] != NULL; i++) {
    const char *val = NULL;
    int n = 0;
    for (j = 0; envp[j] != '\0'; j++) {
      if (envp[i][j] == '=') {
        val = &envp[i][j + 1];
        n = j;
        break;
      }
    }
    if (val == NULL) {
      break;
    }
    if (strncmp(envp[i], key, n) == 0) {
      return val;
    }
  }
  return NULL;
}

