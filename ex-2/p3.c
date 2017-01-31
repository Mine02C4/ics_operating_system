#include <stdio.h>
#include <pthread.h>

static int count = 0;

void *threadA(void *arg)
{
  int i;
  int tmp;

  for (i = 0; i < 1000; i++) {
    tmp = count;
    tmp = tmp + 1;
    count = tmp;
  }

  return NULL;
}

void *threadB(void *arg)
{
  int i;
  int tmp;

  for (i = 0; i < 1000; i++) {
    tmp = count;
    tmp = tmp - 1;
    count = tmp;
  }

  return NULL;
}


int main()
{
  pthread_t a, b;

  pthread_create(&a, NULL, threadA, NULL);
  pthread_create(&b, NULL, threadB, NULL);

  pthread_join(a, NULL);
  pthread_join(b, NULL);

  printf("count = %d\n", count);

  return 0;
}

