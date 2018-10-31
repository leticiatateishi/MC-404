
.text
.align 4
.globl _start

@ Inicia com o uóli parado e inicializa o limiar de distancia como 1300
_start:                         @ main

        mov r0, #0              @ r0 := velocidade do motor 0.
        mov r1, #0              @ r1 := velocidade do motor 1.
        mov r7, #124            @ Identifica a syscall 124 (write_motors).
        svc 0x0                 @ Faz a chamada da syscall.

        ldr r6, =1300           @ r6 := 1300 (limiar para girar o uóli)


@ Laco infinito que le os sonares do uoli e toma decisoes sobre seu movimento
loop:
        mov r0, #3              @ r0 := 3 (indica o sonar 3)

        mov r7, #125            @ Identifica a syscall 125 (read_sonar).
        svc 0x0                 @ chama a syscall para o primeiro sonar (o 3)
        mov r5, r0              @ r5 := r0 := distancia detectada pelo sonar 3

        mov r0, #4              @ r0 := numero do outro sonar (o 4)
        mov r7, #125
        svc 0x0                 @ chama a syscall para o segundo sonar (o 4)
                                @ r0 recebe a menor distancia entre os 2 sonares
        cmp r5, r0              @ se distancia do sonar 3 > distancia do sonar 4
        movlt r0, r5            @ r0 := distancia do sonar 3

                                @ r4 recebe o numero do sonar que detectou a
                                @ menor distancia
        cmp r0, r5
        moveq r4, #3            @ se r0 == r5, é o sonar 3
        movne r4, #4            @ se r0 != r5, é o sonar 4

        cmp r0, r6              @ Compara a menor distancia com o limiar
        bgt go_ahead            @ Se menor distancia > limiar: segue reto

        turn:                   @ Se menor distancia >= limiar, fazemos curva
            cmp r4, #3          @ Confere o sonar com menor distancia
            bleq turn_right     @ Se for o 3, viramos a direita
            blne turn_left      @ Se for o 4, viramos a esquerda

            mov r7, #124        @ syscall 124 (write_motors)
            svc 0x0

            @ Fazemos curva ate que a menor distancia seja maior que o limiar
            turn_loop:
                mov r0, r4          @ r0 := sonar de menor distancia
                mov r7, #125        @ syscall 125 (read_sonar).
                svc 0x0
                cmp r0, r6          @ Compara a menor distancia com o limiar
                blt turn_loop       @ Continua a curva se for menor que o limiar
            b loop

        @ Iguala as velocidades dos motores para seguir em linha reta
        go_ahead:
            mov r0, #30         @ r0 := velocidade do motor 0
            mov r1, #30         @ r1 := velocidade do motor 1
            mov r7, #124        @ syscall 124 (write_motors)
            svc 0x0

            b loop              @ Refaz toda a logica


@ Ajusta os motores para virar a direita
turn_right:
        push {lr}
        mov r0, #0              @ velocidade 0 para o motor 0
        mov r1, #20             @ velocidade 20 para o motor 1
        pop {pc}


@ Ajusta os motores para virar a esquerda
turn_left:
        push {lr}
        mov r0, #20             @ velocidade 20 para o motor 0
        mov r1, #0              @ velocidade 0 para o motor 1
        pop {pc}
