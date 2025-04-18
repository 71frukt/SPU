# ����������� ���������
������� �������, 1 ���� ���� ����

## ������ � ������
*��������� �������� ��� ������� Windows.*

### ������
1. ���������� �����������
```bash
$ git clone https://github.com/71frukt/SPU.git
```

2. �������� ������

```bash
$ make
```

### ������

1. ��������� � ����� `build`
```bash
$ cd build
```

2. ��������� ���������
```bash
$ compiler.exe <code_file> <binary_file> 
```

3. ��������� ���������
```bash
$ spu.exe <binary_file>
```


## �������� �������
������ ������������ ����� ��������� ����������, ����������� ������ � ���������� � ����������� �������, ������������ � ���� �����������. 

��������� ������� �� ���� ����������� - ��������� � ���������. ��������� ��������� ��� ��� �� �������� ����, � ��������� ������ �������� ��� � ��������� ���������. ��������� ��� �� ������ ���� ����� ����.

## 1. ���������
��������� - ���������, ������� ��������� ���������� ��� � ��������. ������ ���������� ���������� ���������� 16 ������: ������ 13 ��� ���������� ��� ����������� ���� ����������, ��������� 3 ��������� ������� � ������� � �����������, ����������������� �� ������.

����� ��������� ��������� ���������� ����� � ��������� �� ����� � ���������� ������.

## 2. ���������

��������� ��������� ��� ���������� � ��������� ������ �� ������ � ����������� �������. �� ��������� �� ����� �������� ��� ��������� � ��������� ���. 

��� �������� ����� ������������ � ��������� ���������� ����� ����������� ���� `ip`, ������� �������� �� ����� ������� ����������.

��������� �������� ��������� ����������� ���������. ��� ���������� ������������ ���������� `REGISTERS_NUM` � ����� `src/spu.h`. 


## ������������� ����������

_������ ������ ���������� ��������� � ����� `commands.h`_

### ���������� ������ �� ������

���������� `push` � `pop` �������� �� ������. `push` ����� � ���� 4-������� �����, ���������� ���������� ��� ��������� ��������, ��� ������� � ���������� ����������� ������.

_����� `IMM` - ��������� ���������, `REG` - ��������� �������._

|   ����������      |  ��������
|-------------------|------------
|`push IMM`         | ������ � ���� IMM
|`push REG`         | ������ � ���� �������� REG
|`push [IMM]`       | ����� �� ������ �� ������ IMM �������� � ����� � ����
|`push [REG]`       | ����� �� ������ �� ������, �������� � REG, �������� � ����� � ����
|`push [REG + IMM]` | ����� �� ������ �� ������, ������� �������� REG + IMM, �������� � ����� � ����

� ���� ���������� ���������� ������� `push` ����� ��� 1. ����� �������� ������������������ ������� �����������, ������� ������ ��� ����������, ������� ���������� 6 ������:

| ����������      | Use RAM    | Use register | Use constant | ��� ���������� | 
|-----------------|------------|--------------|--------------|----------------|
| `push 5`        |      0     |     0        |       1      |  `001 001`     |
| `push AX`       |      0     |     1        |       0      |  `010 001`     |
| `push [5]`      |      1     |     0        |       1      |  `101 001`     |
| `push [AX]`     |      1     |     1        |       0      |  `110 001`     |
| `push [AX + 5]` |      1     |     1        |       1      |  `111 001`     |


### �������������� ��������

��� �������������� �������� �������� �� ������ ��������. ��� ����� ��������� �� �����, ��������� ��������� �������� � ��������� ��� � ����. ������ �����������, ��� ���������� ��� ������ ���������� ***\<op>***.

**�������� ��������:** 
```C
    StackElem_t arg1 = 0;
    StackElem_t arg2  = 0;

    Pop(&arg2);
    Pop(&arg1);

    Push(arg1 <op> arg2);
```

**�������:**
```C
    StackElem_t arg = 0;
    Pop(&arg);

    arg = <op>(arg);

    Push(arg);
```

### ����������� � �������� ��������

��� ������������ �������� �� ����� ������������ ������� `JMP`. ��� ������������ ���������� `ip` �������� ������ ���������.

���������� ��������� �������� �������� �� ����� ��� �������� � �������� �������, ����������, � ������ ������� � ����������� �� �������. ������������ ������� ������������ ���:

