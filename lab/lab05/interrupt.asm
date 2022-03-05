; Unfortunately we have not YET installed Windows or Linux on the LC-3,
; so we are going to have to write some operating system code to enable
; keyboard interrupts. The OS code does three things:
;
;    (1) Initializes the interrupt vector table with the starting
;        address of the interrupt service routine. The keyboard
;        interrupt vector is x80 and the interrupt vector table begins
;        at memory location x0100. The keyboard interrupt service routine
;        begins at x1000. Therefore, we must initialize memory location
;        x0180 with the value x1000.
;    (2) Sets bit 14 of the KBSR to enable interrupts.
;    (3) Pushes a PSR and PC to the system stack so that it can jump
;        to the user program at x3000 using an RTI instruction.

        .ORIG x800
        ; (1) Initialize interrupt vector table.
        LD R0, VEC
        LD R1, ISR
        STR R1, R0, #0

        ; (2) Set bit 14 of KBSR.
        LDI R0, KBSR
        LD R1, MASK
        NOT R1, R1
        AND R0, R0, R1
        NOT R1, R1
        ADD R0, R0, R1
        STI R0, KBSR

        ; (3) Set up system stack to enter user space.
        LD R0, PSR
        ADD R6, R6, #-1
        STR R0, R6, #0
        LD R0, PC
        ADD R6, R6, #-1
        STR R0, R6, #0
        ; Enter user space.
        RTI
        
VEC     .FILL x0180
ISR     .FILL x1000
KBSR    .FILL xFE00
MASK    .FILL x4000
PSR     .FILL x8002
PC      .FILL x3000
        .END

        .ORIG x3000
        ; *** Begin user program code here ***
LDSTR   LEA    R1,STRING
Print   LDR    R0,R1,#0     ;输出"ICS2020 "
        BRz    CALL         ;输出完一个字符串后调用计数程序
        TRAP   x21
        ADD    R1,R1,#1
        BRnzp  Print
CALL    JSR    DELAY
        BRnzp  LDSTR
DELAY   ST     R1, SaveR1
        LD     R1, COUNT
REP     ADD    R1, R1, #-1
        BRp    REP
        LD     R1, SaveR1
        RET
COUNT   .FILL x7FFF
STRING  .STRINGZ "ICS2020 "
SaveR1  .BLKW #1
        ; *** End user program code here ***
        .END

        .ORIG x1000
        ; *** Begin interrupt service routine code here ***
        ST     R0,SaveR0
        ST     R1,SaveR1_2
        ST     R2,SaveR2
        ST     R3,SaveR3
        TRAP   x20         ;从键盘读入一个字符到R0
        TRAP   x21         ;Echo
        LD     R2,ASCII0   ;输入的字符与'0'比较
        ADD    R2,R0,R2
        BRn    FALSE
        LD     R2,ASCII9   ;输入的字符与'9'比较
        ADD    R2,R0,R2
        BRp    FALSE
TRUE    ADD    R3,R0,#0    ;由于要输出换行符，先将该字符存到R3
        LD     R0,NEWLINE  
        TRAP   x21
        ADD    R0,R3,#0    ;将该十进制数字字符放回R0后输出
        TRAP   x21
        LEA    R1,POS      ;紧接着输出肯定的字符串
Print1  LDR    R0,R1,#0
        BRz    FINISH
        TRAP   x21
        ADD    R1,R1,#1
        BRnzp  Print1
FALSE   ADD    R3,R0,#0    ;由于要输出换行符，先将该字符存到R3
        LD     R0,NEWLINE
        TRAP   x21
        ADD    R0,R3,#0    ;将该十进制数字字符放回R0后输出
        TRAP   x21
        LEA    R1,NEG      ;紧接着输出否定的字符串
Print2  LDR    R0,R1,#0
        BRz    FINISH
        TRAP   x21
        ADD    R1,R1,#1
        BRnzp  Print2
FINISH  LD     R0,NEWLINE
        TRAP   x21
        LD     R0,SaveR0
        LD     R1,SaveR1_2
        LD     R2,SaveR2
        LD     R3,SaveR3
        RTI
SaveR0  .BLKW #1
SaveR1_2  .BLKW #1
SaveR2  .BLKW #1
SaveR3  .BLKW #1
NEG     .STRINGZ " is not a decimal digit."
POS     .STRINGZ " is a decimal digit."
ASCII0  .FILL xFFD0   ;-x0030 '0'
ASCII9  .FILL xFFC7   ;-x0039 '9'
NEWLINE .FILL x000A
        ; *** End interrupt service routine code here ***
        .END