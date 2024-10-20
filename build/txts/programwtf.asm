crtwnd

call CENTER_IP:
pop CX

NEW_CYCLE:
    push 0
    pop BX

    CYCLE:
        call PushCenterDist:
        call RADIUS:

       JA SKIP:
        push 1
        pop [BX]

       SKIP:
        push BX
        push 1
        add
        pop BX

        push BX
        call RAM_CAPA:
    JB CYCLE:

    draw
    call ClearRAM:
    
    push CX
    push 1
    add
    pop CX
    
    push CX
    call RAM_CAPA:
JB NEW_CYCLE:


push 999
out
hlt

PushCenterDist:
                push BX
                call RAM_SIZE_X:
                mod

                push CX
                call RAM_SIZE_X:
                mod
            sub

                push BX
                call RAM_SIZE_X:
                mod

                push CX
                call RAM_SIZE_X:
                mod
            sub
        mul
        

                push BX
                call RAM_SIZE_X:
                div

                push CX
                call RAM_SIZE_X:
                div
            sub

                push BX
                call RAM_SIZE_X:
                div

                push CX
                call RAM_SIZE_X:
                div
            sub
        mul
    add

    sqrt
RET

ClearRAM:
    push 0
    pop AX

    ClearNextPixel:
        push 0
        pop [AX]

        push AX
        push 1
        add
        pop AX

    call RAM_CAPA:
    out

        push AX
        call RAM_CAPA:

    JB ClearNextPixel:

    push 444
    out
RET


    CENTER_IP:
push 3333
RET

    RADIUS: 
push 35 
RET

    RAM_SIZE_X:
push 80
RET

    RAM_CAPA:
push 6400
RET