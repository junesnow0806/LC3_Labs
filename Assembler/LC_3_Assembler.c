//LC-3 Assembler
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#define Status int
#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0

#define row 50
#define MAX_NUM 50
#define key_amount 27
#define key_len 9

char label[row][MAX_NUM];
char key[key_amount][key_len];

Status CreateKeylist(char ke[key_amount][key_len]){
    char temp[key_amount][key_len]={".ORIG","ADD","AND","BR","JMP","RET","JSR","JSRR","LD","LDI","LDR","LEA","NOT","RTI","ST","STI","STR","TRAP",".END","HALT","BRn","BRz","BRp","BRnz","BRnp","BRzp","BRnzp"};
    for(int i=0;i<key_amount;++i) strcpy(ke[i],temp[i]);
    return OK;
}

Status CreateLabellist(char labe[row][MAX_NUM],char in[row][MAX_NUM],char ke[key_amount][key_len]){
    int i,j,k;
    for(i=0;i<row;++i)
        for(j=0;j<MAX_NUM;++j)
            labe[i][j]='\0';
    CreateKeylist(ke);
    for(i=0;i<row;++i){
        for(j=0,k=0;j<MAX_NUM && in[i][j]!='\0';++j){
            if(in[i][j]==';') break;
            if(in[i][j]==' ') continue;
            if((in[i][j]<='Z' && in[i][j]>='A')||(in[i][j]<='z' && in[i][j]>='a')||(in[i][j]<='9' && in[i][j]>='0')||in[i][j]=='_'||in[i][j]=='.'){
                labe[i][k++]=in[i][j];
                if(in[i][j+1]==' ') break;
            }
        }
    }
    //如果该符号是关键字，则去除
    for(i=0;i<row;++i){
        for(j=0;j<key_amount;++j)
            if(strcmp(labe[i],ke[j])==0){
                for(k=0;labe[i][k]!='\0';++k) labe[i][k]='\0'; 
            }
    }
    return OK;
}

//给定一个0-1字符串，将其转换成补码表示
Status complement(char bin[],int bit_num){
    //if(bin[0]='0') return OK;
    char c[bit_num+1];
    c[bit_num]='\0';
    for(int i=0;i<bit_num;++i){
        c[i]='0';
        //printf("%c",bin[i]);
        if(bin[i]=='1') bin[i]='0';
        else bin[i]='1';
    }
    c[bit_num-1]='1';
    for(int i=bit_num-1;i>=0;--i){
        if(c[i]=='1' && bin[i]=='1'){
            bin[i] = '0';
            if(i>0) c[i-1] = '1';
        }
        else if(c[i]=='1' && bin[i]=='0') bin[i]='1';
    }
    return OK;
}

//给定一个十进制数和指定bit数，输出二进制表示
Status PrintComplement(int R, int bit_num){
    char binary[bit_num+1];
    binary[bit_num]='\0';
    binary[0]='0';
    if(R>=0){
        for(int i=1;i<bit_num;++i){
            if(R>=pow(2,bit_num-i-1)){
               binary[i]='1';
               R = R-pow(2,bit_num-i-1);
			}
            else binary[i]='0';
        }
    }
    else{
        R = -R;
        for(int i=1;i<bit_num;++i){
            if(R>=pow(2,bit_num-i-1)){
               binary[i]='1';
               R = R-pow(2,bit_num-i-1);
			}
            else binary[i]='0';
        }
        complement(binary, bit_num);
    }
    printf(" %s ",binary);
    return OK; 
}

//输入寄存器的编号，输出该寄存器的3-bit二进制表示
Status PrintReg(int R){
    if(R==0) printf(" 000 ");
    else if(R==1) printf(" 001");
    else if(R==2) printf(" 010");
    else if(R==3) printf(" 011");
    else if(R==4) printf(" 100");
    else if(R==5) printf(" 101");
    else if(R==6) printf(" 110");
    else if(R==7) printf(" 111");
    else return ERROR;
    return OK;
}

