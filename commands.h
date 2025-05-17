DEF_CMD_PP_ (PUSH,1,         
{                                   
    StackElem_t *arg = GetArg(&spu);
    StackPush(data_stk, *arg);      
})

DEF_CMD_PP_ (POP, 2,                             
{                                                       
    StackElem_t *arg = GetArg(&spu);                    
    StackPop(data_stk, arg);                            
})

DEF_CMD_JMP_ (CALL,3,
{
    StackPush(func_stk, (StackElem_t)(cmd->ip + 2));
    
// PUSH (ARG (+2))    
    
    goto JUMP_MARK;
})

DEF_CMD_JMP_ (JMP, 4,
{
    JUMP_MARK:
    cmd->ip = cmd->code[cmd->ip + 1];   
})

DEF_CMD_JMP_ (JA,  5,
{
    if (StkTwoLastElmsCmp(data_stk) == A)
        goto JUMP_MARK;
    else
        cmd->ip += 2;
})

DEF_CMD_JMP_ (JAE, 6,
{
    StkElmsCmpVal cmp = StkTwoLastElmsCmp(data_stk);

    if (cmp == A || cmp == E)
        goto JUMP_MARK;
    else
        cmd->ip += 2;
})

DEF_CMD_JMP_ (JB,  7,
{
    StkElmsCmpVal cmp = StkTwoLastElmsCmp(data_stk);

    if (cmp == B)
        goto JUMP_MARK;
    else
        cmd->ip += 2;
})

DEF_CMD_JMP_ (JBE, 8,
{
    StkElmsCmpVal cmp = StkTwoLastElmsCmp(data_stk);

    if (cmp == B || cmp == E)
        goto JUMP_MARK;
    else
        cmd->ip += 2;
})

DEF_CMD_JMP_ (JE,  9,
{
    if (StkTwoLastElmsCmp(data_stk) == E)
        goto JUMP_MARK;
    else
        cmd->ip += 2;
})

DEF_CMD_JMP_ (JNE, 10,
{
    if (StkTwoLastElmsCmp(data_stk) != E)
        goto JUMP_MARK;
    else
        cmd->ip += 2;
})

DEF_CMD_ (RET, 11,
{
    StackElem_t tmp = (StackElem_t)cmd->ip;
    StackPop(func_stk, &tmp);
    cmd->ip = (size_t) tmp; 
})

DEF_CMD_ (ADD, 12,
{
    StackElem_t a = 0;
    StackElem_t b = 0; 
    
    StackPop(data_stk, &a);
    StackPop(data_stk, &b);

    StackPush(data_stk, a + b); 
    cmd->ip++;
})

DEF_CMD_ (SUB, 13,
{
    StackElem_t a = 0;
    StackElem_t b = 0; 
    
    StackPop(data_stk, &b);
    StackPop(data_stk, &a);

    StackPush(data_stk, a - b); 
    cmd->ip++;
})

DEF_CMD_ (MUL, 14,
{
    StackElem_t a = 0;
    StackElem_t b = 0;

    StackPop(data_stk, &a);
    StackPop(data_stk, &b);

    StackPush(data_stk, a * b);
    cmd->ip++;
})

DEF_CMD_ (DIV, 15,
{
    StackElem_t divisible = 0;
    StackElem_t splitter  = 0;

    StackPop(data_stk, &splitter);
    StackPop(data_stk, &divisible);

    StackPush(data_stk, divisible / splitter);
    cmd->ip++;
})

DEF_CMD_ (SQRT, 16,
{
    StackElem_t elem = 0;
    StackPop(data_stk, &elem);

    elem = (StackElem_t) sqrt(elem);

    StackPush(data_stk, elem);

    cmd->ip++;
})

DEF_CMD_ (MOD, 17,
{
    StackElem_t divisible = 0;
    StackElem_t splitter  = 0;

    StackPop(data_stk, &splitter);
    StackPop(data_stk, &divisible);

    StackPush(data_stk, divisible % splitter);
    cmd->ip++;
})

DEF_CMD_ (SIN, 18,
{
    StackElem_t var = 0;

    StackPop(data_stk, &var);

    StackPush(data_stk, (StackElem_t) sin(var));
    cmd->ip++;
})

DEF_CMD_ (COS, 19,
{
    StackElem_t var = 0;

    StackPop(data_stk, &var);

    StackPush(data_stk, (StackElem_t) cos(var));
    cmd->ip++;
})

DEF_CMD_ (TG, 20,
{
    StackElem_t var = 0;

    StackPop(data_stk, &var);

    StackPush(data_stk, (StackElem_t) tan(var));
    cmd->ip++;
})

DEF_CMD_ (CTG, 21,
{
    StackElem_t var = 0;

    StackPop(data_stk, &var);

    StackPush(data_stk, (StackElem_t) (1 / tan(var)));
    cmd->ip++;
})

DEF_CMD_ (CRTWND, 22,
{
    SpuCreateWindow();
    cmd->ip++;

    SPU_ASSERT(&spu);
})

DEF_CMD_ (DRAW, 23,
{
    // DrawInWindow();
    DrawInConsole();

    cmd->ip++;
})

DEF_CMD_ (SETRNDRAM, 24,
{
    SetRandomRam();
    cmd->ip++;
})

DEF_CMD_ (DUMP, 25,
{
    SPU_DUMP(&spu);
    cmd->ip++;
})

DEF_CMD_ (SPU_IN, 26,
{
    int read_val = 0;
    scanf("%d", &read_val);
    StackPush(data_stk, (StackElem_t) read_val);
    cmd->ip++;
})
     
DEF_CMD_ (SPU_OUT, 0,
{
    StackElem_t res = 0;
    StackPop(data_stk, &res);
    fprintf(stderr, "res = %d\n", res);
    cmd->ip++;
})

DEF_CMD_ (HLT, 666,
{  
    cmd->ip++;

    keep_doing = false;
    printf("END\n");
})
