SPU_IN
POP BX      ;число, до которого считаем факториал

PUSH 1
POP CX      ;счётчик = 1

PUSH 1      
CALL Factorial:

SPU_OUT
HLT

Factorial:
PUSH CX
PUSH BX

JAE END_OF_REC:
PUSH CX
SPU_OUT
PUSH CX
PUSH 1
ADD         ; счётчик ++
POP CX

PUSH CX     
MUL         ; умножить на предыдущее значение

CALL Factorial:     ; рекурсивный вызов, принимающий как аргумент полученное произведение

END_OF_REC:         
RET