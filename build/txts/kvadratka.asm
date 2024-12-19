SPU_IN
POP AX
SPU_IN
POP BX
SPU_IN
POP CX

CALL ROOT_1:
SPU_OUT

CALL ROOT_2:
SPU_OUT

HLT


DISCRIMINANT:
    PUSH BX
    PUSH BX
    MUL

    PUSH 4
    PUSH AX
    PUSH CX
    MUL
    MUL

    SUB

    RET

ROOT_1:
    CALL DISCRIMINANT:
    POP DX

    PUSH DX
    PUSH 0

    JB NEGATIVE_DISCRIMINANT_X1:
    JMP POSITIVE_DISCRIMINANT_X1:

    NEGATIVE_DISCRIMINANT_X1:
        PUSH -666
        RET

    POSITIVE_DISCRIMINANT_X1:
        PUSH BX
        PUSH -1
        MUL

        PUSH DX        
        SQRT

        SUB

        PUSH AX
        DIV

        PUSH 2
        DIV

        RET

ROOT_2:
    CALL DISCRIMINANT:
    POP DX

    PUSH DX
    PUSH 0

    JB NEGATIVE_DISCRIMINANT_X2:
    JMP POSITIVE_DISCRIMINANT_X2:

    NEGATIVE_DISCRIMINANT_X2:
        PUSH -666
        RET

    POSITIVE_DISCRIMINANT_X2:
        PUSH BX
        PUSH -1
        MUL

        PUSH DX        
        SQRT

        ADD

        PUSH AX
        DIV

        PUSH 2
        DIV

        RET