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

pthread_mutex_t mutex2;
sem_t full2, sttBuffer2;

pthread_t tid;
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
  dd->counter = 0;
  dd->comer = 0;
  pthread_mutex_init(&mutex2, NULL);
  sem_init(&full2, 0, 0);
  sem_init(&sttBuffer2, 0, BUFFER_JAVALIS);
}
int insert_item(int val)
{
  dd->counter += val;
  return 0;
}
void *cozinhar(void *param)
{
  int item = 1;
  while (1)
  {
    sem_wait(&sttBuffer2);
    pthread_mutex_lock(&mutex2);
    if (!insert_item(item))
    {
      printf("+%d - Javali na mesa - QTD: %d\n", item, dd->counter);
    }
    else
    {
      fprintf(stderr, "error condition\n");
    }
    pthread_mutex_unlock(&mutex2);
    if (dd->counter >= (BUFFER_JAVALIS))
    {
      dd->comer = 1;
      printf("VALOR DD: %d\n", dd->comer);
    }
  }
}

void main()
{
  int fd;
  fd = shm_open("/infod0", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
  ftruncate(fd, sizeof(dado));
  dd = mmap(NULL, sizeof(dado), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  int i;
  pthread_t cozinheiro;
  while (1)
  {
    if (dd->comer == 0)
    {
      inicializar();
      pthread_create(&cozinheiro, NULL, cozinhar, NULL);
    }
    sleep(1);
  }
}