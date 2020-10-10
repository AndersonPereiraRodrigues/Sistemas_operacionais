/*
  Universidade Tecnológica Federal do Paraná UTFPR - Campus Pato Branco
  Disciplina: Sistemas Operacionais
  Trabalho 1 - FORK WAIT
  #######################
  # Arvore implementada #
  #         A           #
  #          \          #
  #           N         #
  #          / \        #  
  #         D   R       #
  #          \  /\      #
  #           E O S     #
  #######################
*/
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
  pid_t pid;
  int status;
  printf("proc-A, pid %d, ppid %d, acaba de ser criado\n", getpid(), getppid());
  pid = fork();
  if (pid == 0)
  {
    printf("proc-N, pid %d, ppid %d, acaba de ser criado\n", getpid(), getppid());
    pid = fork();
    if (pid == 0)
    {
      printf("proc-R, pid %d, ppid %d, acaba de ser criado\n", getpid(), getppid());
      pid = fork();
      if (pid == 0)
      {
        printf("proc-O, pid %d, ppid %d, acaba de ser criado\n", getpid(), getppid());
        printf("proc-O, pid %d, ppid %d, morreu\n", getpid(), getppid());
      }
      else
      {
        wait(pid, &status, 0);
        printf("proc-S, pid %d, ppid %d, acaba de ser criado\n", getpid(), getppid());
        pid = fork();
        if (pid == 0)
        {
          printf("proc-S, pid %d, ppid %d, morreu\n", getpid(), getppid());
        }
        else
        {
          wait(pid, &status, 0);
          printf("proc-R, pid %d, ppid %d, morreu\n", getpid(), getppid());
          exit(0);
        }
        exit(0);
      }
    }
    else
    {
      wait(pid, &status, 0);
      pid = fork();
      if (pid == 0)
      {
        printf("proc-D, pid %d, ppid %d, acaba de ser criado\n", getpid(), getppid());
        pid = fork();
        if (pid == 0)
        {
          printf("proc-E, pid %d, ppid %d, acaba de ser criado\n", getpid(), getppid());
          printf("proc-E, pid %d, ppid %d, morreu\n", getpid(), getppid());
        }
        else
        {
          wait(pid, &status, 0);
          printf("proc-D, pid %d, ppid %d, morreu\n", getpid(), getppid());
          exit(0);
        }
      }
      else
      {
        wait(pid, &status, 0);
        printf("proc-N, pid %d, ppid %d, morreu\n", getpid(), getppid());
        exit(0);
      }
      exit(0);
    }
  }
  else
  {
    wait(pid, &status, 0);
    printf("proc-A, pid %d, morreu\n", getpid());
  }
  return 0;
}