#include <stdio.h>
#include <stdlib.h>

#include "ucontext.h"

#define STACKSIZE 512

ucontext_t ContextPing, ContextPong, ContextMain;

void BodyPing(void *arg) {
  swap_context_asm(&ContextPing, &ContextPong);
  swap_context_asm(&ContextPing, &ContextMain);
}

void BodyPong(void *arg) {
  swap_context_asm(&ContextPong, &ContextPing);
  swap_context_asm(&ContextPong, &ContextMain);
}

void contexts(void) {
  char *stack;

  printf("Main INICIO\n");

  get_context_asm(&ContextPing);

  stack = (char *)malloc(STACKSIZE);
  if (stack == NULL) {
    UARTprintf("Erro na criacao da pilha\n");
  }

  ContextPing.uc_stack.ss_sp = stack;
  ContextPing.uc_stack.ss_size = STACKSIZE;
  ContextPing.uc_stack.ss_flags = 0;
  ContextPing.uc_link = 0;

  makecontext(&ContextPing, (int)(*BodyPing), 1, "    Ping");

  get_context_asm(&ContextPong);

  stack = (char *)malloc(STACKSIZE);
  if (stack == NULL) {
    UARTprintf("Erro na criacao da pilha\n");
  }

  ContextPong.uc_stack.ss_sp = stack;
  ContextPong.uc_stack.ss_size = STACKSIZE;
  ContextPong.uc_stack.ss_flags = 0;
  ContextPong.uc_link = 0;

  makecontext(&ContextPong, (int)(*BodyPong), 1, "        Pong");

  swap_context_asm(&ContextMain, &ContextPing);
  swap_context_asm(&ContextMain, &ContextPong);
}
