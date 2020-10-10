#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#define NUM_GAULESES 8
#define BUFFER_JAVALIS 15

pthread_mutex_t mutex;
sem_t full, sttBuffer;

//pthread_t tid;
int cno = 0;

typedef struct dados
{
  int counter;
  int comer;
} dado;
dado *dd;
char nome[] = {'A', 'N', 'D', 'E', 'R', 'S', 'O', 'N'};
void inicializar()
{
  pthread_mutex_init(&mutex, NULL);
  sem_init(&full, 0, BUFFER_JAVALIS);
  sem_init(&sttBuffer, 0, 0);
}

int remove_item(int val)
{
  if (dd->counter > 0)
  {
    dd->counter -= val;
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
    if (!remove_item(item))
    {
      printf("Gaules %c(%d) comendo  - QTD: %d\n", *val, 0, dd->counter);
    }
    if (dd->counter <= 0)
    {
      dd->comer = 0;
      printf("VALOR DD: %d\n", dd->comer);
    }
    pthread_mutex_unlock(&mutex);
  }
}

void main()
{

  int fd;

  fd = shm_open("/infod0", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
  ftruncate(fd, sizeof(dado));

  dd = mmap(NULL, sizeof(dado), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  int i;
  pthread_t gauleses[NUM_GAULESES];
  //pthread_t cozinheiro;

  printf("VALOR DD: %d\n", dd->comer);
  while (1)
  {
    if (dd->comer == 1)
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