.org 0x000
inicio:
    LOAD MQ, M(gravidade)
    MUL M(0x105)
    LOAD MQ
    STOR M(y)
    RSH
    STOR M(k)
laco:
    LOAD M(y)
    DIV M(k)
    LOAD MQ
    ADD M(k)
    RSH
    STOR M(k)
    LOAD M(contador)
    SUB M(constante_1)
    STOR M(contador)
    JUMP+ M(laco)
final:
    LOAD M(k)
    JUMP M(0x400)
.align 1
gravidade: .word 0xA
k: .skip 1
y: .skip 1
contador: .word 0xA
constante_1: .word 0x1