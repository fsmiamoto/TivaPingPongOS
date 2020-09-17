#include "ppos.h"
#define MAXTASK 10

int memPC = 0;
int flag;

task_t task[MAXTASK + 1];

// corpo das threads
void BodyTask(void *arg) {
  int next;

  UARTprintf("Iniciei  tarefa %5d\n", task_id());

  // passa o controle para a proxima tarefa
  next = (task_id() < MAXTASK) ? task_id() + 1 : 1;
  task_switch(&task[next]);

  UARTprintf("Encerrei tarefa %5d\n", task_id());

  task_exit(0);
}

void teste_task_control_2() {
  int i;

  UARTprintf("main: inicio\n");

  ppos_init();

  for (i = 1; i <= MAXTASK; i++) task_create(&task[i], BodyTask, NULL);

  for (i = 1; i <= MAXTASK; i++) task_switch(&task[i]);

  UARTprintf("main: fim\n");
}
