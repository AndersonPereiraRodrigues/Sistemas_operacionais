/*
  ver processos: ps -la
  matar processos: kill -SIGKILL "pid"
                   kill -SIGINT "pid"
  para o processo: kill -SIGSTOP "pid"
  -> kill -SIGINT "pid" é tambem gerado pelo Ctrl+C
  -> kill -SIGTSTP "pid" é tambem gerado pelo Ctrl+Z
  Mudando função do sinal: signal(_,_);
    -> 1 parametro
        -SIGINT
        -SIGTSTP
    -> 2 parametro
        - SIG_IGN : iguinora o sinal
        - SIG_DFL : restaura função padrão
        - nomeDeFuncao : vai a função desejada
*/
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
int cont = 0;
char status = 'f';
void mostrar()
{
  printf("PRECIONADO (Ctrl+C): %d\n", cont);
}
void contar()
{
  cont++;
  if (cont >= 10)
  {
    printf("Really exit (Y/n)?\n");
    status = 'v';
  }
}
void fim()
{
  printf("Bye-bye\n");
  kill(getpid(), SIGKILL);
}
int main(void)
{

  signal(SIGINT, contar);
  signal(SIGTSTP, mostrar);
  char a;
  for (;;)
  {
    if (status == 'f')
    {
      pid_t pid = getpid();
      printf("PID: %ld\n", (long)pid);
      printf("Aguardando comando Ctrl+C || Ctrl+Z\n");
      sleep(10);
    }
    else
    {
      signal(SIGALRM, fim);
      alarm(5);
      a = getchar();
      if (a == 'Y' || a == 'y')
      {
        kill(getpid(), SIGKILL);
      }
      else if (a == 'n' || a == 'N')
      {
        cont = 0;
        status = 'f';
        signal(SIGALRM, SIG_IGN);
      }
    }
  }
  return 0;
}