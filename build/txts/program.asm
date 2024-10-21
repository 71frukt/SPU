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
push 3
JA CELL_DYE:

push CX
push 2
JB CELL_DYE:

push 1
call PopExtraRamBX:

jump IT_END:

DED_CELL:
push CX
push 3
JE CELL_BORN:
jump IT_END:

CELL_BORN:
    push 1
    call PopExtraRamBX:
    jump IT_END:


CELL_DYE:
    push 0
    call PopExtraRamBX:
    jump IT_END:

IT_END:
    push BX
    push 1
    add
    pop BX

    push BX
    call END_FIELD:
JBE NEXT_PIXEL:

call CopyExtraRamInRam:

draw

jump NEW_FIELD:

hlt


    RAM_SIZE_X:
push 20
RET

    RAM_CAPA:
call RAM_SIZE_X:
call RAM_SIZE_X:
mul
RET

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

PopExtraRamBX:
    push BX
    call RAM_CAPA:
    add
    pop BX

    pop [BX]

    push BX
    call RAM_CAPA:
    sub
    pop BX
RET

CopyExtraRamInRam:
    call START_FIELD:
    pop BX


    RAM_NOT_COPIED: 
        push BX
        call RAM_CAPA:
        add
        pop BX

        push [BX]

        push BX
        call RAM_CAPA:
        sub
        pop BX

        pop [BX]

        push BX
        push 1
        add
        pop BX

        push BX
        call END_FIELD:
    JB RAM_NOT_COPIED:
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