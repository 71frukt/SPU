SPU_IN
CALL Factorial:
SPU_OUT

HLT


Factorial:
    POP BX                  ; BX == arg

    PUSH BX
    PUSH 1
    JA Recurse:

    PUSH 1                  ; if (arg < 2) return 1
    RET

    Recurse:                ; else
        PUSH BX
        PUSH BX
        PUSH 1
        SUB         
        CALL Factorial:            
        
        MUL                 ;return arg * Factorial(arg - 1)
        RET