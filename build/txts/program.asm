push [125]

push 5
push 6

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