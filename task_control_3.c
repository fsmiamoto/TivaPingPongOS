// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DINF UFPR
// Versão 1.1 -- Julho de 2016

// Teste da gestão básica de tarefas

#include <stdio.h>
#include <stdlib.h>

#include "ppos.h"

#define MAXTASK3 10

task_t task;

// corpo das threads
void BodyTask3(void *arg) {
  UARTprintf("Estou na tarefa %5d\n", task_id());
  task_exit(0);
}

void teste_task_control_3() {
  int i;

  UARTprintf("Teste 3: inicio\n");

  ppos_init();

  // cria MAXTASK tarefas, ativando cada uma apos sua criacao
  for (i = 0; i < MAXTASK3; i++) {
    task_create(&task, BodyTask3, NULL);
    task_switch(&task);
  }

  UARTprintf("Teste 3: fim\n");
}
