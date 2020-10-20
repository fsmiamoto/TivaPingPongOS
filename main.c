// PingPongOS - P4
// Aluno: Francisco S Miamoto - 1450522

#include "ppos.h"
#include "tiva_core.h"

#define WORKLOAD 4000

task_t Pang, Peng, Ping, Pong, Pung;

// Simula um processamento pesado
int hardwork(int n) {
  int i, j, soma;

  soma = 0;
  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++) soma += j;
  return (soma);
}

// corpo das threads
void Body(void *arg) {
  UARTprintf("%s: inicio em %4d ms\n", (char *)arg, systime());
  hardwork(WORKLOAD);
  UARTprintf("%s: fim    em %4d ms\n", (char *)arg, systime());
  task_exit(0);
}

int main() {
  TivaInit();
  UARTprintf("PingPongOS\n");

  UARTprintf("main: inicio\n");

  ppos_init();

  task_create(&Pang, Body, "    Pang");
  task_create(&Peng, Body, "        Peng");
  task_create(&Ping, Body, "            Ping");
  task_create(&Pong, Body, "                Pong");
  task_create(&Pung, Body, "                    Pung");

  task_yield();

  UARTprintf("main: fim\n");
}
