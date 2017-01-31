#include <stdio.h>
#include <pthread.h>

#define N 5

int buffer[N];
int inptr = 0, outptr = 0;
int count = 0;

pthread_cond_t full = PTHREAD_COND_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

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
    pthread_mutex_lock(&lock);
    while (count >= N) pthread_cond_wait(&full, &lock);
    count = count + 1;
    buffer[inptr] = data;
    inptr = (inptr + 1) % N;
    pthread_mutex_unlock(&lock);
    pthread_cond_signal(&empty);
  }
}

void *consumer(void *arg)
{
  int data;
  for (;;) {
    pthread_mutex_lock(&lock);
    while (count == 0) pthread_cond_wait(&empty, &lock);
    count = count - 1;
    data = buffer[outptr];
    outptr = (outptr + 1) % N;
    pthread_mutex_unlock(&lock);
    pthread_cond_signal(&full);
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

