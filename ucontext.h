#ifndef __UCONTEXT__
#define __UCONTEXT__

typedef struct stack_t {
  char *ss_sp;
  long ss_size;
  int ss_flags;
} stack_t;

typedef struct mcontext_t {
  int regPC;    // program counter
  int regSP;    // stack pointer
  int regCPSR;  // status register
  int regR0;
  int regR1;
  int regR2;
  int regR3;
  int regR4;
  int regR5;
  int regR6;
  int regR7;
  int regR8;
  int regR9;
  int regR10;
  int regR11;
  int regR12;
  int regR13;
  int regR14;
  int regR15;
} mcontext_t;

typedef struct ucontext_t {
  struct ucontext_t *uc_link;
  stack_t uc_stack;
  mcontext_t uc_mcontext;
  int func;
  int initialized;
} ucontext_t;

extern int get_context_asm(ucontext_t *);
extern int set_context_asm(ucontext_t *);
extern int swap_context_asm(ucontext_t *, ucontext_t *);

int getcontext(ucontext_t *);
int setcontext(ucontext_t *);
int swapcontext(ucontext_t *, ucontext_t *);
void makecontext(ucontext_t *context, int bodyAddress, int n_params,
                 char *param);

void setgearclock(int);

#endif
