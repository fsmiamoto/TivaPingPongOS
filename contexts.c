#include <stdio.h>
#include <stdlib.h>

#include "ucontext.h"

#define STACKSIZE 4096 /* tamanho de pilha das threads */

ucontext_t ContextPing, ContextPong, ContextMain;

void BodyPing(void *arg) {
  int i;

  UARTprintf("PING  %s iniciada\n", (char *)arg);

  for (i = 0; i < 6; i++) {
    UARTprintf("PING  %s %d\n", (char *)arg, i);
    swap_context_asm(&ContextPing, &ContextPong);
  }
  UARTprintf("%s FIM\n", (char *)arg);

  swap_context_asm(&ContextPing, &ContextMain);
}

void BodyPong(void *arg) {
  int i;

  UARTprintf("PONG  %s iniciada\n", (char *)arg);

  for (i = 0; i < 6; i++) {
    UARTprintf("PONG  %s %d\n", (char *)arg, i);
    swap_context_asm(&ContextPong, &ContextPing);
  }
  UARTprintf("%s FIM\n", (char *)arg);

  swap_context_asm(&ContextPong, &ContextMain);
}

void teste1(void) {
  char *stack;

  printf("Main INICIO\n");

  get_context_asm(&ContextPing);

  stack = malloc(10);
  if (stack == NULL) {
    perror("Erro na criacao da pilha: ");
  }

  ContextPing.uc_stack.ss_sp = stack;
  ContextPing.uc_stack.ss_size = STACKSIZE;
  ContextPing.uc_stack.ss_flags = 0;
  ContextPing.uc_link = 0;

  makecontext(&ContextPing, (int)(*BodyPing), 1, "    Ping");

  get_context_asm(&ContextPong);

  stack = malloc(STACKSIZE);
  if (stack == NULL) {
    perror("Erro na criacao da pilha: ");
  }

  ContextPong.uc_stack.ss_sp = stack;
  ContextPong.uc_stack.ss_size = STACKSIZE;
  ContextPong.uc_stack.ss_flags = 0;
  ContextPong.uc_link = 0;

  makecontext(&ContextPong, (int)(*BodyPong), 1, "        Pong");

  swap_context_asm(&ContextMain, &ContextPing);
  swap_context_asm(&ContextMain, &ContextPong);

  printf("Main FIM\n");

  return;
}
