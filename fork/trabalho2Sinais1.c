/*  
  Universidade Tecnológica Federal do Paraná UTFPR - Campus Pato Branco
  Disciplina: Sistemas Operacionais
  Departamento: DAINF 
  Trabalho 2.1 - SINAIS
*/
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
int cont = 0;
char status = 'f';
void VER_handler()
{
  printf("PRECIONADO (Ctrl+C): %d\n", cont);
}
void CONT_handler()
{
  cont++;
  if (cont >= 10)
  {
    printf("Really exit (Y/n)?\n");
    status = 'v';
  }
}
void EXIT_handler()
{
  printf("Bye-bye\n");
  kill(getpid(), SIGKILL);
}
int main(void)
{

  signal(SIGINT, CONT_handler);
  signal(SIGTSTP, VER_handler);
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
      signal(SIGALRM, EXIT_handler);
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