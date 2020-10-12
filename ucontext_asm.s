;.syntax unified
;.arch armv7-m

.text

P_ucontext_t           .set   0
stack_t_Pss_sp         .set   1 * 4
stack_t_ss_size        .set   2 * 4
stack_t_ss_flags       .set   3 * 4
mcontext_t_regR0       .set   4 * 4
mcontext_t_regR1       .set   5 * 4
mcontext_t_regR2       .set   6 * 4
mcontext_t_regR3       .set   7 * 4
mcontext_t_regR4       .set   8 * 4
mcontext_t_regR5       .set   9 * 4
mcontext_t_regR6       .set  10 * 4
mcontext_t_regR7       .set  11 * 4
mcontext_t_regR8       .set  12 * 4
mcontext_t_regR9       .set  13 * 4
mcontext_t_regR10      .set  14 * 4
mcontext_t_regR11      .set  15 * 4
mcontext_t_regR12      .set  16 * 4
mcontext_t_regSP       .set  17 * 4
mcontext_t_regLR       .set  18 * 4
mcontext_t_regPC       .set  19 * 4
mcontext_t_regXPSR     .set  20 * 4
ucontext_t_func        .set  21 * 4
ucontext_t_initialized .set  22 * 4
ucontext_t_arg         .set  23 * 4

   .global   get_context_asm
get_context_asm:
	str.w    r0,  [r0, #mcontext_t_regR0]
	str.w    r1,  [r0, #mcontext_t_regR1]
	str.w    r2,  [r0, #mcontext_t_regR2]
	str.w    r3,  [r0, #mcontext_t_regR3]
	str.w    r4,  [r0, #mcontext_t_regR4]
	str.w    r5,  [r0, #mcontext_t_regR5]
	str.w    r6,  [r0, #mcontext_t_regR6]
	str.w    r7,  [r0, #mcontext_t_regR7]
	str.w    r8,  [r0, #mcontext_t_regR8]
	str.w    r9,  [r0, #mcontext_t_regR9]
	str.w    r10, [r0, #mcontext_t_regR10]
	str.w    r11, [r0, #mcontext_t_regR11]
	str.w    r12, [r0, #mcontext_t_regR12]
	push     {r4}
    mov      r4,  r13
	str.w    r4,  [r0, #mcontext_t_regSP]
    mov      r4,  r14
	str.w    r4,  [r0, #mcontext_t_regLR]
	mov      r4,  r15
	str.w    r4,  [r0, #mcontext_t_regPC]
	mrs      r4,  xpsr
	str.w    r4,  [r0, #mcontext_t_regXPSR]
	pop      {r4}
    bx       lr 

    .global    swap_context_asm
swap_context_asm:
    ; Save the context of the current task
	str.w    r0,  [r0, #mcontext_t_regR0]
	str.w    r1,  [r0, #mcontext_t_regR1]
	str.w    r2,  [r0, #mcontext_t_regR2]
	str.w    r3,  [r0, #mcontext_t_regR3]
	str.w    r4,  [r0, #mcontext_t_regR4]
	str.w    r5,  [r0, #mcontext_t_regR5]
	str.w    r6,  [r0, #mcontext_t_regR6]
	str.w    r7,  [r0, #mcontext_t_regR7]
	str.w    r8,  [r0, #mcontext_t_regR8]
	str.w    r9,  [r0, #mcontext_t_regR9]
	str.w    r10, [r0, #mcontext_t_regR10]
	str.w    r11, [r0, #mcontext_t_regR11]
	str.w    r12, [r0, #mcontext_t_regR12]
	push     {r4}
    mov      r4,  sp
	str.w    r4,  [r0, #mcontext_t_regSP]
    mov      r4,  lr
	str.w    r4,  [r0, #mcontext_t_regLR]
	mov      r4,  PC
	str.w    r4,  [r0, #mcontext_t_regPC]
	mrs      r4,  xpsr
	str.w    r4,  [r0, #mcontext_t_regXPSR]
	pop      {r4}

    ; Load the context
    ldr      r2,   [r1, #mcontext_t_regR2]
	ldr      r3,   [r1, #mcontext_t_regR3]
    ldr      r4 ,  [r1, #mcontext_t_regR4]
    ldr      r5 ,  [r1, #mcontext_t_regR5]
    ldr      r6 ,  [r1, #mcontext_t_regR6]
    ldr      r7 ,  [r1, #mcontext_t_regR7]
    ldr      r8 ,  [r1, #mcontext_t_regR8]
    ldr      r9 ,  [r1, #mcontext_t_regR9]
    ldr      r10,  [r1, #mcontext_t_regR10]
    ldr      r11,  [r1, #mcontext_t_regR11]
    ldr      r12,  [r1, #mcontext_t_regR12]

    ; Check if the context is initialized
	ldr	     r0 ,  [r1 ,#ucontext_t_initialized]
	cmp      r0 ,  #0
	bne      initialized
	; Initialize the context
    mov      r0,   #1
	str      r0,   [r1, #ucontext_t_initialized]
    ldr      r0 ,  [r1, #ucontext_t_arg]
    ldr      pc,   [r1, #ucontext_t_func]
    ldr      sp,   [r1, #stack_t_Pss_sp]
	b        return
initialized:
	ldr      pc,   [r1, #mcontext_t_regPC]
	ldr      sp,   [r1, #mcontext_t_regSP]
return:
    bx       lr
