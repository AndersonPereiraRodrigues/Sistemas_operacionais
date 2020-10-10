//gcc -o papiw papiw.c -lrt -lpthread
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

pthread_mutex_t mutex2;
sem_t full2, sttBuffer2;

typedef struct pipebuf
{
  int v;
  int a;
} pipebuf_t;

char nome[] = {'A', 'N', 'D', 'E', 'R', 'S', 'O', 'N'};

int insert_item(int val, pipebuf_t *da)
{
  da->v += val;
  return 0;
}

int main()
{
  int fd, fd2;
  pipebuf_t data;
  pipebuf_t *ptr = &data;
  mkfifo("myfifo", 0666);
  fd = open("myfifo", O_WRONLY);
  mkfifo("myfifo2", 0666);
  fd2 = open("myfifo2", O_RDONLY);
  int item = 1;
  ptr->v = 0;
  ptr->a = 0;

  while (1)
  {
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
        write(fd, ptr, sizeof(pipebuf_t));
      }
    }
    else
    {
      read(fd2, ptr, sizeof(pipebuf_t));
    }
    sleep(1);
  }
}
