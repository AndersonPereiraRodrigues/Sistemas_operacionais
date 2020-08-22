/*  
  Universidade Tecnológica Federal do Paraná UTFPR - Campus Pato Branco
  Disciplina: Sistemas Operacionais
  Trabalho 3 - THREADS
  Sequences:Nilakantha's Series - Double Precision
  ###############################################################################
  #                                  Times                                      #
  # 1- 4.978s - 3.14159265358979322788762766904824275115970522165298            #
  # 2- 2.598s - 3.14159265358979322788762766904824275115970522165298            #
  # 4- 1.652s - 3.14159265358979322788762766904824275115970522165298            #
  # 8- 1.424s - 3.14159265358979322788762766904824275115970522165298            #
  # 16-1.401s - 3.14159265358979322788762766904824275115970522165298            #
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
  long double s = 1;
  for (double i = (inicio + 2); i <= fim; i += 2)
  {
    parc[8 * tid] = parc[8 * tid] + s * (4 / (i * (i + 1) * (i + 2)));
    s = -s;
  }
}

void main()
{
  pthread_t thead[NUM_THREAD];
  long double result = 3;
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
  printf("Final: %.50Lf\n", result);
  pthread_exit(0);
}