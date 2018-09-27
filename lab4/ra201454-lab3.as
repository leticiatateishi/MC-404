.org 0x000
laco:
    LOAD M(endereco_1)
    ADD M(i)
    STA M(003, 8:19)
    LOAD M(endereco_2)
    ADD M(i)
    STA M(003, 28:39)
    LOAD MQ, M(000)
    MUL M(000)
    LOAD MQ
    ADD M(soma)
    STOR M(soma)
    LOAD M(i)
    ADD M(constante_1)
    STOR M(i)
    LOAD M(0x3FF)
    SUB M(i)
    SUB M(constante_1)
    JUMP+ M(000, 0:19)
final:
    LOAD M(soma)
    JUMP M(0x400)
.align 1
constante_1: .word 0x1
soma: .skip 0x1
i: .skip 0x1
.org 0x3FF
tamanho:
.org 0x3FD
endereco_1:
.org 0x3FE
endereco_2:

