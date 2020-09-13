// #pragma ARM
#include "ucontext.h"
extern int flag;
extern int memPC;
extern set_context_asm(ucontext_t*);
extern get_context_asm(ucontext_t*);

void makecontext(ucontext_t* context, int start_routine, int n_parms,
                 char* parm) {
  context->func = start_routine;
}
