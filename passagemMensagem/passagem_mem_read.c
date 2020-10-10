//gcc -o read read.c -lrt -lpthread
#include <stdlib.h>
#include <stdio.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_GAULESES 8
#define BUFFER_JAVALIS 15

typedef struct messbuf
{
  int v;
  int a;
} messbuf_t;
pthread_mutex_t mutex;
sem_t full, sttBuffer;
char nome[] = {'A', 'N', 'D', 'E', 'R', 'S', 'O', 'N'};
mqd_t fd;
messbuf_t *ptr;
messbuf_t data;
void inicializar()
{
  pthread_mutex_init(&mutex, NULL);
  sem_init(&full, 0, BUFFER_JAVALIS);
  sem_init(&sttBuffer, 0, 0);
}
int remove_item(int val, messbuf_t *da)
{
  if (da->v > 0)
  {
    da->v -= val;
    return 0;
  }
  else
  {
    return 1;
  }
}
void *servir(void *param)
{
  int item = 1;
  char *val = (char *)param;
  while (1)
  {
    sem_wait(&full);
    pthread_mutex_lock(&mutex);
    sleep(1);
    char nom = nome[*val];
    if (!remove_item(item, &data))
    {
      printf("Gaules %c(%d) comendo  - QTD: %d\n", *val, 0, ptr->v);
    }
    if (ptr->v <= 0)
    {
      ptr->a = 0;
      ptr->v = 0;
      mq_send(fd, (void *)ptr, sizeof(messbuf_t), 0);
    }

    pthread_mutex_unlock(&mutex);
  }
}
int main()
{
  struct mq_attr attr;
  ptr = &data;
  pthread_t gauleses[NUM_GAULESES];
  fd = mq_open("/yuo", O_RDWR);

  int i;
  printf("INI\n");
  while (1)
  {
    mq_receive(fd, (void *)ptr, sizeof(messbuf_t), 0);
    if (ptr->a == 1)
    {

      inicializar();
      for (i = 0; i < (NUM_GAULESES); i++)
      {
        pthread_create(&gauleses[i], NULL, servir, &nome[i]);
      }
    }

    sleep(1);
  }
}