Status ADD(char in[row][MAX_NUM], int number, int r){
    if(number!=1){
        printf("The instruction is not ADD.\n");
        return ERROR;
    }
    Status DR_visited=0,SR1_visited=0,SR2_visited=0,imm=0;
    int DR,SR1,SR2,imm5=0;
    for(int j=0;j<MAX_NUM && in[r][j]!='\0';++j){
        //if(in[r][j]==' ' | in[r][j]=='A' | in[r][j]=='D' | in[r][j]==',') continue;
        if(in[r][j]=='R' && (in[r][j-1]==' ' || in[r][j-1]==',')){
            if(!DR_visited){
                DR_visited = TRUE;
                DR = in[r][++j]-'0';
                if(DR<0 || DR>7) {printf("Error: R%d invalid.\n",DR);return ERROR;}
            }
            else if(!SR1_visited){
                SR1_visited = TRUE;
                SR1 = in[r][++j]-'0';
                if(SR1<0 || SR1>7) {printf("Error: R%d invalid.\n",SR1);return ERROR;}
            }
            else if(!SR2_visited){
                SR2_visited = TRUE;
                SR2 = in[r][++j]-'0';
                if(SR2<0 || SR2>7) {printf("Error: R%d invalid.\n",SR2);return ERROR;}
            }
        }
        if(in[r][j]=='#'){
            imm = TRUE;
            int i=0,k,minus=FALSE;
            ++j;
            if(in[r][j]=='-'){
                ++j;
                minus = TRUE;
            }
            while(in[r][j]<='9'&&in[r][j]>='0'){
                ++i; ++j;
            }
            for(k=j-i;i>=1;--i,++k) imm5=imm5+(in[r][k]-'0')*pow(10,i-1);
            if(minus) imm5 = -imm5;
        }
        else continue;
    }
    if(imm && (imm5>15 || imm5<-16)){
        printf("Error: the imm5 is not among -16~15.\n");
        return ERROR;
    }
    printf(" 0001");
    if(DR_visited) PrintReg(DR);
    if(SR1_visited) PrintReg(SR1);
    if(imm){
        printf(" 1");
        //printf("\nimm5=%d",imm5);
        PrintComplement(imm5,5);
    }
    else if(SR2_visited){
        printf(" 0 00");
        PrintReg(SR2);
    }
    printf("\n");
    return OK;
}

Status AND(char in[row][MAX_NUM], int number, int r){
    if(number!=5){
        printf("The instruction is not AND.\n");
        return ERROR;
    }
    Status DR_visited=0,SR1_visited=0,SR2_visited=0,imm=0;
    int DR,SR1,SR2,imm5=0;
    for(int j=0;j<MAX_NUM && in[r][j]!='\0';++j){
        //if(in[r][j]==' ' | in[r][j]=='A' | in[r][j]=='D' | in[r][j]==',') continue;
        if(in[r][j]=='R' && (in[r][j-1]==' ' || in[r][j-1]==',')){
            if(!DR_visited){
                DR_visited = TRUE;
                DR = in[r][++j]-'0';
                if(DR<0 || DR>7) {printf("Error: R%d invalid.\n",DR);return ERROR;}
            }
            else if(!SR1_visited){
                SR1_visited = TRUE;
                SR1 = in[r][++j]-'0';
                if(SR1<0 || SR1>7) {printf("Error: R%d invalid.\n",SR1);return ERROR;}
            }
            else if(!SR2_visited){
                SR2_visited = TRUE;
                SR2 = in[r][++j]-'0';
                if(SR2<0 || SR2>7) {printf("Error: R%d invalid.\n",SR2);return ERROR;}
            }
        }
        if(in[r][j]=='#'){
            imm = TRUE;
            int i=0,k,minus=FALSE;
            ++j;
            if(in[r][j]=='-'){
                ++j;
                minus = TRUE;
            }
            while(in[r][j]<='9'&&in[r][j]>='0'){
                ++i; ++j;
            }
            for(k=j-i;i>=1;--i,++k) imm5=imm5+(in[r][k]-'0')*pow(10,i-1);
            if(minus) imm5 = -imm5;
        }
        else continue;
    }
    if(imm && (imm5>15 || imm5<-16)){
        printf("Error: the imm5 is not among -16~15.\n");
        return ERROR;
    }
    printf(" 0101");
    if(DR_visited) PrintReg(DR);
    if(SR1_visited) PrintReg(SR1);
    if(imm){
        printf(" 1");
        //printf("\nimm5=%d",imm5);
        PrintComplement(imm5,5);
    }
    else if(SR2_visited){
        printf(" 0 00");
        PrintReg(SR2);
    }
    printf("\n");
    return OK;
}

