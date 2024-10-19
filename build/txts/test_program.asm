push 5
push 65
push 123
push 5

pop BX

pop [BX]

call PLUS:

out
draw
hlt


PLUS:
    add
    push 70
    push 80

    call CUB_POW:
RET

CUB_POW:
    mul
    mul
RET