
.data
contador:       .word 0
.set GPT_CR,    0x53FA0000
.set GPT_PR,    0x53FA0004
.set GPT_SR,    0x53FA0008
.set GPT_OCR1,  0x53FA0010
.set GPT_IR,    0x53FA000C

.text
.org 0x0
.section .iv,"a"

_start:

interrupt_vector:
    b RESET_HANDLER
.org 0x18
    b IRQ_HANDLER


.org 0x100

RESET_HANDLER:

    ldr r0, =interrupt_vector       @ r0 aponta para a tabela de interrupcoes
    mcr p15, 0, r0, c12, c0, 0

    ldr r1, =GPT_CR                 @ r1 := endereco de GPT_CR
    mov r2, #0x41                   @ configura o clock_src para periférico
    str r2, [r1]

    ldr r1, =GPT_PR                 @ r1 := endereco de GPT_PR
    mov r2, #0                      @ zera o prescaler
    str r2, [r1]

    ldr r1, =GPT_OCR1               @ r1 := endereco de GPT_OCR1
    mov r2, #0x64                   @ valor que queremos contar
    str r2, [r1]

    ldr r1, =GPT_IR                 @ r1 := endereco de GPT_IR
    mov r2, #1                      @ habilita a interrupção do GPT
    str r2, [r1]


    fim_pilha:                      @ pilha do IRQ
        .skip 416                   @ tamanho de 13 registradores
    inicio_pilha:



SET_TZIC:

    .set TZIC_BASE,         0x0FFFC000
    .set TZIC_INTCTRL,      0x0
    .set TZIC_INTSEC1,      0x84
    .set TZIC_ENSET1,       0x104
    .set TZIC_PRIOMASK,     0xC
    .set TZIC_PRIORITY9,    0x424


    ldr	r1, =TZIC_BASE              @ Liga o controlador de interrupcoes


    mov	r0, #(1 << 7)               @ Configura interrupcao 39 do GPT como
    str	r0, [r1, #TZIC_INTSEC1]     @ nao segura

    mov	r0, #(1 << 7)               @ Habilita interrupcao 39 (GPT)
    str	r0, [r1, #TZIC_ENSET1]

    ldr r0, [r1, #TZIC_PRIORITY9]   @ Configure interrupt39 priority as 1
    bic r0, r0, #0xFF000000
    mov r2, #1
    orr r0, r0, r2, lsl #24
    str r0, [r1, #TZIC_PRIORITY9]

    eor r0, r0, r0                  @ Configure PRIOMASK as 0
    str r0, [r1, #TZIC_PRIOMASK]

    mov	r0, #1                      @ Habilita o controlador de interrupcoes
    str	r0, [r1, #TZIC_INTCTRL]

                                    @ instrucao msr - habilita interrupcoes
    msr  CPSR_c, #0x13              @ SUPERVISOR mode, IRQ/FIQ enabled

    laco:
        b laco                      @ aguarda a interrupcao


IRQ_HANDLER:
    ldr r13, =inicio_pilha          @ R13_IRQ aponta para o inicio da pilha
    push {r0-r12}                   @ salva os registradores na pilha do IRQ

    ldr r0, =GPT_SR                 @ GPT_SR := 1
    mov r1, #1                      @ indica que o processador esta ciente
    str r1, [r0]                    @ da interrupcao

    ldr r0, =contador               @ contador := contador + 1
    ldr r1, [r0]
    mov r0, #1
    add r0, r0, r1
    ldr r1, =contador
    str r0, [r1]

    pop {r0-r12}                    @ recupera os registradores

    movs pc, lr
