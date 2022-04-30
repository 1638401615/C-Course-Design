#include"def.h"

//读入cnf文件
int loadcnf(Clauses *&cls)
{
    FILE *fp;
    char ch;
    char chs[30];
    Clauses *cp;
    Literals *lp;
    int num;
    fp = fopen(filename,"r");
    if(fp == NULL)
    {
        printf("文件打开失败！\n");
        return 0;
    }
    while ((ch = getc(fp)) == 'c')//当读入行为注释行的时候
    {
        while ((ch = getc(fp)) != '\n')//重复读入，抛弃改行内容直至换行
        {
            continue;
        }
    }
    fscanf(fp,"%[ cnf ]",chs);    //读取cnf这个字符串
    fscanf(fp,"%d",&varible_num); //读取变量个数
    fscanf(fp,"%d",&clause_num);  //读取字句个数
    cls = (Clauses *)malloc(sizeof(Clauses));
    cls ->nextclause = NULL;
    cls->head = (Literals *)malloc(sizeof(Literals));
    cls->head->next = NULL;
    cp = cls;      //指向第一个子句 
    lp = cls->head;//指向第一个子句的第一个变量

    for(int i = 1;i <= clause_num;i++)
    {
        
        fscanf(fp,"%d",&num);
        while (num != 0)
        {
            lp->data = num;
            
            fscanf(fp,"%d",&num);
            if(num == 0)
            lp -> next = NULL;
            else
            {
                lp -> next = (Literals *)malloc(sizeof(Literals));
                lp = lp -> next;
                lp -> next = NULL;
            }  
        }
       
        if(i == clause_num)//当读取到最后一个子句的时候
        {
            cp -> nextclause = NULL;
            break;
        }
        //当不是最后一个子句的时候
        cp->nextclause = (Clauses *)malloc(sizeof(Clauses));//为下一个子句分配内存
        cp->nextclause->head = (Literals *)malloc(sizeof(Literals));//为下一个子句的第一个变量分配内存 
        cp = cp->nextclause; //子句指针指向下一个子句
        lp = cp->head;       //文字指针指向新子句的开头
    }
    printf("cnf文件数据读取完毕！\n");
    fclose(fp);
    return 1;
}
