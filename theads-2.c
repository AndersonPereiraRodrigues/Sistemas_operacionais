/*  
  Universidade Tecnológica Federal do Paraná UTFPR - Campus Pato Branco
  Disciplina: Sistemas Operacionais
  Trabalho 3 - THREADS
  Sequences:Série de Leibniz
  ###############################################################################
  #                                  Times                                      #
  # 1- 58.653s - 3.14159265258979539269836023240856093252659775316715           #
  # 2- 30.293s - 3.14159265258979414760058535005526891836780123412609           #
  # 4- 16.852s - 3.14159265258979510907107191020060099617694504559040           #
  # 8- 16.444s - 3.14159265258979446332025797783416010133805684745312           #
  # 16-16.302s - 3.14159265258979388825942569152260830378509126603603           #
  ###############################################################################
*/
#include <signal.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#define NUM_THREAD 16
#define BILHAO 1000000000

long double parc[8 * NUM_THREAD];

void *calc(void *param)
{
  long int tid = (long)param;
  unsigned long int termos = BILHAO / NUM_THREAD;
  unsigned long int inicio = tid * termos;
  unsigned long int fim = tid * termos + termos;
  for (int i = inicio; i < fim; i++)
  {
    parc[8 * tid] += (pow(-1, i) / (2 * i + 1));
  }
}

void main()
{
  pthread_t thead[NUM_THREAD];
  long double result = 0;
  for (int i = 0; i < NUM_THREAD; i++)
  {
    parc[8 * i] = 0;
  }
  for (int i = 0; i < NUM_THREAD; i++)
  {
    pthread_create(&thead[i], NULL, calc, (void *)(intptr_t)i);
  }
  for (int i = 0; i < NUM_THREAD; i++)
  {
    pthread_join(thead[i], NULL);
    printf("%d Final: %.50Lf\n", i, parc[8 * i]);
    result += parc[8 * i];
  }
  printf("Final: %.50Lf\n", (result * 4));
  pthread_exit(0);
}