Status JMP(char in[row][MAX_NUM],int number,int r){
    if(number!=12){
        printf("This instruction is not JMP.\n");
        return ERROR;
    }
    int BaseR;
    for(int j=0;in[r][j]!='\0';++j){
        if(in[r][j]=='J' && in[r][j+1]=='M' && in[r][j+2]=='P') j=j+3;
        if(in[r][j]=='R' && (in[r][j+1]<='9' && in[r][j+1]>='0')){
            BaseR = in[r][++j] - '0';
            if(BaseR>7 || BaseR<0){printf("Error: R%d invalid.\n",BaseR);return ERROR;}
            break;
        }
        if(in[r][j]=='R' && in[r][j+1]=='E' && in[r][j+2]=='T') BaseR = 7;
    }
    printf(" 1100 000");
    PrintReg(BaseR);
    printf(" 000000\n");
    return OK;
}

Status JSR(char in[row][MAX_NUM],int number,int r,char labe[row][MAX_NUM]){
    if(number!=4){
        printf("This instruction is not JSR.\n");
        return ERROR;
    }
    Status offset_mode=FALSE;
    int label_row,PCoffset11,BaseR;
    char temp[MAX_NUM]="";
    for(int j=0;j<MAX_NUM && in[r][j]!='\0';++j){
        if(in[r][j]=='J' && in[r][j+1]=='S' && in[r][j+2]=='R' && in[r][j+3]=='R') j=j+4;
        if(in[r][j]=='J' && in[r][j+1]=='S' && in[r][j+2]=='R' && in[r][j+3]!='R') {j=j+4;offset_mode=TRUE;}
        if(offset_mode && (in[r][j]<='Z' && in[r][j]>='A')||(in[r][j]<='z' && in[r][j]>='a')||(in[r][j]<='9' && in[r][j]>='0')||in[r][j]=='_'){
            for(int k=0;in[r][j]!=' ' && in[r][j]!='\0';++k,++j) temp[k]=in[r][j];
            break;
        }
        if(!offset_mode && in[r][j]=='R' && in[r][j+1]<='9' && in[r][j+1]>='0'){
            BaseR = in[r][++j]-'0';
            if(BaseR<0 || BaseR>7) {printf("Error: R%d invalid.\n",BaseR);return ERROR;}
            break;
        } 
    }
    printf(" 0100");
    if(offset_mode){
        int i;
        for(i=0;i<row;++i)
            if(!strcmp(labe[i],temp)) {label_row = i; break;}
        if(i==row) {printf("Error: Label %s not found!\n",temp); return ERROR;}
        PCoffset11 = label_row-(r+1);      //万一r的下一行是注释怎么办？
        if(PCoffset11>1023 || PCoffset11<-1024){printf("Error: PCoffset invalid.\n");return ERROR;}
        printf(" 1");
        PrintComplement(PCoffset11,11);
    }
    else{
        printf(" 000");
        PrintReg(BaseR);
        printf(" 000000");
    }
    printf("\n");
    return OK;
}

Status LDR(char in[row][MAX_NUM],int number,int r){
    if(number!=6){
        printf("This instruction is not LDR.\n");
        return ERROR;
    }
    Status DR_visited=FALSE,BaseR_visited=FALSE;
    int DR,BaseR,offset6=0;
    for(int j=0;j<MAX_NUM && in[r][j]!='\0';++j){
        //if(in[r][j]==' ' | in[r][j]=='A' | in[r][j]=='D' | in[r][j]==',') continue;
        if(in[r][j]=='R'&&(in[r][j+1]<='9'&&in[r][j+1]>='0')){
            if(!DR_visited){
                DR_visited = TRUE;
                DR = in[r][++j]-'0';
                if(DR<0 || DR>7) {printf("Error: R%d invalid.\n",DR);return ERROR;}
            }
            else if(!BaseR_visited){
                BaseR_visited = TRUE;
                BaseR = in[r][++j]-'0';
                if(BaseR<0 || BaseR>7) {printf("Error: R%d invalid.\n",BaseR);return ERROR;}
            }
        }
        if(in[r][j]=='#'){
            int i=0,k,minus=FALSE;
            ++j;
            if(in[r][j]=='-'){
                ++j;
                minus = TRUE;
            }
            while(in[r][j]<='9'&&in[r][j]>='0'){
                ++i; ++j;
            }
            for(k=j-i;i>=1;--i,++k) offset6=offset6+(in[r][k]-'0')*pow(10,i-1);
            if(minus) offset6 = -offset6;
            if(offset6<-32 || offset6>31){printf("Error: offset invalid.\n");return ERROR;}
        }
        else continue;
    }
    printf(" 0110");
    if(DR_visited) PrintReg(DR);
    if(BaseR_visited) PrintReg(BaseR);
    PrintComplement(offset6,6);
    printf("\n");
    return OK;
}

