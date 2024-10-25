SPU_IN
CALL Factorial:
SPU_OUT

HLT


Factorial:
    POP BX                  ; BX == arg

    PUSH BX
    PUSH 1
    JBE End:

    PUSH BX
    PUSH BX
    PUSH 1
    SUB         
    CALL Factorial:            

    MUL                     ; if (arg > 1) return arg * Factorial(arg - 1)
    RET

    End:
        PUSH 1              ; else if (arg <= 1) return 1
        RET
