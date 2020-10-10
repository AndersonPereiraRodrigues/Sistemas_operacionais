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
messbuf_t data;
messbuf_t *ptr = &data;

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
int insert_item(int val, messbuf_t *da)
{
  da->v += val;
  return 0;
}
void *servir(void *param)
{
  int item = 1;
  char *val = (char *)param;
  while (1)
  {
    sem_wait(&full);
    pthread_mutex_lock(&mutex);
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

void *cozinha()
{
  int item = 1;
  if (ptr->a == 0)
  {
    if (!insert_item(item, &data))
    {
      printf("+%d - Javali na mesa - QTD: %d\n", item, ptr->v);
    }
    else
    {
      fprintf(stderr, "error condition\n");
    }
    if (ptr->v >= (BUFFER_JAVALIS))
    {
      ptr->a = 1;
      mq_send(fd, (void *)ptr, sizeof(messbuf_t), 0);
    }
  }
  else
  {
    mq_receive(fd, (void *)ptr, sizeof(messbuf_t), 0);
    if (ptr->a == 1)
    {
      mq_send(fd, (void *)ptr, sizeof(messbuf_t), 0);
    }
  }
}
void *come()
{
  int i;
  pthread_t gauleses[NUM_GAULESES];
  mq_receive(fd, (void *)ptr, sizeof(messbuf_t), 0);
  if (ptr->a == 1)
  {
    inicializar();
    for (i = 0; i < (NUM_GAULESES); i++)
    {
      pthread_create(&gauleses[i], NULL, servir, &nome[i]);
    }
  }
}

int main()
{

  mqd_t fd;
  struct mq_attr attr;

  attr.mq_maxmsg = 1;
  attr.mq_msgsize = sizeof(messbuf_t);
  attr.mq_flags = 0;

  mq_unlink("/yuo");
  fd = mq_open("/yuo", O_RDWR | O_CREAT, 0666, &attr);

  ptr->v = 0;
  ptr->a = 0;

  pthread_t op;
  pthread_t op2;

  while (1)
  {
    pthread_create(&op, NULL, cozinha, NULL);
    pthread_join(op, NULL);
    pthread_create(&op2, NULL, come, NULL);
    pthread_join(op2, NULL);
    sleep(1);
  }
}