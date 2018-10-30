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

        ldr r6, =1300           @ r6 <- 1200 (Limiar para parar o robo)

loop:
        mov r0, #3
        mov r1, #4
        mov r7, #125            @ Identifica a syscall 125 (read_sonar).
        svc 0x0                 @ chama a syscall para o primeiro sonar (em r0)
        mov r5, r0              @ r5 := r0 := distancia do primeiro sonar

        mov r0, #4              @ r0 := numero do outro sonar
        mov r7, #125            @ r0 := distancia do segundo sonar
        svc 0x0                 @ chama a syscall para o segundo sonar
        cmp r5, r0              @ se r0 > r5
        movlt r0, r5            @ r0 := r5

        cmp r0, r5
        moveq r4, #3
        movne r4, #4

min:
        cmp r0, r6               @ Compara menor distancia detectada com o limiar
        bgt go_ahead             @ Se r0 > limiar: segue reto

        turn:
            cmp r4, #3
            bleq turn_right
            blne turn_left
            mov r7, #124
            svc 0x0
            turn_loop:
                mov r0, r4              @ r0 := sonar de menor distancia
                mov r7, #125            @ Identifica a syscall 125 (read_sonar).
                svc 0x0
                cmp r0, r6
                blt turn_loop
            b loop


        go_ahead:
            mov r0, #30
            mov r1, #30
            mov r7, #124
            svc 0x0

            b loop                  @ Refaz toda a logica

@ Parametros:
@   r0: numero do primeiro sonar
@   r1: numero do segundo sonar
@ Retorno:
@   r0: menor distancia
@   r1: numero do sonar de menor distancia
read_sonar:

    push {r4-r7, lr}

    mov r4, r0              @ r4 := # do primeiro sonar
    mov r5, r1              @ r5 := # do segundo sonar
    mov r7, #125            @ Identifica a syscall 125 (read_sonar).
    svc 0x0                 @ chama a syscall para o primeiro sonar (em r0)
    mov r6, r0              @ r6 := distancia do primeiro sonar

    mov r0, r1              @ r0 := numero do outro sonar
    mov r7, #125
    svc 0x0                 @ chama a syscall para o segundo sonar
                            @ r0 := distancia do segundo sonar

    cmp r6, r0              @ se distancia do primeiro sonar < distancia do segundo sonar
    movlt r0, r6            @ r0 := distancia do primeiro sonar
    movlt r1, r4            @ r1 := # do primeiro sonar
    movge r1, r5            @ r1 := # do segundo sonar

    pop {r4-r7, pc}



turn_right:
        push {lr}
        mov r0, #0
        mov r1, #20
        pop {pc}

turn_left:
        push {lr}
        mov r0, #20           @ Se r0 != r5, distancia do sonar 4 é a menor
        mov r1, #0
        pop {pc}
