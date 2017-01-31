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
  pthread_t a, b;
  int arg_a = 0, arg_b = 1;

  pthread_create(&a, NULL, thread, &arg_a);
  pthread_create(&b, NULL, thread, &arg_b);

  pthread_join(a, NULL);
  pthread_join(b, NULL);

  return 0;
}

