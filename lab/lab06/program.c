//lab01 - lab05 implemented in C
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<conio.h>

#define Row_Num 3
#define word_len 17   //用char数组存16个0/1时需要额外的一个空间存'\0'

#define Status int
#define OK 1
#define ERROR 0

//lab03需要用到的结点结构
typedef struct CPMNode{
    char CPM_int[word_len];
    struct CPMNode *NEXT;
}CPMNode;

Status lab01(char word[word_len]){
    char temp;
    int rotamount;
    printf("\nInput rotate amount: ");
    scanf("%d",&rotamount);
    //rotation
    for(int j=0;j<rotamount;++j){
        temp = word[0];
        for(int i=0;i<word_len-2;++i) word[i] = word[i+1];
        word[word_len-2] = temp;
    }
    printf("\nAfter rotation: ");
    printf("%s",word);
    printf("\n");
    return OK;
}

Status lab02(char word1[word_len],char word2[word_len]){
    //输入两个16 bit二进制补码表示的正整数，求出它们的最大公约数并打印显示
    char gcd[word_len];   //最大公约数的16 bit二进制补码表示 
    int factor1=0,factor2=0;
    //将char表示的二进制数转化成int十进制数运算
    for(int i=0;i<word_len-1;++i){
        if(word1[i]!='0') factor1+=pow(2,15-i);
        if(word2[i]!='0') factor2+=pow(2,15-i);
    }
    int temp1=factor1, temp2=factor2;
    //辗转相减
    while(factor2!=factor1){
        if(factor1 > factor2) factor1 = factor1 - factor2;
        else factor2 = factor2 - factor1;
    }
    //将最大公约数转化从16 bit二进制补码表示
    for(int i=0;i<word_len-1;++i)
       if(factor1 >= pow(2,15-i)) {gcd[i] = '1'; factor1 = factor1 - pow(2,15-i);}
       else gcd[i] = '0';
    gcd[word_len-1] = '\0';
    int result = (factor1==0) ? factor2 : factor1;
    printf("The GCD of %s(%d) and %s(%d) is %s(%d)\n",word1,temp1,word2,temp2,gcd,result);
    return OK;
}

Status lab03(CPMNode *Head){
    srand(time(NULL));
    printf("\nAssign the length of the linked list: ");
    int list_len;
    scanf("%d",&list_len);
    CPMNode *p = NULL, *q = NULL, *r = NULL;
    int temp;
    //指定链表长度后，每个结点用随机生成的整数作为数据域的值来构造链表
    for(int i=0;i<list_len;++i){
        p = (CPMNode*)malloc(sizeof(CPMNode));
        if(!p) exit(0);
        p->NEXT = NULL;
        temp = rand() % 16383;
        for(int j=0;j<word_len-1;++j)
            if(temp > pow(2,15-j)) {p->CPM_int[j]='1';temp = temp -pow(2,15-j);}
            else p->CPM_int[j] = '0';
        p->CPM_int[word_len-1] = '\0';
        p->NEXT = Head;
        Head = p;
    }
    //选择排序
    p = Head;
    while(p){
        q=r=p;
        int min;
        temp=0;
        //将0-1串转换成int型
        for(int j=0;j<word_len-1;++j)
            if(p->CPM_int[j]=='1') temp += pow(2,15-j);
        min = temp;
        //选出未排序的最小值，用r指向该结点
        while(q){
            temp=0;
            for(int j=0;j<word_len-1;++j)
                if(q->CPM_int[j]=='1') temp += pow(2,15-j);
            if(temp < min) {min = temp;r=q;}
            q = q->NEXT;
        }
        //交换当前p和r所指的结点的数据域
        char exchange[word_len];
        for(int k=0;k<word_len-1;++k) exchange[k] = p->CPM_int[k];
        for(int k=0;k<word_len-1;++k) p->CPM_int[k] = r->CPM_int[k];
        for(int k=0;k<word_len-1;++k) r->CPM_int[k] = exchange[k];
        p = p->NEXT;
    }
    printf("The address and value of the list is as follows:\n");
    p = Head;
    while(p){
        printf("address:%d,value:%s,points to:%d\n",p,p->CPM_int,p->NEXT);
        p = p->NEXT;
    }
    return OK;
}

Status PrintRows(int *Row){
	int i,j;
    for(i=0;i<Row_Num;++i){
        switch(i){
            case 0: {printf("ROW A: ");break;}
            case 1: {printf("ROW B: ");break;}
            case 2: {printf("ROW C: ");break;}
        }
        for(j=0;j<Row[i];++j) printf("o ");
        printf("\n");
    }
    return OK;
}

int RemoveRocks(int Player, int *Row){
    int winner=0;
    if(Player>2 || Player<1) return ERROR;
    int i;
    for(int i=0;i<Row_Num;i++) 
        if(Row[i]!=0) break;
    if(i==Row_Num) return ERROR; 
    printf("Player%d choose a row and number of rocks: ",Player);   //提示玩家输入
    char row[2]; 
    scanf("%s",row);
    //判断输入是否有效
    while(row[0]<'A' || row[0]>'C' || row[1]<'1' || row[1]-48>Row[row[0]-65]){
          printf("Invalid. Try again.\n");
          scanf("%s",row);
    }
    Row[row[0]-65]=Row[row[0]-65]-(row[1]-48);   //修改石头数
    //判断游戏是否结束和结束时的赢家
    for(i=0;i<Row_Num;i++) 
        if(Row[i]!=0) break;
    if(i==Row_Num) {winner=Player==1? 2 : 1; printf("Player%d wins.",winner);}
    return winner;
}

Status lab04(int *Row){
    PrintRows(Row);
    int winner,player=1;
    winner = RemoveRocks(player,Row);
    while(!winner){
    	printf("winner=%d\n",winner);
        PrintRows(Row);
        player = (player==1)? 2:1;
        printf("player=%d\n",player);      
        winner=RemoveRocks(player,Row);
    }
}

Status lab05(char *str){
    char ch;
    while(1){
        printf("%s\n",str);
        for(int i=0;i<500000000;++i);
        //当从键盘输入字符时，中断当前循环输出程序，判断输入的字符是否十进制数字
        if(kbhit()){
            ch = getch();
            if(ch<'0' || ch>'9') printf("%c is not a decimal digit.\n",ch);
            else printf("%c is a decimal digit.\n",ch);
        }
    }
    return OK;
}

int main(){
    //test lab01
    //char word[word_len] = "1101000100001000";
    //lab01(word);
    //test lab02
    //char word1[word_len] = "0000000001100010";
    //char word2[word_len] = "0000000000111111";
    //lab02(word1,word2);
    //test lab03
    //struct CPMNode *Head=NULL;
    //lab03(Head);
    //test lab04
    //int Row[Row_Num];
    //Row[0]=3; Row[1]=5;Row[2]=8;
    //lab04(Row);
    //test lab05
    //char *str = "ICS2020 ";
    //lab05(str);
}