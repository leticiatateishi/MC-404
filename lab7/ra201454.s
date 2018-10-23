@ ----------- Data -------------
.data
input_buffer:   .skip 32            @ 32 bytes para input_buffer
output_buffer:  .skip 32            @ 32 bytes para output_buffer
string_newline: .asciz "\n"         @ String, contendo
string_space:   .asciz "  "         @ String contedo espaço


@ ----------- Text -------------
.text
.align 4
.globl ajudaORobinson

ajudaORobinson:

    push {r0, r1, lr}


    bl compare_coordinates
    cmp r0, #0
    bne continue
    mov r0, #0
    pop {r0, r1, pc}

    continue:
        bl posicaoYRobinson         @ R0 := posicao y do Robinson
        mov r1, r0                  @ R1 := posicao y do Robinson
        bl posicaoXRobinson         @ R0 := posicao x do Robinson
        bl daParaPassar             @ R0 == 1 se dá pra passar
        cmp r0, #0                  @ Se não dá pra passar
        moveq r0, 1                 @ R0 := 1
        beq return

        @ Se dá pra passar, visita os vizinhos


        return:
            pop {r0, r1, pc}


@ Visita os vizinhos da posição atual
@ Parametros:
@   R0: posicao x atual
@   R1: posicao y atual
visit_neighbours:
    push {r4, r5, r6, lr}
    sub r4, r0, #1              @ R4 := x-1
    mov r5, #0                  @ i := 0
    loop:
        cmp r5, #3              @ while i < 3
        beq end_loop
        add r0, r4, r5          @ R0 := R4 - i
        bl foiVisitado          @ posicao (R0, R1) já foi visitada
        cmp r0, #0              @ se a posicao nao foi visitada
        moveq r0, r4
        bleq ajudaORobinson


    pop {r4-r7, pc}



@ Compara as coordenadas atuais do Robinson com as coordenadas do local
@ Parametros:
@   R1: coordenada x atual
@   R2: coordenada y atual
@ Retorno:
@   R0 == 1, se o local atual não é o final ou R0 == 0 caso contrario
compare_coordinates:
    push {lr}
    bl posicaoXLocal                @ R0 := posição x do local
    cmp r0, r1                      @ R0 != R1?
    mov r0, #1
    bne return_comparation
    bl posicaoYLocal                @ R0 := posição y do local
    cmp r0, r2                      @ R0 != R1?
    moveq r0, #0

    return_comparation:
        pop {pc}

@ Imprime uma quebra de linha
print_newline:
    push {lr}
    ldr r0, =string_newline
    mov r1, #1
    bl write
    pop {pc}

@ Imprime um espaço
print_space:
    push {lr}
    ldr r0, =string_space
    mov r1, #1
    bl write
    pop {pc}


@ Divide R0 por R1, subtraindo sucessivamente (para numeros não sinalizados!)
@ Parametros:
@   r0: Inteiro positivo dividendo
@   r1: Inteiro positivo divisor
@ Retorno:
@   r0: Quociente
@   r1: Resto
div:
    push {r4, lr}
    mov r4, r1
    mov r1, r0
    mov r0, #0
div_loop:
    cmp r1, r4
    subhs r1, r1, r4
    addhs r0, r0, #1
    bhs div_loop
    pop {r4, pc}


@ Le uma sequencia de bytes da entrada padrao.
@ parametros:
@  r0: endereco do buffer de memoria que recebera a sequencia de bytes.
@  r1: numero maximo de bytes que pode ser lido (tamanho do buffer).
@ retorno:
@  r0: numero de bytes lidos.
read:
    push {r4-r7, lr}
    mov r4, r0
    mov r5, r1
    mov r0, #0         @ stdin file descriptor = 0
    mov r1, r4         @ endereco do buffer
    mov r2, r5         @ tamanho maximo.
    mov r7, #3         @ read
    svc 0x0
    pop {r4-r7, pc}


