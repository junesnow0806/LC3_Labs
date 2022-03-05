;使用更相减损术求给定两个正整数的最大公约数
;给定的两个正整数储存在R0和R1
;最后求出的最大公约数存放在R0
;手动设置R0和R1的初始值
;
            .ORIG   x3000
UPDATE      ADD     R2,R0,#0     ;将R0、R1放在R2、R3中作相减和比较
            ADD     R3,R1,#0
;
;相减
;
            NOT     R3,R3
            ADD     R3,R3,#1
            ADD     R2,R2,R3     ;R2中存放相减的结果
;
;比较和选择跳转
;
            BRp     POS
            BRz     EQUAL
            NOT     R1,R2        ;R2-R3结果为负，则R1比R0大，将差值取为正后存进R1
            ADD     R1,R1,#1
            BRnzp   UPDATE
POS         ADD     R0,R2,#0     ;R2-R3结果为正，则R0比R1大，将差值存进R0
            BRnzp   UPDATE
EQUAL       HALT                 ;相等时已经求得最大公约数，结束
            .END
            