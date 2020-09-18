;.syntax unified
;.arch armv7-m

.text

P_ucontext_t           .set  0
stack_t_Pss_sp         .set  1 * 4
stack_t_ss_size        .set  2 * 4
stack_t_ss_flags       .set  3 * 4
mcontext_t_regPC       .set  4 * 4  ;  program counter
mcontext_t_regSP       .set  5 * 4  ;  stack pointer
mcontext_t_regCPSR     .set  6 * 4  ;  status register
mcontext_t_regR0       .set  7 * 4
mcontext_t_regR1       .set  8 * 4
mcontext_t_regR2       .set  9 * 4
mcontext_t_regR3       .set 10 * 4
mcontext_t_regR4       .set 11 * 4
mcontext_t_regR5       .set 12 * 4
mcontext_t_regR6       .set 13 * 4
mcontext_t_regR7       .set 14 * 4
mcontext_t_regR8       .set 15 * 4
mcontext_t_regR9       .set 16 * 4
mcontext_t_regR10      .set 17 * 4
mcontext_t_regR11      .set 18 * 4
mcontext_t_regR12      .set 19 * 4
mcontext_t_regR13      .set 20 * 4
mcontext_t_regR14      .set 21 * 4
mcontext_t_regR15      .set 22 * 4
ucontext_t_func        .set 23 * 4
ucontext_t_initialized .set 24 * 4


   .global   get_context_asm