@ Escreve uma sequencia de bytes na saida padrao.
@ parametros:
@  r0: endereco do buffer de memoria que contem a sequencia de bytes a ser impressa.
@  r1: numero de bytes a serem escritos
write:
    push {r4-r7, lr}
    mov r4, r0
    mov r5, r1
    mov r0, #1         @ stdout file descriptor = 1
    mov r1, r4         @ endereco do buffer
    mov r2, r5         @ tamanho do buffer.
    mov r7, #4         @ write
    svc 0x0
    pop {r4-r7, pc}


@ Finaliza a execucao de um processo.
@  r0: codigo de finalizacao (Zero para finalizacao correta)
exit:
    mov r7, #1         @ syscall number for exit
    svc 0x0


@ Converte uma string numerica para um inteiro
@ Parametros
@   r0: endereço do buffer contendo a string que será convertida em inteiro
@   r1: tamanho da string (numero de caracteres)
@   r2: base do numero que a string se encontra (maximo: 16).
@   Os digitos A-F, devem ser maiusculos!
@ Retorno:
@   r0: valor da string, em inteiro (sem sinal)
atoi:
    push {r4-r7, lr}
    mov r3, #0          @value
    mov r4, #0          @i
atoi_loop:
    cmp r4, r1
    bhs atoi_done
    ldrb r5, [r0, r4]       @ r5 = c
    sub r5, r5, #'0'        @ r5 = c - '0'
    cmp r5, #9              @ r5 > 9?
    subhi r5, r5, #7        @ if(r5 > 9) r5 -= 7
    mul r6, r2, r3          @ r6 =  base * value
    add r3, r5, r6          @ r3 = r5 + r6 (c - '0' + base * value)
    add r4, r4, #1
    b atoi_loop
atoi_done:
    mov r0, r3
    pop {r4-r7, pc}


@ Reverte uma string (utilizado apenas por itoa)
@ Parametros
@   r0: endereço string a ser invertida
@   r1: numero de caracteres da string
@ Retorno:
@   r0: endereço da string invertida
reverse:
    push {r4, lr}
    mov r2, #0      @i
    sub r1, r1, #1  @j-1

reverse_loop:
    cmp r2, r1      @i>j?
    bhs reverse_done    @if true: done

    ldrb r3, [r0, r2]    @load byte from r0[i]
    ldrb r4, [r0, r1]    @load byte from r0[j]

    strb r3, [r0, r1]    @store r0[i] on r0[j]
    strb r4, [r0, r2]    @store r0[j] on r0[i]

    sub r1, r1, #1
    add r2, r2, #1
    b reverse_loop

reverse_done:
    pop {r4, pc}


@ Converte um inteiro para uma string
@ Parametros
@   r0: valor inteiro a ser convertido para string
@   r1: endereço de onde será armazenado a string ( Ex.: output_buffer )
@   r2: base para a string (Max: 16)
@ Retorno
@   r0: endereço para string, que contem o inteiro convertido para string
@   r1: numero de digitos (caracteres) da string

itoa:
    push {r4-r7, lr}
    mov r3, r1          @pointer to buffer
    mov r4, r0          @value acc
    mov r5, #0          @qtde dig

    cmp r4, #0          @value == 0?
    beq itoa_zero

itoa_loop:
    cmp r4, #0              @loop control
    bls itoa_done           @done
    mov r0, r4              @r4 = atual value
    mov r1, r2              @divisor
    bl div                  @div returns quoc r0 and remaind r1
    mov r4, r0              @mov quoc to actual value
    cmp r1, #9              @compare remainder with 9
    addhi r1, r1, #55       @if(r1 > 9) r1 + 55    (a-f)
    addls r1, r1, #48       @else r1 + 48          (0-9)
    strb r1, [r3, r5]       @store r1 in r3[digito]
    add r5, r5, #1          @digitos++
    b itoa_loop             @loop_back

itoa_zero:
    mov r1, #'0'            @add a single '0'
    strb r1, [r3, r5]       @store r1 --> r3[i]
    add r5, r5, #1          @digitos ++

itoa_done:
    mov r0, r3
    mov r1, r5              @i (number of digits)
    bl reverse
    mov r1, r5              @i (number of digits)
    pop {r4-r7, pc}