Status NOT(char in[row][MAX_NUM],int number,int r){
    if(number!=9){
        printf("This instruction is not NOT.\n");
        return ERROR;
    }
    Status DR_visited=FALSE,SR_visited=FALSE;
    int DR,SR;
    for(int j=0;j<MAX_NUM && in[r][j]!='\0';++j){
        //if(in[r][j]==' ' | in[r][j]=='A' | in[r][j]=='D' | in[r][j]==',') continue;
        if(in[r][j]=='R'&&(in[r][j+1]<='9'&&in[r][j+1]>='0')){
            if(!DR_visited){
                DR_visited = TRUE;
                DR = in[r][++j]-'0';
                if(DR<0 || DR>7) {printf("Error: R%d invalid.\n",DR);return ERROR;}
            }
            else if(!SR_visited){
                SR_visited = TRUE;
                SR = in[r][++j]-'0';
                if(SR<0 || SR>7) {printf("Error: R%d invalid.\n",SR);return ERROR;}
            }
        }
        else continue;
    }
    printf(" 1001");
    if(DR_visited) PrintReg(DR);
    if(SR_visited) PrintReg(SR);
    printf(" 111111\n");
    return OK;
}

Status RTI(char in[row][MAX_NUM],int number,int r){
    if(number!=8){
        printf("This instruction is not RTI.\n");
        return ERROR;
    }  
    printf(" 1000 000000000000\n");
    return OK;
} 

Status STR(char in[row][MAX_NUM],int number,int r){
    if(number!=7){
        printf("This instruction is not STR.\n");
        return ERROR;
    }
    Status DR_visited=FALSE,BaseR_visited=FALSE;
    int DR,BaseR,offset6=0;
    for(int j=0;j<MAX_NUM && in[r][j]!='\0';++j){
        //if(in[r][j]==' ' | in[r][j]=='A' | in[r][j]=='D' | in[r][j]==',') continue;
        if(in[r][j]=='R'&&(in[r][j+1]<='9'&&in[r][j+1]>='0')){
            if(!DR_visited){
                DR_visited = TRUE;
                DR = in[r][++j]-'0';
                if(DR<0 || DR>7) {printf("Error: R%d invalid.\n",DR);return ERROR;}
            }
            else if(!BaseR_visited){
                BaseR_visited = TRUE;
                BaseR = in[r][++j]-'0';
                if(BaseR<0 || BaseR>7) {printf("Error: R%d invalid.\n",BaseR);return ERROR;}
            }
        }
        if(in[r][j]=='#'){
            int i=0,k,minus=FALSE;
            ++j;
            if(in[r][j]=='-'){
                ++j;
                minus = TRUE;
            }
            while(in[r][j]<='9'&&in[r][j]>='0'){
                ++i; ++j;
            }
            for(k=j-i;i>=1;--i,++k) offset6=offset6+(in[r][k]-'0')*pow(10,i-1);
            if(minus) offset6 = -offset6;
            if(offset6<-32 || offset6>31){printf("Error: offset invalid.\n");return ERROR;}
        }
        else continue;
    }
    printf(" 0111");
    if(DR_visited) PrintReg(DR);
    if(BaseR_visited) PrintReg(BaseR);
    PrintComplement(offset6,6);
    printf("\n");
    return OK;
}

Status TRAP(char in[row][MAX_NUM],int number,int r){
    if(number!=15){
        printf("This instruction is not TRAP.\n");
        return ERROR;
    }
    int trapvect8=0;
    for(int j=0;j<MAX_NUM && in[r][j]!='\0';++j){
        //if(in[r][j]==' ' | in[r][j]=='A' | in[r][j]=='D' | in[r][j]==',') continue;
        if(in[r][j]=='x'){
            int i=0,k;
            ++j;
            while(in[r][j]<='9'&&in[r][j]>='0'){
                ++i; ++j;
            }
            for(k=j-i;i>=1;--i,++k) trapvect8=trapvect8+(in[r][k]-'0')*pow(16,i-1);
        }
        else continue;
    }
    printf(" 1111 0000");
    //printf(" trapvect=%d ",trapvect8);
    PrintComplement(trapvect8,8);
    printf("\n");
    return OK;
}

