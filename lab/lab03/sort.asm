;对二进制补码表示的整数链表进行升序排序
;采用选择排序，每次都从未排序的数值中选择最小值min，并放到前面
;R0指向当前要存放min的位置
;R7用于比较和存放min值
;R1指向当前访问结点的地址
;R2存放当前结点的数值
        .ORIG x3000
;R0存放第一个结点的地址，这里为x3100,需要手动输入
UPDATER LDR R7,R0,#1     
UPDATEA LDR R1,R0,#0     ;R1存放R0下一个结点的地址
        BRz FINISH       ;R0已经是最后一个结点
UPDATED LDR R2,R1,#1     ;R2存放R1所指结点的数值
;比较R7与当前访问结点数值的大小
        NOT R3,R2
        ADD R3,R3,#1
        ADD R3,R7,R3     ;R7-R2
        BRnz NEXT
        ADD R7,R2,#0     ;R2<R7时,更新R7
        ADD R4,R1,#0     ;更新R7时也把更新值的结点地址记录下来
;访问下一个结点
NEXT    LDR R1,R1,#0
        BRnp UPDATED
;如果不跳转，说明R1指向了x0000，这时已经把未排序数值中的最小值求出在R7
        LDR R5,R0,#1     ;把原先存放在R0结点的值取出
        STR R7,R0,#1     ;把求得的min值放进R0结点的值域
        STR R5,R4,#1     ;把取出的R0结点的值放进存有min值的结点中，即交换值域
        LDR R6,R0,#0     ;R6指向下一个要存放min值的结点
        BRz FINISH       ;当R6为x0000时，说明R0已经指向最后一个结点，排序完成
        ADD R0,R6,#0
        ADD R4,R0,#0     ;避免残留的R4对后面的排序造成影响
        BRnzp UPDATER
FINISH  HALT   
        .END
        