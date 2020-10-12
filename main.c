// PingPongOS - P4
// Aluno: Francisco S Miamoto - 1450522

#include "ppos.h"
#include "tiva_core.h"

task_t Pang, Peng, Ping, Pong, Pung;

void Body(void *arg) {
  int i;

  UARTprintf("%s: inicio (prioridade %d)\n", (char *)arg, task_getprio(NULL));

  for (i = 0; i < 10; i++) {
    UARTprintf("%s: %d\n", (char *)arg, i);
    task_yield();
  }
  UARTprintf("%s: fim\n", (char *)arg);
  task_exit(0);
}

int main(void) {
  TivaInit();

  UARTprintf("PingPongOS\n");

  UARTprintf("main: inicio\n");

  ppos_init();

  task_create(&Pang, Body, "    Pang");
  task_setprio(&Pang, 0);

  task_create(&Peng, Body, "        Peng");
  task_setprio(&Peng, 2);

  task_create(&Ping, Body, "            Ping");
  task_setprio(&Ping, 4);

  task_create(&Pong, Body, "                Pong");
  task_setprio(&Pong, 6);

  task_create(&Pung, Body, "                    Pung");
  task_setprio(&Pung, 8);

  task_yield();

  UARTprintf("main: fim\n");
}