Status BR(char in[row][MAX_NUM],int number,int r,char labe[row][MAX_NUM]){
    if(number!=0){
        printf("This instruction is not BR.\n");
        return ERROR;
    }
    int n=FALSE,z=FALSE,p=FALSE;
    int label_row,PCoffset9;
    char temp[MAX_NUM]="";
    for(int j=0;j<MAX_NUM && in[r][j]!='\0';++j){
        if(in[r][j]=='B' && in[r][j+1]=='R'){
            j=j+2;
            if(in[r][j]=='n') {n=TRUE;++j;}
            if(in[r][j]=='z') {z=TRUE;++j;}
            if(in[r][j]=='p') {p=TRUE;++j;}
        }
        if((in[r][j]<='Z' && in[r][j]>='A')||(in[r][j]<='z' && in[r][j]>='a')||(in[r][j]<='9' && in[r][j]>='0')||in[r][j]=='_'){
            for(int k=0;in[r][j]!=' ' && in[r][j]!='\0';++k,++j) temp[k]=in[r][j];
        }
    }
    int i;
    for(i=0;i<row;++i)
        if(!strcmp(labe[i],temp)) {label_row = i; break;}
    if(i==row) {printf("Error: Label %s not found!\n",temp); return ERROR;}
    PCoffset9 = label_row-(r+1);      //万一r的下一行是注释怎么办？
    if(PCoffset9<-256 || PCoffset9>255){printf("Error: PCoffset invalid.\n");return ERROR;}
    printf(" 0000 %d%d%d",n,z,p);
    PrintComplement(PCoffset9,9);
    printf("\n");
    return OK;
}

Status LD(char in[row][MAX_NUM],int number,int r,char labe[row][MAX_NUM]){
    if(number!=2){
        printf("This instruction is not LD.\n");
        return ERROR;
    }
    int label_row,PCoffset9,DR;
    char temp[MAX_NUM]="";
    for(int j=0;j<MAX_NUM && in[r][j]!='\0';++j){
    	if(in[r][j]=='L' && in[r][j+1]=='D') j=j+2;
        if(in[r][j]=='R'){
            ++j;
            DR = in[r][j++]-'0';
            if(DR>7 || DR<0){printf("Error: R%d invalid.\n",DR);return ERROR;}
        }
        if((in[r][j]<='Z' && in[r][j]>='A')||(in[r][j]<='z' && in[r][j]>='a')||(in[r][j]<='9' && in[r][j]>='0')||in[r][j]=='_'){
            for(int k=0;in[r][j]!=' ' && in[r][j]!='\0';++k,++j) temp[k]=in[r][j];
        }
    }
    int i;
    for(i=0;i<row;++i)
        if(!strcmp(labe[i],temp)) {label_row = i; break;}
    if(i==row) {printf("Error: Label %s not found!\n",temp); return ERROR;}
    PCoffset9 = label_row-(r+1);      //万一r的下一行是注释怎么办？
    if(PCoffset9<-256 || PCoffset9>255){printf("Error: PCoffset invalid.\n");return ERROR;}
    printf(" 0010");
    PrintReg(DR);
    PrintComplement(PCoffset9,9);
    printf("\n");
    return OK;
}

Status LDI(char in[row][MAX_NUM],int number,int r,char labe[row][MAX_NUM]){
    if(number!=10){
        printf("This instruction is not LDI.\n");
        return ERROR;
    }
    int label_row,PCoffset9,DR;
    char temp[MAX_NUM]="";
    for(int j=0;j<MAX_NUM && in[r][j]!='\0';++j){
        if(in[r][j]=='L' && in[r][j+1]=='D' && in[r][j+2]=='I') j=j+3;
        if(in[r][j]=='R'){
            ++j;
            DR = in[r][j++]-'0';
            if(DR>7 || DR<0){printf("Error: R%d invalid.\n",DR);return ERROR;}
        }
        if((in[r][j]<='Z' && in[r][j]>='A')||(in[r][j]<='z' && in[r][j]>='a')||(in[r][j]<='9' && in[r][j]>='0')||in[r][j]=='_'){
            for(int k=0;in[r][j]!=' ' && in[r][j]!='\0';++k,++j) temp[k]=in[r][j];
        }
    }
    int i;
    for(i=0;i<row;++i)
        if(!strcmp(labe[i],temp)) {label_row = i; break;}
    if(i==row) {printf("Error: Label %s not found!\n",temp); return ERROR;}
    PCoffset9 = label_row-(r+1);      //万一r的下一行是注释怎么办？
    if(PCoffset9<-256 || PCoffset9>255){printf("Error: PCoffset invalid.\n");return ERROR;}
    printf(" 1010");
    PrintReg(DR);
    PrintComplement(PCoffset9,9);
    printf("\n");
    return OK;
}

