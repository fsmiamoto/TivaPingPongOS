#include "ppos_internal.h"

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
