// #pragma ARM
#include "ucontext.h"
extern int flag;
extern int memPC;
extern set_context_asm(ucontext_t*);
extern get_context_asm (ucontext_t*);
//int r;
//int r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, r13, r14, r15;

//int t1 = 4;


void makecontext(ucontext_t *context, int start_routine, int n_parms, char* parm)
{
context->func = start_routine;
}
