            .ORIG x3000
;Initialize the number of rocks
            AND   R0,R0,#0
            ADD   R0,R0,#3
            STI   R0,NumA
            AND   R0,R0,#0
            ADD   R0,R0,#5
            STI   R0,NumB
            AND   R0,R0,#0
            ADD   R0,R0,#8
            STI   R0,NumC
;main
            JSR   PrintState     ;调用 输出面板的子程序
            AND   R6,R6,#0       ;R6为0表示Player1，为1表示Player2
Loop        JSR   Play           ;调用 取石头的子程序
            JSR   PrintState
            JSR   IsOver         ;判断游戏是否结束
            AND   R5,R5,#1       ;R5为IsOver的返回值，为1时表示游戏结束
            BRp   Finish
            NOT   R6,R6          ;Player交替
            AND   R6,R6,#1
            BRnzp Loop 
;Subroutine PrintState
PrintState  ST    R1,SaveR1
            ST    R2,SaveR2
            ST    R3,SaveR3
            ST    R4,SaveR4
            ST    R5,SaveR5
            LD    R0,Newline
            TRAP  x21
;以下输出“ROW X:”
            LEA   R1,RowA       ;R1为row指针
            AND   R4,R4,#0      ;R4作计数器，同时可作为偏移量
PrintRow    LDR   R0,R1,#0      ;R0存输入/输出的字符
            BRz   LDASCII
            TRAP  x21
            ADD   R1,R1,#1
            BRnzp PrintRow
;以下输出每一行的'o'            
LDASCII     LEA   R0,ASCII_o    
            LDR   R0,R0,#0      ;R0存'o'的ASCII码
            LEA   R2,NumA
            ADD   R2,R2,R4      ;R2存 存有该行现有石头数的位置
            LDR   R3,R2,#0      
            LDR   R3,R3,#0      ;R3先存好Row A,B或C的石头数量
PrintRocks  BRz   RowNoRocks   
            TRAP  x21
            ADD   R3,R3,#-1     ;R3作输出石头数的计数器     
            BRp   PrintRocks
RowNoRocks  LD    R0,Newline    ;输出换行符
            TRAP  x21
            ADD   R4,R4,#1      ;R4作为计数器控制输出行
            ADD   R0,R4,#-3
            BRz   Return        ;R4=3时结束
            ADD   R1,R1,#1      ;R1存"ROW B:"或"ROW C:"字符串的首地址，要求A,B,C字符串存放在连续的空间
            BRnzp PrintRow
Return      LD    R1,SaveR1
            LD    R2,SaveR2
            LD    R3,SaveR3
            LD    R4,SaveR4
            LD    R5,SaveR5
            RET
RowA        .STRINGZ "ROW A:"
RowB        .STRINGZ "ROW B:"
RowC        .STRINGZ "ROW C:"
ASCII_o     .FILL x006F
;Subroutine IsOver
IsOver      ST    R1,SaveR1
            ST    R2,SaveR2
            ST    R6,SaveR6
            AND   R2,R2,#0      ;R2作访问 存放石头数的位置 时的计数器
            LEA   R1,NumA
            LDR   R1,R1,#0      ;R1存 存放行A的石头数的地址
LDRocks     LDR   R0,R1,#0      ;R0存石头数
            BRp   FALSE         ;R0为正，说明游戏未结束
            ADD   R1,R1,#1
            ADD   R2,R2,#1
            ADD   R0,R2,#-3
            BRz   TRUE          ;3行的石头数都是0，游戏结束
            BRnzp LDRocks
TRUE        AND   R5,R5,#0
            ADD   R5,R5,#1      ;R5置1
            LEA   R1,Result1
            LD    R0,Length2
            AND   R6,R6,#1
            BRp   PrintWinner   ;R6为1时表示取石头的是Player2，则Player1胜
            ADD   R1,R1,R0      ;R6为0时Player2胜，R0为偏移量，R1指向2胜的字符串
PrintWinner LDR   R0,R1,#0      ;打印胜者信息
            BRz   Return2
            TRAP  x21
            ADD   R1,R1,#1
            BRnzp PrintWinner
