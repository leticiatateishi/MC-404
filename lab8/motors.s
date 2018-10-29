@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@
@ Codigo de exemplo para controle basico de um robo.
@ Este codigo le os valores de 2 sonares frontais para decidir se o
@ robo deve parar ou seguir em frente.
@ 2 syscalls serao utilizadas para controlar o robo:
@   write_motors  (syscall de numero 124)
@                 Parametros:
@                       r0 : velocidade para o motor 0  (valor de 6 bits)
@                       r1 : velocidade para o motor 1  (valor de 6 bits)
@
@  read_sonar (syscall de numero 125)
@                 Parametros:
@                       r0 : identificador do sonar   (valor de 4 bits)
@                 Retorno:
@                       r0 : distancia capturada pelo sonar consultado (valor de 12 bits)
@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


.text
.align 4
.globl _start

_start:                         @ main

        mov r0, #0              @ Carrega em r0 a velocidade do motor 0.
                                @ Lembre-se: apenas os 6 bits menos significativos
                                @ serao utilizados.
        mov r1, #0              @ Carrega em r1 a velocidade do motor 1.
        mov r7, #124            @ Identifica a syscall 124 (write_motors).
        svc 0x0                 @ Faz a chamada da syscall.


loop:
        ldr r6, =1500           @ r6 <- 1200 (Limiar para parar o robo)
        mov r0, #3
        mov r1, #4
        bl read_sonar
        cmp r0, r5
        moveq r4, #3
        movne r4, #4

min:
        cmp r0, r6              @ Compara r0 com r6
        blt turn                @ Se r0 < limiar: muda de direcao

        mov r0, #2
        mov r1, #5
        bl read_sonar
        ldr r6, =200
        cmp r0, r6              @ Compara r0 com r6
        bgt go_ahead                @ Se r0 < limiar: muda de direcao
        cmp r0, r5
        moveq r4, #2
        movne r4, #5

        turn:
            cmp r0, r5
            bleq turn_right
            blne turn_left
            mov r7, #124
            @ eu amo meu amorzinho <3<3<3
            svc 0x0
            turn_loop:
                mov r0, r4              @ r0 := sonar de menor distancia
                mov r7, #125            @ Identifica a syscall 125 (read_sonar).
                svc 0x0
                cmp r0, r6
                blt turn_loop
            b loop


        go_ahead:
            mov r0, #36
            mov r1, #36
            mov r7, #124
            svc 0x0

            b loop                  @ Refaz toda a logica

@ Parametros:
@   r0: numero do primeiro sonar
@   r1: numero do segundo sonar
@ Retorno:
@   r0: menor distancia
read_sonar:

    push {lr}

    mov r7, #125            @ Identifica a syscall 125 (read_sonar).
    svc 0x0                 @ chama a syscall para o primeiro sonar (em r0)
    mov r5, r0              @ r5 := distancia do primeiro sonar

    mov r0, r1              @ r0 := numero do outro sonar
    mov r7, #125
    svc 0x0                 @ chama a syscall para o segundo sonar

    cmp r0, r5              @ se r0 > r5
    movgt r0, r5            @ r0 := r5

    pop {pc}



turn_right:
        push {lr}
        mov r0, #0
        mov r1, #30
        pop {pc}

turn_left:
        push {lr}
        mov r0, #30           @ Se r0 != r5, distancia do sonar 4 é a menor
        mov r1, #0
        pop {pc}