Status LEA(char in[row][MAX_NUM],int number,int r,char labe[row][MAX_NUM]){
    if(number!=14){
        printf("This instruction is not LEA.\n");
        return ERROR;
    }
    int label_row,PCoffset9,DR;
    char temp[MAX_NUM]="";
    for(int j=0;j<MAX_NUM && in[r][j]!='\0';++j){
        if(in[r][j]=='L' && in[r][j+1]=='E' && in[r][j+2]=='A') j=j+3;
        if(in[r][j]=='R'){
            ++j;
            DR = in[r][j++]-'0';
            if(DR>7 || DR<0){printf("Error: R%d invalid.\n",DR);return ERROR;}
        }
        if((in[r][j]<='Z' && in[r][j]>='A')||(in[r][j]<='z' && in[r][j]>='a')||(in[r][j]<='9' && in[r][j]>='0')||in[r][j]=='_'){
            for(int k=0;in[r][j]!=' ' && in[r][j]!='\0';++k,++j) temp[k]=in[r][j];
        }
    }
    int i;
    for(i=0;i<row;++i)
        if(!strcmp(labe[i],temp)) {label_row = i; break;}
    if(i==row) {printf("Error: Label %s not found!\n",temp); return ERROR;}
    PCoffset9 = label_row-(r+1);      //万一r的下一行是注释怎么办？
    if(PCoffset9<-256 || PCoffset9>255){printf("Error: PCoffset invalid.\n");return ERROR;}
    printf(" 1110");
    PrintReg(DR);
    PrintComplement(PCoffset9,9);
    printf("\n");
    return OK;
}

Status ST(char in[row][MAX_NUM],int number,int r,char labe[row][MAX_NUM]){
    if(number!=3){
        printf("This instruction is not ST.\n");
        return ERROR;
    }
    int label_row,PCoffset9,SR;
    char temp[MAX_NUM]="";
    for(int j=0;j<MAX_NUM && in[r][j]!='\0';++j){
        if(in[r][j]=='S' && in[r][j+1]=='T') j=j+2;
        if(in[r][j]=='R'){
            ++j;
            SR = in[r][j++]-'0';
            if(SR>7 || SR<0){printf("Error: R%d invalid.\n",SR);return ERROR;}
        }
        if((in[r][j]<='Z' && in[r][j]>='A')||(in[r][j]<='z' && in[r][j]>='a')||(in[r][j]<='9' && in[r][j]>='0')||in[r][j]=='_'){
            for(int k=0;in[r][j]!=' ' && in[r][j]!='\0';++k,++j) temp[k]=in[r][j];
        }
    }
    int i;
    for(i=0;i<row;++i)
        if(!strcmp(labe[i],temp)) {label_row = i; break;}
    if(i==row) {printf("Error: Label %s not found!\n",temp); return ERROR;}
    PCoffset9 = label_row-(r+1);      //万一r的下一行是注释怎么办？
    if(PCoffset9<-256 || PCoffset9>255){printf("Error: PCoffset invalid.\n");return ERROR;}
    printf(" 0011");
    PrintReg(SR);
    PrintComplement(PCoffset9,9);
    printf("\n");
    return OK;
}

