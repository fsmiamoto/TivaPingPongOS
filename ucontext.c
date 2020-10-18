// #pragma ARM
#include "ucontext.h"

extern int flag;
extern int memPC;
extern int set_context_asm(ucontext_t *);
extern int get_context_asm(ucontext_t *);

void makecontext(ucontext_t *context, int start_routine, int n_parms,
                 char *param) {
  context->func = start_routine;
  context->uc_mcontext.regR0 = (int)param;
}

int swapcontext(ucontext_t *saida, ucontext_t *entrada) {
  get_context_asm(saida);
  set_context_asm(entrada);
  return 0;
}
