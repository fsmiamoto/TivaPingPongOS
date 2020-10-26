// PingPongOS - P4
// Aluno: Francisco S Miamoto - 1450522

#include "ppos.h"
#include "tiva_core.h"

#define WORKLOAD 4000

task_t Pang, Peng, Ping, Pong, Pung;

// corpo das threads
void Body(void *arg) {
  int i;
  UARTprintf("%s: inicio em %4d ticks\n", (char *)arg, systime());
  for (i = 0; i < WORKLOAD; i++) {
    UARTprintf("%s: %d\n", (char *)arg, i);
  }
  UARTprintf("%s: fim em %d ticks\n", (char *)arg, systime());
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