FALSE       AND   R5,R5,#0
Return2     LD    R1,SaveR1
            LD    R2,SaveR2
            LD    R6,SaveR6
            RET
NumA        .FILL x9001
NumB        .FILL x9002
NumC        .FILL x9003
SaveR1      .FILL x0000
SaveR2      .FILL x0000
SaveR3      .FILL x0000
SaveR4      .FILL x0000
SaveR5      .FILL x0000
SaveR6      .FILL x0000
Newline     .FILL x000A
Result1     .STRINGZ "Player1 wins."   ;长14
Result2     .STRINGZ "Player2 wins."
Length1     .FILL x002A
Length2     .FILL x000E
;Print prompt for players,R1 for which player to tell
Play        ST    R1,SaveR1    
            ST    R2,SaveR2
            ST    R3,SaveR3
            ST    R4,SaveR4
            ST    R5,SaveR5
            ST    R6,SaveR6
            LEA   R1,Player1   ;R1先载入Player1字符串首地址
            LD    R0,Length1   ;R0载入偏移量
            AND   R6,R6,#1
            ST    R1,InnerSave ;后面的操作会改变R1，需先另存
            BRz   PrintPrompt  ;R6=0时是Player1，R1不需要偏移
            ADD   R1,R1,R0     ;R6=1时是Player2，加偏移量后R1指向Player2
            ST    R1,InnerSave
PrintPrompt LDR   R0,R1,#0     
            BRz   PlayerIn1
            TRAP  x21
            ADD   R1,R1,#1
            BRnzp PrintPrompt
PlayerIn1   TRAP  x20          ;输入A,B或C进R0
Echo1       TRAP  x21
            ADD   R2,R0,#0     ;R2存所输入的row的ASCII码
            LD    R3,ASCII_A
            ADD   R3,R3,R2     ;R3暂存寻址用的偏移量
            LEA   R5,NumA
            ADD   R5,R5,R3     ;R5存NumA,B或C的地址
            LDR   R5,R5,#0     ;R5=x9001,x9002或x9003
            LDR   R4,R5,#0     ;R4存该行现有石头数
PlayerIn2   TRAP  x20          ;输入要取走的石头数目的ASCII码
Echo2       TRAP  x21
            LD    R3,ASCII_N
            ADD   R3,R0,R3     ;R3存输入的石头数的数值
            BRnz  Error        ;石头数小于1，无效
            LD    R0,ASCII_A
            ADD   R0,R2,R0     ;R2跟A的ASCII码比较
            BRn   Error        ;输入字母小于A，无效
            LD    R0,ASCII_C
            ADD   R0,R2,R0     ;R2跟C的ASCII码比较
            BRp   Error        ;输入字母大于C，无效
            NOT   R3,R3
            ADD   R3,R3,#1
            ADD   R4,R3,R4     ;现有石头数跟要取走的石头数比较
            BRn   Error        ;要取走大于现有，无效
            STR   R4,R5,#0     ;更新石头数目
            LD    R1,SaveR1
            LD    R2,SaveR2
            LD    R3,SaveR3
            LD    R4,SaveR4
            LD    R5,SaveR5
            LD    R6,SaveR6
            RET
Error       LD    R0,Newline
            TRAP  x21
            LEA   R1,Invalid
Errorprompt LDR   R0,R1,#0     ;输出无效提示符
            BRz   RecoverR1
            TRAP  x21
            ADD   R1,R1,#1
            BRnzp Errorprompt
RecoverR1   LD    R0,Newline   
            TRAP  x21
            LD    R1,InnerSave ;恢复R1，以重新打印Player1/2字符串
            BRnzp PrintPrompt
;
Finish      HALT
InnerSave   .FILL x0000
ASCII       .FILL x0030 
ASCII_N     .FILL xFFD0   ;-x0030
ASCII_A     .FILL xFFBF   ;-65,-X0041
ASCII_C     .FILL xFFBD   ;-67,-X0043
Player1     .STRINGZ "Player1,choose a row and number of rocks:" 
Player2     .STRINGZ "Player2,choose a row and number of rocks:"   ;长42
Invalid     .STRINGZ "Invalid.Try again."
            .END