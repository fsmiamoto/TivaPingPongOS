#ifndef __UCONTEXT__
#define __UCONTEXT__

typedef struct stack_t {
  char *ss_sp;
  long ss_size;
  int ss_flags;
} stack_t;

typedef struct mcontext_t {
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
  int regSP;
  int regLR;
  int regPC;
  int regXPSR;
} mcontext_t;

typedef struct ucontext_t {
  struct ucontext_t *uc_link;
  stack_t uc_stack;
  mcontext_t uc_mcontext;
  int func;
  int initialized;
  char* arg;
} ucontext_t;

extern int getcontext(ucontext_t *);
extern int get_context_asm(ucontext_t *);

extern int setcontext(ucontext_t *);
extern int set_context_asm(ucontext_t *);

extern void makecontext(ucontext_t *context, int bodyAdress, int n_parms,
                        char *parm);

extern void setgearclock(int);

extern int swapcontext(ucontext_t *, ucontext_t *);
extern int swap_context_asm(ucontext_t *, ucontext_t *);

#endif
