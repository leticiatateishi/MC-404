
.data
contador:   .skip 32
GPT_CR:     .word 0x53FA0000000000000000000000000000
GPT_PR:     .word 0x53F

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

    @ Zera o contador
    ldr r2, =contador
    mov r0, #0
    str r0, [r2]

    @Faz o registrador que aponta para a tabela de interrupções apontar para a tabela interrupt_vector
    ldr r0, =interrupt_vector
    mcr p15, 0, r0, c12, c0, 0

    @ Ajustar a pilha do modo IRQ.
    @ Você deve iniciar a pilha do modo IRQ aqui. Veja abaixo como usar a instrução MSR para chavear de modo.
    @ ...

    @@@...continua tratando o reset
    @ enviar os dados pro gpt
    ldr r1, =GPT_CR
    mov r2, #0x41
    mov r2, [r1]
