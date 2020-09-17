// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DINF UFPR
// Versão 1.1 -- Julho de 2016

// Teste da gestão básica de tarefas

#include <stdio.h>
#include <stdlib.h>

#include "ppos.h"

task_t Ping, Pong;

// corpo da thread Ping
void BodyPing1(void *arg) {
  int i;
  UARTprintf("Ping: inicio\n");
  for (i = 0; i < 4; i++) {
    UARTprintf("Ping: %d\n", i);
    task_switch(&Pong);
  }
  UARTprintf("Ping: fim\n");
  task_exit(0);
}

// corpo da thread Pong
void BodyPong1(void *arg) {
  int i;
  UARTprintf("Pong: inicio\n");
  for (i = 0; i < 4; i++) {
    UARTprintf("Pong: %d\n", i);
    task_switch(&Ping);
  }
  UARTprintf("Pong: fim\n");
  task_exit(0);
}

void teste_task_control_1() {
  UARTprintf("Teste 1: inicio\n");

  ppos_init();

  task_create(&Ping, BodyPing1, "    Ping");
  task_create(&Pong, BodyPong1, "        Pong");

  task_switch(&Ping);
  task_switch(&Pong);

  UARTprintf("Teste 1: fim\n\n");
}