Status STI(char in[row][MAX_NUM],int number,int r,char labe[row][MAX_NUM]){
    if(number!=11){
        printf("This instruction is not STI.\n");
        return ERROR;
    }
    int label_row,PCoffset9,SR;
    char temp[MAX_NUM]="";
    for(int j=0;j<MAX_NUM && in[r][j]!='\0';++j){
        if(in[r][j]=='S' && in[r][j+1]=='T' && in[r][j+2]=='I') j=j+3;
        if(in[r][j]=='R'){
            ++j;
            SR = in[r][j++]-'0';
        }
        if((in[r][j]<='Z' && in[r][j]>='A')||(in[r][j]<='z' && in[r][j]>='a')||(in[r][j]<='9' && in[r][j]>='0')||in[r][j]=='_'){
            for(int k=0;in[r][j]!=' ' && in[r][j]!='\0';++k,++j) temp[k]=in[r][j];
        }
    }
    int i;
    for(i=0;i<row;++i)
        if(!strcmp(labe[i],temp)) {label_row = i; break;}
    if(i==row) {printf("Error: Label %s not found!\n",temp); return ERROR;}
    PCoffset9 = label_row-(r+1);      //万一r的下一行是注释怎么办？
    printf(" 1011");
    PrintReg(SR);
    PrintComplement(PCoffset9,9);
    printf("\n");
    return OK;
}

Status ORIG(char in[row][MAX_NUM],int r){
    int start_addr=0;
    for(int j=0;j<MAX_NUM && in[r][j]!='\0';++j){
        if(in[r][j]=='x'){
            int i=0,k;
            ++j;
            while(in[r][j]<='9'&&in[r][j]>='0'){
                ++i; ++j;
            }
            for(k=j-i;i>=1;--i,++k) start_addr=start_addr+(in[r][k]-'0')*pow(16,i-1);
        }
        else continue;
    }
    PrintComplement(start_addr,16);
    printf("\n");
    return OK;
}

Status HALT(void){
    printf(" 1111 0000 00100101\n");
    return OK;
}

int assemble(char in[row][MAX_NUM],int r){
    for(int j=0;j<MAX_NUM && in[r][j]!='\0';++j){
        if(in[r][j]==';') break;
        if((j==0||!((in[r][j-1]>='A'&&in[r][j]<='Z')||(in[r][j-1]>='a'&&in[r][j]<='z'))) && in[r][j]=='B' && in[r][j+1]=='R') return 0;
        if((j==0||!((in[r][j-1]>='A'&&in[r][j]<='Z')||(in[r][j-1]>='a'&&in[r][j]<='z'))) && in[r][j]=='A' && in[r][j+1]=='D' && in[r][j+2]=='D' ) return 1;
        if((j==0||!((in[r][j-1]>='A'&&in[r][j]<='Z')||(in[r][j-1]>='a'&&in[r][j]<='z'))) && in[r][j]=='L' && in[r][j+1]=='D' && in[r][j+2]==' ') return 2;
        if((j==0||!((in[r][j-1]>='A'&&in[r][j]<='Z')||(in[r][j-1]>='a'&&in[r][j]<='z'))) && in[r][j]=='S' && in[r][j+1]=='T' && in[r][j+2]==' ') return 3;
        if((j==0||!((in[r][j-1]>='A'&&in[r][j]<='Z')||(in[r][j-1]>='a'&&in[r][j]<='z'))) && in[r][j]=='J' && in[r][j+1]=='S' && in[r][j+2]=='R') return 4;
        if((j==0||!((in[r][j-1]>='A'&&in[r][j]<='Z')||(in[r][j-1]>='a'&&in[r][j]<='z'))) && in[r][j]=='A' && in[r][j+1]=='N' && in[r][j+2]=='D') return 5;
        if((j==0||!((in[r][j-1]>='A'&&in[r][j]<='Z')||(in[r][j-1]>='a'&&in[r][j]<='z'))) && in[r][j]=='L' && in[r][j+1]=='D' && in[r][j+2]=='R') return 6;
        if((j==0||!((in[r][j-1]>='A'&&in[r][j]<='Z')||(in[r][j-1]>='a'&&in[r][j]<='z'))) && in[r][j]=='S' && in[r][j+1]=='T' && in[r][j+2]=='R') return 7;
        if((j==0||!((in[r][j-1]>='A'&&in[r][j]<='Z')||(in[r][j-1]>='a'&&in[r][j]<='z'))) && in[r][j]=='R' && in[r][j+1]=='T' && in[r][j+2]=='I') return 8;
        if((j==0||!((in[r][j-1]>='A'&&in[r][j]<='Z')||(in[r][j-1]>='a'&&in[r][j]<='z'))) && in[r][j]=='N' && in[r][j+1]=='O' && in[r][j+2]=='T') return 9;
        if((j==0||!((in[r][j-1]>='A'&&in[r][j]<='Z')||(in[r][j-1]>='a'&&in[r][j]<='z'))) && in[r][j]=='L' && in[r][j+1]=='D' && in[r][j+2]=='I') return 10;
        if((j==0||!((in[r][j-1]>='A'&&in[r][j]<='Z')||(in[r][j-1]>='a'&&in[r][j]<='z'))) && in[r][j]=='S' && in[r][j+1]=='T' && in[r][j+2]=='I') return 11;
        if((j==0||!((in[r][j-1]>='A'&&in[r][j]<='Z')||(in[r][j-1]>='a'&&in[r][j]<='z'))) && in[r][j]=='J' && in[r][j+1]=='M' && in[r][j+2]=='P') return 12;
        if((j==0||!((in[r][j-1]>='A'&&in[r][j]<='Z')||(in[r][j-1]>='a'&&in[r][j]<='z'))) && in[r][j]=='L' && in[r][j+1]=='E' && in[r][j+2]=='A') return 14;
        if((j==0||!((in[r][j-1]>='A'&&in[r][j]<='Z')||(in[r][j-1]>='a'&&in[r][j]<='z'))) && in[r][j]=='T' && in[r][j+1]=='R' && in[r][j+2]=='A' && in[r][j+3]=='P') return 15;
        if(in[r][j]=='.' && in[r][j+1]=='O' && in[r][j+2]=='R' && in[r][j+3]=='I' && in[r][j+4]=='G') return 16;
        if((j==0||!((in[r][j-1]>='A'&&in[r][j]<='Z')||(in[r][j-1]>='a'&&in[r][j]<='z'))) && in[r][j]=='H' && in[r][j+1]=='A' && in[r][j+2]=='L' && in[r][j+3]=='T') return 17;
        if(in[r][j]=='.' && in[r][j+1]=='E' && in[r][j+2]=='N' && in[r][j+3]=='D') return 18;
    }
    return -1;
}

