crtwnd

call CENTER_IP:
pop CX

NEW_CYCLE:
    call PushLUcoord:
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

        call PushCentIpX:
        call RADIUS:
        add

        push BX
        call RAM_SIZE_X:
        mod

        JA StayCurLine:
                push BX
                    call RADIUS:
                    push 2
                mul
            sub

            push 1
            sub
            
            call RAM_SIZE_X:
        add

        pop BX
        StayCurLine:

        push BX
        call PushRDcoord:
    JB CYCLE:

    draw
    call ClearRAM:
    
    push CX
    push 5
    add
    pop CX
    
    push CX
    call RAM_CAPA:
JB NEW_CYCLE:

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

        push AX
        call RAM_CAPA:
    JB ClearNextPixel:
RET


    CENTER_IP:
push 210
RET

PushCentIpX:
        call CENTER_IP:
        call RAM_SIZE_X:
    mod
RET

    RADIUS: 
push 4 
RET

    RAM_CAPA:
push 400
RET

    RAM_SIZE_X:
push 20
RET

PushLUcoord:
            push CX
            call RADIUS:
        sub

            call RAM_SIZE_X:
            call RADIUS:
        mul
    sub
    pop DX

    push DX
    push 0

    JB RET_NULL:

    push DX
    jump FUNC1_RET:

    RET_NULL:
    push 0

    FUNC1_RET:
RET

PushRDcoord:
            push CX
            call RADIUS:
        add

            call RAM_SIZE_X:
            call RADIUS:
        mul
    add
    pop DX

    push DX

    call RAM_CAPA:
    
    JAE RETURN_CAPA:

    push DX
    jump FUNC2_RET:

    RETURN_CAPA:
    call RAM_CAPA:

    FUNC2_RET:
RET