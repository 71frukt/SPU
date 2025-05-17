CRTWND

CALL CENTER_IP:
POP CX

PUSH 5
POP [CX]

NEW_CIRCLE:

PUSH 0
POP BX

CYCLE:

CALL PushCenterDist:
CALL RADIUS:

JA NEXT:
PUSH 1
POP [BX]

NEXT:
PUSH BX
PUSH 1
ADD
POP BX

PUSH BX
CALL RAM_SIZE:

JB CYCLE:

DRAW
CALL CleanRAM:

PUSH CX
PUSH 1
ADD
POP CX

JMP NEW_CIRCLE:

HLT



PushCenterDist:
                PUSH BX
                CALL RAM_SIZE_X:
                MOD

                PUSH CX
                CALL RAM_SIZE_X:
                MOD
            SUB

                PUSH BX
                CALL RAM_SIZE_X:
                MOD

                PUSH CX
                CALL RAM_SIZE_X:
                MOD
            SUB
        MUL
        

                PUSH BX
                CALL RAM_SIZE_X:
                DIV

                PUSH CX
                CALL RAM_SIZE_X:
                DIV
            SUB

                PUSH BX
                CALL RAM_SIZE_X:
                DIV

                PUSH CX
                CALL RAM_SIZE_X:
                DIV
            SUB
        MUL
    ADD

    SQRT
RET

CleanRAM:
    PUSH 0
    POP DX

    CLEAN_NEW_PIXEL:
    PUSH 0
    POP [DX]

    PUSH DX
    PUSH 1
    ADD
    POP DX

    PUSH DX
    CALL RAM_SIZE:

    JB CLEAN_NEW_PIXEL:
RET

CENTER_IP:
    PUSH 5555
    RET

RADIUS: 
    PUSH 40 
    RET

RAM_SIZE_X:
    PUSH 120
    RET

RAM_SIZE:
    CALL RAM_SIZE_X:
    CALL RAM_SIZE_X:
    MUL
    RET