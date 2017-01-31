#include <stdio.h>
#include <pthread.h>

void *thread(void *arg)
{
  for (;;)
    printf("Thread (%f)\n", *(double*)arg);
  return NULL;
}

int main()
{
  pthread_t a, b;
  double arg_a = 3.14, arg_b = 2.71;

  pthread_create(&a, NULL, thread, &arg_a);
  pthread_create(&b, NULL, thread, &arg_b);

  pthread_join(a, NULL);
  pthread_join(b, NULL);

  return 0;
}