|   |       |
|---|-------|
`A` - ������
`B` - ������
`E` - �����

��������, ��������� ���������� JAE:
```C
    StkElmsCmpVal cmp = StkTwoLastElmsCmp(data_stk);    // �������� ��� �������� �����

    if (cmp == A || cmp == E)
        goto JUMP_MARK;                                 // ������� �������
    else
        cmd->ip += 2;                                   // ��� ����� ������
```



### ���������� ��� ������ � ���������

��� ������ � ��������� ������������ `CALL` � `RET`. 

`CALL` ����� � ���� ����� ��������� ���������� � ����������� ���������� `ip` �����, ��������� ����� �������. 

`RET` �������� �� ����� ������� ��� ����� �������� � ����������� `ip` ��� ��������. 

��������� ����� ��������� ������ �������, ����������� ��������� ������������ ��������� ������ ������� � ��������.

### ��������� �� ����������� ������

����������� ������ ������������ ����� ������ �����, ��� ������� ������������ ������� � ����� `stack/leb/stack.h`:

```C
typedef int StackElem_t;
```

� ���� ��������� ����������� ������ ������������ � ���� �����������, ��� ������ ������ ������ �������� �� ��, ����� �� �������� ������� ��� ���. ������� ������ ������������ ����������� `RAM_SIZE_X` � `RAM_SIZE_Y` � ����� `src/spu.h`. 

## ������������� ���������

��� ������������ ������ ���������� � ���������� ������ ��������� - ������ ������� ����������� ���������, ����������� ������ ���������� �����, � ���������, ������������ ����������� - ��������� ���������� � ���� "�����".

<details>
<summary>������ ������������ ������� ����������</summary>

```asm
SPU_IN      ;������� ����� �� �������
POP BX      ;�������� ��� � BX

PUSH 1
POP CX      ;������� = 1

PUSH 1      
CALL Factorial:

SPU_OUT     ;���������� ���������
HLT         ;����� ���������

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
```

</details>

��� ��������� ������������� ����������� ������������� ������������ ������ ������� � ������� �������� ������ ����������. 


## ��������� ����������

<div style="width: 50%; margin: 0 auto;">
  <img src="readme/draw_circle.png" alt="��������" style="width: 100%;">
</div>

��� ��������� �������� ������ �� ���� ��������, ����������� ���������� �� ������� �� ������. ���� ���������� ������ ��� ����� �������, ������� ������������ ������.

��������, ��� ������� �� ����, ������� ������������ ���������� �� ������ � �������, ������� � `BX`, �� ������:

<details>
<summary>�������� ����� build/txts/run_circle.asm</summary>

```asm
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
```

</details>


## ���� "�����"

��� ��������� ��������� ������� ��������� ���� "�����". ��� ��������� ������ ������������ ����������� ���������� `TXLib`, �� ��� ������ ������� ������������ ������. 

### �������

* � ������ (������) ������, � ������� ����������� ��� ����� ������, ����������� �����
* ���� � ����� ������ ���� ��� ��� ��� ����� �������, �� ��� ������ ���������� ����
* � ��������� ������ (���� ����� ������� ������ ���� ��� ������ ���) ������ ������� (��� ����������� ��� ��� ���������������).

��������� ����������� ������ �� ���� �������, � ��� ������ ������� ���������� ������� (���� �������� ������������ ����). � ���������� ���������� ����, �������� ������ ��� ���. Ÿ ����� ��������� ������������ � ������� ������� ������ - ����� ��������. ��� ����������, �.�. ���� ����� ����� ��� �������� ����� � ������, �� ��� �������� ������ � ������ ���� ����������� ������������ ���������� �������.

�� ��������� ����� � �������� ������ ���������� �������, � ���������� ��������� �����.  

<div style="width: 80%; margin: 0 auto;">
  <img src="readme/life.png" alt="��������" style="width: 100%;">
</div>

� ���� ����� ������� ��������� ������ - ����, �������, �����, �������� � ��������, ��� ��������������� � ���������� ���������. ��������� ��������� � `build/txts/life.asm`.


---
����, � ���������� ������ ��� ������� ����������� ���������, ������������ ������ ���������� � �������������� ������� ����� ����������, ����������� ��� ��������� ���������� �������������� �������� � ��������� ���.