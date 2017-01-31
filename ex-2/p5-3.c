#include <stdio.h>
#include <pthread.h>

/* Start semaphore definition */
struct my_semaphore {
  int n;
  pthread_mutex_t mtx;
  pthread_cond_t cond;
};

#define MY_SEMAPHORE(s, n) struct my_semaphore s = {\
  n, PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER\
};

void my_wait(struct my_semaphore *s)
{
  pthread_mutex_lock(&s->mtx);
  while (s->n == 0) pthread_cond_wait(&s->cond, &s->mtx);
  s->n--;
  pthread_mutex_unlock(&s->mtx);
}

void my_signal(struct my_semaphore *s)
{
  pthread_mutex_lock(&s->mtx);
  s->n++;
  pthread_mutex_unlock(&s->mtx);
  pthread_cond_signal(&s->cond);
}

/* End semaphore definition */

#define N 5

int buffer[N];
int inptr = 0, outptr = 0;

MY_SEMAPHORE(Elements, 0);
MY_SEMAPHORE(Spaces, N);

static int sum = 0;

int produce()
{
  static c = 1;
  return c > 1000 ? -1: c++;
}

void consume(int data)
{
  sum += data;
}

void *producer(void *arg)
{
  int data;
  for (;;) {
    data = produce();
    if (data < 0)
      break;
    my_wait(&Spaces);
    buffer[inptr] = data;
    inptr = (inptr + 1) % N;
    my_signal(&Elements);
  }
}

void *consumer(void *arg)
{
  int data;
  for (;;) {
    my_wait(&Elements);
    data = buffer[outptr];
    outptr = (outptr + 1) % N;
    my_signal(&Spaces);
    consume(data);
    if (data == 1000)
      break;
  }
}

int main()
{
  pthread_t p, c;

  pthread_create(&p, NULL, producer, NULL);
  pthread_create(&c, NULL, consumer, NULL);

  pthread_join(p, NULL);
  pthread_join(c, NULL);

  printf("sum = %d\n", sum);
  return 0;
}

