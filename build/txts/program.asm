crtwnd

call CENTER_IP:
pop CX

push 5
pop [CX]


push 0
pop BX
 
CYCLE:

call PushCenterDist:
call RADIUS:

JA NEXT:
push 1
pop [BX]

NEXT:
push BX
push 1
add
pop BX

push BX
call RAM_SIZE:

JB CYCLE:

draw

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



DRAW:
    draw
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

    RAM_SIZE_Y:
push 80
RET

    RAM_SIZE:
push 6400
RET