get_context_asm:
	str.w       r0,  [r0,#mcontext_t_regR0]
	str.w       r1,  [r0,#mcontext_t_regR1]
	str.w       r2,  [r0,#mcontext_t_regR2]
	str.w       r3,  [r0,#mcontext_t_regR3]
	str.w       r4,  [r0,#mcontext_t_regR4]
	str.w       r5,  [r0,#mcontext_t_regR5]
	str.w       r6,  [r0,#mcontext_t_regR6]
	str.w       r7,  [r0,#mcontext_t_regR7]
	str.w       r8,  [r0,#mcontext_t_regR8]
	str.w       r9,  [r0,#mcontext_t_regR9]
	str.w       r10, [r0,#mcontext_t_regR10]
	str.w       r11, [r0,#mcontext_t_regR11]
	str.w       r12, [r0,#mcontext_t_regR12]
	str.w       r13, [r0,#mcontext_t_regR13]
	str.w       r14, [r0,#mcontext_t_regR14]
	push	  {r3}
    mov         r3, pc
	str.w       r3,  [r0,#mcontext_t_regPC]
	mov r3, sp
	str.w       r3,  [r0,#mcontext_t_regSP]
	pop       {r3}
    bx        lr ; return


   .global set_context_asm
set_context_asm:
	ldr	 r1 ,  [r0 ,#mcontext_t_regR1]
    ldr  r2 ,  [r0 ,#mcontext_t_regR2]
    ldr  r3 ,  [r0 ,#mcontext_t_regR3]
    ldr  r4 ,  [r0 ,#mcontext_t_regR4]
    ldr  r5 ,  [r0 ,#mcontext_t_regR5]
    ldr  r6 ,  [r0 ,#mcontext_t_regR6]
    ldr  r7 ,  [r0 ,#mcontext_t_regR7]
    ldr  r8 ,  [r0 ,#mcontext_t_regR8]
    ldr  r9 ,  [r0 ,#mcontext_t_regR9]
    ldr  r10,  [r0 ,#mcontext_t_regR10]
    ldr  r11,  [r0 ,#mcontext_t_regR11]
    ldr  r12,  [r0 ,#mcontext_t_regR12]
    ldr  r13,  [r0 ,#mcontext_t_regR13]
    ldr  r14,  [r0 ,#mcontext_t_regR14]

	ldr	 r1 ,  [r0 ,#ucontext_t_initialized]
	cmp  r1 , #0
	bne  inicializado

    ; ainda não inicializado
    mov  r1,   #1
	STR  r1 ,  [r0 ,#ucontext_t_initialized]
	LDR	 r1 ,  [r0 ,#mcontext_t_regR1]
	ldr  R13,  [r0 ,#stack_t_Pss_sp]
    LDR  r15,  [r0 ,#ucontext_t_func]
    bx     lr

inicializado:
	LDR	 r1 ,  [r0 ,#mcontext_t_regR1]
    LDR  r13,  [r0 ,#mcontext_t_regR13]
;    LDR  r15,  [r0 ,#mcontext_t_regR15]
    LDR  r15,  [r0 ,#mcontext_t_regR14]
;	pop     {pc}
    bx     lr

    .global    swap_context_asm
swap_context_asm:
	str.w       r0,  [r0,#mcontext_t_regR0]
	str.w       r1,  [r0,#mcontext_t_regR1]
	str.w       r2,  [r0,#mcontext_t_regR2]
	str.w       r3,  [r0,#mcontext_t_regR3]
	str.w       r4,  [r0,#mcontext_t_regR4]
	str.w       r5,  [r0,#mcontext_t_regR5]
	str.w       r6,  [r0,#mcontext_t_regR6]
	str.w       r7,  [r0,#mcontext_t_regR7]
	str.w       r8,  [r0,#mcontext_t_regR8]
	str.w       r9,  [r0,#mcontext_t_regR9]
	str.w       r10, [r0,#mcontext_t_regR10]
	str.w       r11, [r0,#mcontext_t_regR11]
	str.w       r12, [r0,#mcontext_t_regR12]
	str.w       r13, [r0,#mcontext_t_regR13]
	str.w       r14, [r0,#mcontext_t_regR14]
	push	  {r3}
    mov         r3, r15
	str.w       r3,  [r0,#mcontext_t_regR15]
	pop       {r3}

;	//LDR	 r1 ,  [r1 ,#mcontext_t_regR1]
    LDR  r2 ,  [r1 ,#mcontext_t_regR2]
    LDR  r3 ,  [r1 ,#mcontext_t_regR3]
    LDR  r4 ,  [r1 ,#mcontext_t_regR4]
    LDR  r5 ,  [r1 ,#mcontext_t_regR5]
    LDR  r6 ,  [r1 ,#mcontext_t_regR6]
    LDR  r7 ,  [r1 ,#mcontext_t_regR7]
    LDR  r8 ,  [r1 ,#mcontext_t_regR8]
    LDR  r9 ,  [r1 ,#mcontext_t_regR9]
    LDR  r10,  [r1 ,#mcontext_t_regR10]
    LDR  r11,  [r1 ,#mcontext_t_regR11]
    LDR  r12,  [r1 ,#mcontext_t_regR12]
;    LDR  r13,  [r0 ,#mcontext_t_regR13]
    LDR  r14,  [r1 ,#mcontext_t_regR14]

	LDR	 r0 ,  [r1 ,#ucontext_t_initialized]
	CMP  r0 , #0
	BNE  inicializado1

    ; ainda nao inicializado
    mov  r0,   #1
	str  r0 ,  [r1 ,#ucontext_t_initialized]
	;LDR	 r1 ,  [r1 ,#mcontext_t_regR1]
	ldr  R13,  [r1 ,#stack_t_Pss_sp]
    LDR  r0 ,  [r1 ,#mcontext_t_regR0]
    LDR  r15,  [r1 ,#ucontext_t_func]
    bx     lr

inicializado1:
	;LDR	 r1 ,  [r1 ,#mcontext_t_regR1]
    LDR  r13,  [r1 ,#mcontext_t_regR13]
;    LDR  r15,  [r1 ,#mcontext_t_regR15]
    LDR  r0 ,  [r1 ,#mcontext_t_regR0]
    LDR  r15,  [r1 ,#mcontext_t_regR14]
;	pop     {pc}
    bx     lr


