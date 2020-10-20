#include "ppos.h"
#include "ppos_data.h"

#define STACKSIZE 512
#define SCHEDULER_AGING_ALPHA 1

#define perror UARTprintf
#define printf UARTprintf
#define getcontext get_context_asm
#define swapcontext swap_context_asm

// Internal functions
void *__highest_prio_task(void *prev, void *next);
void __apply_aging(void *ptr);

unsigned int next_task_id = 1;  // IDs for other tasks start at 1
unsigned int system_tick_count = 0;

task_t main_task;
task_t dispatcher_task;
task_t *current_task;

// Task queues for each state:
// Created, Ready, Running, Waiting, Terminated
task_t *queues[] = {NULL, NULL, NULL, NULL, NULL};

// Internal functions
void *__highest_prio_task(void *prev, void *next);
void __apply_aging(void *ptr);

task_t *scheduler() {
  if (queue_size((queue_t *)queues[READY]) == 0) {
    return NULL;
  }

  task_t *chosen = (task_t *)queue_reduce((queue_t *)queues[READY], NULL,
                                          __highest_prio_task);

  queue_foreach((queue_t *)queues[READY], __apply_aging);

  chosen->prio_d = chosen->prio;

  return chosen;
}

void dispatcher() {
  while (1) {
    dispatcher_task.activations++;

    task_t *next = scheduler();
    if (next == NULL) {
      break;
    }

#ifdef DEBUG
    printf("dispatcher: dispatching task %d\n", next->id);
#endif
    queue_remove((queue_t **)&queues[READY], (queue_t *)next);

    next->state = RUNNING;
    next->tick_budget = 20;
    next->activations += 1;

    task_switch(next);

    queue_append((queue_t **)&queues[next->state], (queue_t *)next);
  }

  task_exit(0);
}

void ppos_init() {
  task_create(&main_task, 0, NULL);
  main_task.context.initialized = 1;

  current_task = &main_task;

  task_create(&dispatcher_task, (void *)dispatcher, NULL);
}

int task_create(task_t *task, void (*start_routine)(void *), void *arg) {
  getcontext(&(task->context));

  char *stack = (char *)malloc(STACKSIZE);
  if (stack) {
    task->context.uc_stack.ss_sp = stack;
    task->context.uc_stack.ss_size = STACKSIZE;
    task->context.uc_stack.ss_flags = 0;
    task->context.uc_link = 0;
  } else {
    perror("task_create: error on stack allocation");
    return -1;
  }

  makecontext(&(task->context), (void *)start_routine, 1, arg);

  task->id = next_task_id++;
  task->start_tick = systime();
  task->activations = 0;
  task->prev = NULL;
  task->next = NULL;
  task->state = CREATED;
  task->prio = 0;
  task->prio_d = 0;
  task->is_system_task = 0;

#ifdef DEBUG
  printf("task_create: created task %d\n", task->id);
#endif

  if (task != &dispatcher_task && task != &main_task) {
    task->state = READY;
    queue_append((queue_t **)&queues[READY], (queue_t *)task);
  }

  return task->id;
}

int task_switch(task_t *task) {
  task_t *previous = current_task;
  current_task = task;

#ifdef DEBUG
  printf("task_switch: changing context %d -> %d\n", previous->id, task->id);
#endif

  swapcontext(&(previous->context), &(task->context));

  return 0;
}

void task_exit(int exit_code) {
#ifdef DEBUG
  printf("task_exit: exiting task %d\n", current_task->id);
#endif

  printf(
      "Task %d exit: execution time  %4d ms, processor time  %4d ms, %4d "
      "activations\n",
      current_task->id, systime() - current_task->start_tick,
      current_task->tick_count, current_task->activations);

  if (current_task == &dispatcher_task) {
    task_switch(&main_task);
    return;
  }

  current_task->state = TERMINATED;
  task_switch(&dispatcher_task);
}

int task_id() { return current_task->id; }

void task_yield() {
#ifdef DEBUG
  printf("task_yield: called from task %d\n", current_task->id);
#endif

  current_task->state = READY;
  task_switch(&dispatcher_task);
}

void task_setprio(task_t *task, int prio) {
  if (prio > 19 || prio < -20)
    printf("task_setprio: invalid priority, must be between -20 and 19");

  if (task == NULL) {
    task = current_task;
  }

  task->prio = (short)prio;
  task->prio_d = (short)prio;
}

int task_getprio(task_t *task) {
  if (task == NULL) {
    task = current_task;
  }

  return (int)task->prio;
}

unsigned int systime() { return system_tick_count; }

// Return the task with the highest priority
void *__highest_prio_task(void *prev, void *next) {
  if (prev == NULL) return next;

  task_t *prev_task = (task_t *)prev;
  task_t *next_task = (task_t *)next;

  if (next_task->prio_d < prev_task->prio_d) {
    return next;
  }

  return prev_task;
}

// Apply the aging factor on tasks
void __apply_aging(void *ptr) {
  task_t *task = (task_t *)ptr;
  task->prio_d -= SCHEDULER_AGING_ALPHA;
}
