SPU_IN
POP BX      ;�����, �� �������� ������� ���������

PUSH 1
POP CX      ;������� = 1

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
ADD         ; ������� ++
POP CX

PUSH CX     
MUL         ; �������� �� ���������� ��������

CALL Factorial:     ; ����������� �����, ����������� ��� �������� ���������� ������������

END_OF_REC:         
RET