.data
input_buffer:   .skip 32
output_buffer:  .skip 32
previous_line:  .skip 3072
current_line:   .skip 3072

.text
.align 4
.globl _start


@ Função principal
_start:
    @ Chama a funcao "read" para ler 4 caracteres da entrada padrao
    ldr r0, =input_buffer
    mov r1, #4             @ 3 caracteres + '\n'
    bl  read

    ldr r0, =input_buffer  @ endereço entrada
    mov r1, #3             @ Numero caracteres
    mov r2, #16            @ Base
    bl atoi                @ Chama atoi (r0 := n em inteiro)

    mov r1, #0             @ linha

    lines_loop:
        cmp r1, r0             @ inicio do loop que itera nas linhas
        bge line_done          @ 0 < linha < n
        mov r2, #0             @ coluna

        column_loop:
            cmp r2, r1                  @ inicio do loop que itera nas colunas
            bgt column_done             @ 0 < coluna <= linha

            cmp r2, #0                  @ se r2 == 0
            beq put_one                 @ salta para put_one e coloca um no vetor atual
            cmp r1, r2                  @ se r1 == r2
            beq put_one                 @ salta para put_one e coloca um no vetor atual

    	    ldr r6, =previous_line      @ r6 recebe o endereço do vetor antigo
            ldr r5, [r6, r2, lsl #2]    @ r5 := previous_line[4*coluna]
    	    sub r7, r2, #1	            @ r7 := coluna-1
    	    ldr r8, [r6, r7, lsl #2]	@ r8 := previous_line[4*(coluna-1)]
            add r4, r5, r8	            @ r4 := previous_line[coluna]+previous_line[coluna-1]
            b continue

            put_one:
                mov r4, #1              @ r4 := 1

            continue:
                ldr r5, =current_line       @ r5 := &current_line
                add r5, r5, r2, lsl #2      @ r5 := &current_line[coluna*4]
                str r4, [r5]                @ current_line[coluna*4] = r4

                push {r0, r1, r2}           @ salva os registradores
                mov r0, r4                  @ r0 := previous_line[coluna]+previous_line[coluna-1]
                ldr r1, =output_buffer
                mov r2, #16
                bl itoa
                mov r1, #4                  @ r1 := 1 (numero de bytes a serem escritos)
                bl write                    @ escreve r0
                pop {r0, r1, r2}            @ recupera os registradores

                add r2, r2, #1
                b column_loop

        column_done:
            push {r0, r1, r2}
            ldr r0, =current_line
            ldr r1, =previous_line
            bl copy_array
            pop {r0, r1, r2}
            add r1, r1, #1
            b lines_loop
    line_done:


    @ldr r1, =output_buffer @ endereço para armazenar a string referente ao inteiro
    @mov r2, #10            @ Base a ser impressa
@    bl itoa                @ Chama itoa

@    bl write

    mov r0, #0             @ Status de retorno
    b exit

@ Copia current_line para previous_line
@ Parametros:
@   r0: endereco de current_line
@   r1: endereco de previous_line
@   r2: tamanho de current_line
copy_array:
    push {r4, r5, r6, r7}
    mov r4, #0                      @ i := 0
    copy_loop:
        cmp r4, r2                  @ Compara i e tamanho de current_line
        bge copy_loop_end           @ 0 < i < tamanho de current_line
        mov r5, #4
        mul r6, r4, r5              @ r6 := 4*i
        ldr r5, [r0, r6]            @ r5 := current_line[4*i]
        add r7, r1, r6              @ r7 := &previous_line + 4*i
        str r5, [r7]
        add r4, r4, #1
    copy_loop_end:
    pop {r4, r5, r6, r7}


@ Divide R0 por R1, subtraindo sucessivamente
@ Parametros:
@   r0: Inteiro positivo divisor
@   r1: Inteiro positivo dividendo
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
    subge r1, r1, r4
    addge r0, r0, #1
    bge div_loop
    pop {r4, pc}


@ Le uma sequencia de bytes da entrada padrao.
@ parametros:
@  r0: endereco do buffer de memoria que recebera a sequencia de bytes.
@  r1: numero maximo de bytes que pode ser lido (tamanho do buffer).
@ retorno:
@  r0: numero de bytes lidos.
read:
    push {r4,r5, lr}
    mov r4, r0
    mov r5, r1
    mov r0, #0         @ stdin file descriptor = 0
    mov r1, r4         @ endereco do buffer
    mov r2, r5         @ tamanho maximo.
    mov r7, #3         @ read
    svc 0x0
    pop {r4, r5, lr}
    mov pc, lr

@ Escreve uma sequencia de bytes na saida padrao.
@ parametros:
@  r0: endereco do buffer de memoria que contem a sequencia de bytes.
@  r1: numero de bytes a serem escritos
write:
    push {r4,r5, r7, lr}
    mov r4, r0
    mov r5, r1
    mov r0, #1         @ stdout file descriptor = 1
    mov r1, r4         @ endereco do buffer
    mov r2, r5         @ tamanho do buffer.
    mov r7, #4         @ write
    svc 0x0
    pop {r4, r5, r7, lr}
    mov pc, lr

@ Finaliza a execucao de um processo.
@  r0: codigo de finalizacao (Zero para finalizacao correta)
exit:
    mov r7, #1         @ syscall number for exit
    svc 0x0


@ Converte uma string numerica para um inteiro
@ Parametros
@   r0: endereço do buffer contendo a string
@   r1: tamanho da string (numero de elementos)
@   r2: base do numero (maximo: 16). Os digitos A-F, devem ser maiusculos!
@ Retorno:
@   r0: valor da string, em inteiro
atoi:
    push {r4-r6, lr}
    mov r3, #0          @value
    mov r4, #0          @i
atoi_loop:
    cmp r4, r1
    bge atoi_done
    ldrb r5, [r0, r4]       @ r5 = c
    sub r5, r5, #'0'        @ r5 = c - '0'
    cmp r5, #9              @ r5 > 9?
    subgt r5, r5, #7        @ if(r5 > 9) r5 -= 7
    mul r6, r2, r3          @ r6 =  base * value
    add r3, r5, r6          @ r3 = r5 + r6 (c - '0' + base * value)
    add r4, r4, #1
    b atoi_loop
atoi_done:
    mov r0, r3
    pop {r4-r6, pc}


@ Reverte uma string
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
    bge reverse_done    @if true: done

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
@   r0: valor a ser convertido
@   r1: endereço de onde será armazenado a string
@   r2: base a ser impressa (Max: 16)
@ Retorno
@   r0: endereço para string
@   r1: numero de digitos da string

itoa:
    push {r4-r5, lr}
    mov r3, r1          @pointer to buffer
    mov r4, r0          @value acc
    mov r5, #0          @qtde dig

    cmp r4, #0          @value == 0?
    beq itoa_zero

itoa_loop:
    cmp r4, #0              @loop control
    ble itoa_done           @done
    mov r0, r4              @r4 = atual value
    mov r1, r2              @dividend
    bl div                  @div returns quoc r0 and remaind r1
    mov r4, r0              @mov quoc to actual value
    cmp r1, #9              @compare remainder with 9
    addgt r1, r1, #55       @if(r1 > 9) r1 + 55    (a-f)
    addle r1, r1, #48       @else r1 + 48          (0-9)
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
    pop {r4-r5, pc}
