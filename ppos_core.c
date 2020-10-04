#include "internal.h"
#include "ppos_data.h"

#define STACKSIZE 4096
#define perror UARTprintf

#define getcontext get_context_asm
#define swapcontext swap_context_asm

int next_task_id = 1;  // IDs for other tasks start at 1
task_t main_task;
task_t *current_task;

void ppos_init() {
  main_task.id = 0;
  getcontext(&(main_task.context));
  main_task.next = NULL;
  main_task.prev = NULL;
  current_task = &main_task;
}

int task_create(task_t *task, void (*start_routine)(void *), void *arg) {
  current_task->next = task;

  getcontext(&(task->context));

  char *stack = malloc(STACKSIZE);
  if (stack) {
    task->context.uc_stack.ss_sp = stack;
    task->context.uc_stack.ss_size = STACKSIZE;
    task->context.uc_stack.ss_flags = 0;
    task->context.uc_link = 0;
  } else {
    perror("task_create: error on stack allocation\n");
    return -1;
  }

  makecontext(&(task->context), (int)start_routine, 1, arg);

  task->id = next_task_id++;
  task->prev = current_task;

#ifdef DEBUG
  UARTprintf("task_create: created task %d\n", task->id);
#endif

  return task->id;
}

int task_switch(task_t *task) {
  task_t *previous = current_task;
  current_task = task;

  int status = swapcontext(&(previous->context), &(task->context));
  if (status < 0) {
    perror("task_switch: error on swapcontext call\n");
    return status;
  }

#ifdef DEBUG
  UARTprintf("task_switch: changing context %d -> %d\n", previous->id,
             task->id);
#endif

  return 0;
}

void task_exit(int exit_code) { task_switch(&main_task); }

int task_id() { return current_task->id; }
