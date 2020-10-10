//gcc -o write write.c -lrt -lpthread
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

int insert_item(int val, messbuf_t *da)
{
  da->v += val;
  return 0;
}

int main()
{
  mqd_t fd, fd2;
  struct mq_attr attr;
  messbuf_t data;
  messbuf_t *ptr = &data;

  attr.mq_maxmsg = 1;
  attr.mq_msgsize = sizeof(messbuf_t);
  attr.mq_flags = 0;

  mq_unlink("/yuo");
  fd = mq_open("/yuo", O_RDWR | O_CREAT, 0666, &attr);

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

    sleep(1);
  }
}