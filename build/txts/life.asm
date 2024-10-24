CRTWND              ; создать окно
SETRNDRAM           ; выставить рандомные пиксели
CALL NEW_FIELD:
HLT

NEW_FIELD:
    DRAW

    PUSH 0
    POP BX

NEXT_PIXEL:
    CALL CountNegr:

    POP CX

    PUSH [BX]
    PUSH 0

JE DEAD_CELL:

    PUSH CX
    PUSH 3
    JA CELL_DIE:

    PUSH CX
    PUSH 2
    JB CELL_DIE:

    JMP CELL_BORN:


DEAD_CELL:
    PUSH CX
    PUSH 3
    JE CELL_BORN:
    JMP CELL_DIE:


CELL_BORN:
    PUSH 1
    CALL POPExtraRamBX:
    JMP IT_END:


CELL_DIE:
    PUSH 0
    CALL POPExtraRamBX:
    JMP IT_END:

IT_END:
    PUSH BX
    PUSH 1
    ADD
    POP BX

    PUSH BX
    CALL RAM_CAPA:
    JB NEXT_PIXEL:

    CALL CopyExtraRamInRam:
    JMP NEW_FIELD:

    RET


RAM_SIZE_X:
    PUSH 120
    RET

RAM_CAPA:
    CALL RAM_SIZE_X:
    CALL RAM_SIZE_X:
    MUL
    RET

CountNegr:
    CALL LU_IP:        
    POP DX
    PUSH [DX]

    CALL MU_IP:
    POP DX
    PUSH [DX]

    CALL RU_IP:
    POP DX
    PUSH [DX]

    CALL LD_IP:
    POP DX
    PUSH [DX]

    CALL MD_IP:
    POP DX
    PUSH [DX]

    CALL RD_IP:
    POP DX
    PUSH [DX]

    CALL LM_IP:
    POP DX
    PUSH [DX]

    CALL RM_IP:
    POP DX
    PUSH [DX]

    ADD
    ADD
    ADD
    ADD
    ADD
    ADD 
    ADD
    
    RET

POPExtraRamBX:
    PUSH BX
    CALL RAM_CAPA:
    ADD
    POP BX

    POP [BX]

    PUSH BX
    CALL RAM_CAPA:
    SUB
    POP BX
    
    RET

CopyExtraRamInRam:
    PUSH 0
    POP BX


RAM_NOT_COPIED: 
    PUSH BX
    CALL RAM_CAPA:
    ADD
    POP BX

    PUSH [BX]

    PUSH BX
    CALL RAM_CAPA:
    SUB
    POP BX

    POP [BX]

    PUSH BX
    PUSH 1
    ADD
    POP BX

    PUSH BX
    CALL RAM_CAPA:
    JB RAM_NOT_COPIED:
    
    RET

LU_IP:
    PUSH BX

    PUSH 1
    SUB

    CALL RAM_SIZE_X:
    SUB

    CALL RAM_CAPA:
    ADD

    CALL RAM_CAPA:
    MOD

    RET

MU_IP:
    PUSH BX
    PUSH 0
    ADD
    CALL RAM_SIZE_X:
    SUB
    CALL RAM_CAPA:
    ADD
    CALL RAM_CAPA:
    MOD

    RET

RU_IP:
    PUSH BX
    PUSH 1
    ADD
    CALL RAM_SIZE_X:
    SUB
    CALL RAM_CAPA:
    ADD
    CALL RAM_CAPA:
    MOD

    RET

LD_IP:
    PUSH BX
    PUSH 1
    SUB
    CALL RAM_SIZE_X:
    ADD
    CALL RAM_CAPA:
    ADD
    CALL RAM_CAPA:
    MOD

    RET

MD_IP:
    PUSH BX
    PUSH 0
    ADD
    CALL RAM_SIZE_X:
    ADD
    CALL RAM_CAPA:
    ADD
    CALL RAM_CAPA:
    MOD
    
    RET

RD_IP:
    PUSH BX

    PUSH 1
    ADD
    CALL RAM_SIZE_X:
    ADD
    CALL RAM_CAPA:
    ADD
    CALL RAM_CAPA:
    MOD
  
    RET

LM_IP:
    PUSH BX
    PUSH 1
    SUB
    CALL RAM_CAPA:
    ADD
    CALL RAM_CAPA:
    MOD
    
    RET

RM_IP:
    PUSH BX
    PUSH 1
    ADD
    CALL RAM_CAPA:
    ADD
    CALL RAM_CAPA:
    MOD
    
    RET