//gcc -o papir papir.c -lrt -lpthread
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>

#define NUM_GAULESES 8
#define BUFFER_JAVALIS 15

typedef struct pipebuf
{
  int v;
  int a;
} pipebuf_t;
pthread_mutex_t mutex;
sem_t full, sttBuffer;
char nome[] = {'A', 'N', 'D', 'E', 'R', 'S', 'O', 'N'};
int fd, fd2;
pipebuf_t data;
pipebuf_t *ptr = &data;

void inicializar()
{
  pthread_mutex_init(&mutex, NULL);
  sem_init(&full, 0, BUFFER_JAVALIS);
  sem_init(&sttBuffer, 0, 0);
}
int remove_item(int val, pipebuf_t *da)
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
      write(fd2, ptr, sizeof(pipebuf_t));
    }

    pthread_mutex_unlock(&mutex);
  }
}

int main()
{
  mkfifo("myfifo", 0666);
  fd = open("myfifo", O_RDONLY);
  mkfifo("myfifo2", 0666);
  fd2 = open("myfifo2", O_WRONLY);
  ptr->v = 0;
  ptr->a = 0;
  int item = 1;
  int i;
  pthread_t gauleses[NUM_GAULESES];

  while (1)
  {
    if (ptr->a == 1)
    {
      inicializar();
      for (i = 0; i < (NUM_GAULESES); i++)
      {
        pthread_create(&gauleses[i], NULL, servir, &nome[i]);
      }
    }
    else
    {
      read(fd, ptr, sizeof(pipebuf_t));
    }
    sleep(1);
  }
}
