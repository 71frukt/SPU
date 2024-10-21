crtwnd
setrndram
draw

NEW_FIELD:
call START_FIELD:
pop BX

NEXT_PIXEL:

call CountNegr:
pop CX

push [BX]
push 0
JE DED_CELL:

push CX
push 4
JE CELL_DYE:

push CX
push 1
JE CELL_DYE:

DED_CELL:
push CX
push 3
JE CELL_BORN:
jump IT_END:

CELL_BORN:
    push 1
    pop [BX]
    jump IT_END:


CELL_DYE:
    push 0
    pop [BX]

IT_END:

    push BX
    push 1
    add
    pop BX

    push BX
    call END_FIELD:
JBE NEXT_PIXEL:

draw

jump NEW_FIELD:

hlt

    RAM_CAPA:
push 100
RET

    START_FIELD:
push 11
RET

    END_FIELD:
push 88
RET

    RAM_SIZE_X:
push 10
RET

CountNegr:
    call LU_IP:
            
    pop DX
    push [DX]

    call MU_IP:
    pop DX
    push [DX]

    call RU_IP:
    pop DX
    push [DX]

    call LD_IP:
    pop DX
    push [DX]

    call MD_IP:
    pop DX
    push [DX]

    call RD_IP:
    pop DX
    push [DX]

    call LM_IP:
    pop DX
    push [DX]

    call RM_IP:
    pop DX
    push [DX]

    add
    add
    add
    add
    add
    add  
RET


LU_IP:
    push BX
    push 1
    sub
    call RAM_SIZE_X:
    sub

RET

MU_IP:

    push BX
    push 0
    add
    call RAM_SIZE_X:
    sub
RET

RU_IP:
    push BX
    push 1
    add
    call RAM_SIZE_X:
    sub
RET

LD_IP:
    push BX
    push 1
    sub
    call RAM_SIZE_X:
    add
RET

MD_IP:
    push BX
    push 0
    add
    call RAM_SIZE_X:
    add
RET

RD_IP:
    push BX
    push 1
    add
    call RAM_SIZE_X:
    add
RET

LM_IP:
    push BX
    push 1
    sub
RET

RM_IP:
    push BX
    push 1
    add
RET