int main(){
    char instr[row][MAX_NUM]={
	".ORIG x3050",
    "LD R1,SIX",
    "LD R2,NUMBER",
    "AND R3,R3,#0",
    "AGAIN ADD R3,R3,R2",
    "ADD R1,R1,#-1",
    "BRp AGAIN",
    "HALT",
    "NUMBER .BLKW 1",
    "SIX .FILL x0006 "
    ".END"
	};
    CreateKeylist(key);
	CreateLabellist(label,instr,key);
    int number;
    int r=0;
    Status END_visited = FALSE, iserror = FALSE, finish = FALSE;
    number = assemble(instr,r);
    while(!finish){
        switch(number){
            case -1: break;
            case 0 : {if(!BR(instr,number,r,label)) iserror = TRUE;break;}
            case 1 : {if(!ADD(instr,number,r)) iserror = TRUE;break;}
            case 2 : {if(!LD(instr,number,r,label)) iserror = TRUE;break;}
            case 3 : {if(!ST(instr,number,r,label)) iserror = TRUE;break;}
            case 4 : {if(!JSR(instr,number,r,label)) iserror = TRUE;break;}
            case 5 : {if(!AND(instr,number,r)) iserror = TRUE;break;}
            case 6 : {if(!LDR(instr,number,r)) iserror = TRUE;break;}
            case 7 : {if(!STR(instr,number,r)) iserror = TRUE;break;}
            case 8 : {if(!RTI(instr,number,r)) iserror = TRUE;break;}
            case 9 : {if(!NOT(instr,number,r)) iserror = TRUE;break;}
            case 10 : {if(!LDI(instr,number,r,label)) iserror = TRUE;break;}
            case 11 : {if(!STI(instr,number,r,label)) iserror = TRUE;break;}
            case 12 : {if(!JMP(instr,number,r)) iserror = TRUE;break;}
            case 14 : {if(!LEA(instr,number,r,label)) iserror = TRUE;break;}
            case 15 : {if(!TRAP(instr,number,r)) iserror = TRUE;break;}
            case 16 : {if(!ORIG(instr,r)) iserror = TRUE;break;}
            case 17 : {if(!HALT()) iserror = TRUE;break;}
            case 18 : {END_visited = TRUE; finish = TRUE;}
        }
        if(iserror == TRUE) break;
        ++r;
        number = assemble(instr,r);
    }
    if(!END_visited && !iserror) printf("Error: there is no .END\n"); 
}