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
JAE CELL_DYE:

push CX
push 1
JBE CELL_DYE:

jump IT_END:

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
    jump IT_END:

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

    START_FIELD:
call RAM_SIZE_X:
push 1
add
RET

    END_FIELD:
call RAM_CAPA:
call RAM_SIZE_X:
sub
push 2
sub
RET

    RAM_CAPA:
push 1600
RET

    RAM_SIZE_X:
push 40
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