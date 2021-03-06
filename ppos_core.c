#include "ppos.h"
#include "ppos_data.h"
#include "ppos_internal.h"

unsigned int next_task_id = 1;  // IDs for other tasks start at 1
unsigned int system_tick_count = 0;

task_t main_task;
task_t dispatcher_task;
task_t *current_task;

// Task queues for each state:
// Created, Ready, Running, Waiting, Terminated
task_t *queues[] = {NULL, NULL, NULL, NULL, NULL};

void ppos_init() {
  task_create(&main_task, 0, NULL);
  main_task.context.initialized = 1;
  main_task.id = 0;
  main_task.next = NULL;
  main_task.prev = NULL;
  main_task.prio = 0;
  main_task.prio_d = 0;
  main_task.preemptible = 1;
  main_task.activations = 0;
  main_task.start_tick = systime();
  main_task.state = READY;

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
  task->preemptible = 1;

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
      "Task %d exit: execution time  %4d ticks, processor time  %4d ticks, %4d "
      "activations\n",
      current_task->id, systime() - current_task->start_tick,
      current_task->tick_count, current_task->activations);

  if (current_task == &dispatcher_task) {
    task_switch(&main_task);
    return;
  }

  current_task->state = TERMINATED;
  current_task->exit_code = exit_code;

  // Wakeup the waiting tasks
  while (queue_size((queue_t *)current_task->waiting) > 0) {
    queue_t *task = queue_remove((queue_t **)&current_task->waiting,
                                 (queue_t *)current_task->waiting);
    queue_append((queue_t **)&queues[READY], task);
  }

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

int task_join(task_t *task) {
  if (task->state == TERMINATED) return task->exit_code;
  current_task->state = WAITING;
  queue_append((queue_t **)&task->waiting, (queue_t *)current_task);
  task_switch(&dispatcher_task);
  return task->exit_code;
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
    next->tick_budget = DEFAULT_TICK_BUDGET;
    next->activations += 1;

    task_switch(next);

    if (next->next == NULL && next->prev == NULL)
      queue_append((queue_t **)&queues[next->state], (queue_t *)next);
  }

  task_exit(0);
}
