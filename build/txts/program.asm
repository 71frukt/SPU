push 30
push 40

call PLUS:

out
hlt

PLUS:
add
push 70
push 80

JE FUNC:

call CUB_POW:
RET

FUNC:
CUB_POW:
mul
mul
RET