// PingPongOS - Operador Join - P8
// Aluno: Francisco S Miamoto - 1450522

#include "ppos.h"
#include "tiva_core.h"

#define WORKLOAD 500

task_t Pang, Peng, Ping, Pong, Pung;

void Body(void *arg) {
  int i;
  UARTprintf("%s: inicio em %4d ticks\n", (char *)arg, systime());
  for (i = 0; i < task_id() * WORKLOAD; i++) {
    UARTprintf("%s: %d\n", (char *)arg, i);
  }
  UARTprintf("%s: fim em %d ticks\n", (char *)arg, systime());

  task_exit(task_id());
}

int main() {
  int i, ec;

  TivaInit();
  UARTprintf("PingPongOS\n");

  ppos_init();

  UARTprintf("main: inicio\n");

  task_create(&Pang, Body, "    Pang");
  task_create(&Peng, Body, "        Peng");
  task_create(&Ping, Body, "            Ping");
  task_create(&Pong, Body, "                Pong");
  task_create(&Pung, Body, "                    Pung");

  for (i = 0; i < WORKLOAD; i++) {
    UARTprintf("main: %d\n", i);
  }

  UARTprintf("main: esperando Pang...\n");
  ec = task_join(&Pang);
  UARTprintf("main: Pang acabou com exit code %d\n", ec);

  UARTprintf("main: esperando Peng...\n");
  ec = task_join(&Peng);
  UARTprintf("main: Peng acabou com exit code %d\n", ec);

  UARTprintf("main: esperando Ping...\n");
  ec = task_join(&Ping);
  UARTprintf("main: Ping acabou com exit code %d\n", ec);

  UARTprintf("main: esperando Pong...\n");
  ec = task_join(&Pong);
  UARTprintf("main: Pong acabou com exit code %d\n", ec);

  UARTprintf("main: esperando Pung...\n");
  ec = task_join(&Pung);
  UARTprintf("main: Pung acabou com exit code %d\n", ec);

  UARTprintf("main: fim\n");

  task_exit(0);
}
