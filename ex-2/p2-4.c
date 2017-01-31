#include <stdio.h>
#include <pthread.h>

void *thread(void *arg)
{
  for (;;)
    printf("Thread (%d)\n", *(int*)arg);
  return NULL;
}

int main()
{
  pthread_t t[2];
  int i;

  for (i = 0; i < 2; i++)
    pthread_create(&t[i], NULL, thread, &i);

  for (i = 0; i < 2; i++)
    pthread_join(t[i], NULL);

  return 0;
}

