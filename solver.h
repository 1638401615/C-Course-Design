//#include"def.h"
#include"cnfparser.h"

/**************************************
 * 函数名称：cleanclause
 * 参数：Clauses*&(子句链表的指针的引用)
 * 功能：清楚当前储存的所有子句链表
 * 返回值：无
 *************************************/
 
void cleanclause(Clauses *&cls)//清除所有子句
{
    Clauses *cp1 , *cp2;
    Literals *lp1 , *lp2;
    for(cp1 = cls;cp1 != NULL;cp1 = cp2)
    {
        cp2 = cp1->nextclause;
        for(lp1 = cp1->head;lp1 != NULL;lp1 = lp2)
        {
            lp2 = lp1->next;
            free(lp1);
        }
        free(cp1);
    }
    cls = NULL;
}

/**************************************
 * 函数名称：isunitclause
 * 参数：Literal*(文章链表的指针)
 * 功能：判断是否为单子句
 * 返回值：成功返回1，否返回0
 *************************************/
int isunitclause(Literals *lit)
{
    if(lit != NULL && lit->next == NULL)
    return 1;
    else
    return 0;
}

/**************************************
 * 函数名称：eraseclause
 * 参数：Clauses*& Clauses*&(子句链表的指针的引用) 第一个子句指针指向要删除子句 第二个指针指向子句链表的头部
 * 功能：清除满足条件的某个子句
 * 返回值：成功返回1，失败返回0
 *************************************/
int eraseclause(Clauses *&cls, Clauses *&fcls)
{
    Clauses *cp = fcls;
    if(cp == cls)//删除子句为头部子句
    fcls = fcls->nextclause;
    else
    {
        while(cp != NULL && cp->nextclause != cls)
        cp = cp->nextclause;
        cp->nextclause = cp->nextclause->nextclause;
    }
    free(cls);
    cls = NULL;
    return 1;
}

/**************************************
 * 函数名称：eraseliteral
 * 参数：Literal*& Literal*&(文字链表的指针的引用) 第一个子句指针指向要删除文字 第二个指针指向文字链表的头部
 * 功能：清除指定子句中的特定文字
 * 返回值：成功返回1，失败返回0
 *************************************/
int  eraseliteral(Literals *&lit,Literals *&flit)
{
    Literals *lp = flit;
    if(lp == lit)//删除的文字在链表头部
    flit = flit->next;
    else
    {
        while (lp != NULL && lp->next != lit)
        {
            lp = lp->next;
        }
        lp->next = lp->next->next;
    }
    free(lit);
    lit = NULL;
    return OK;
}

/**************************************
 * 函数名称：isemptyclause
 * 参数：Clause*
 * 功能: 判断是否有子句是空的
 * 返回值：是返回1，否返回0
 *************************************/
int isemptyclause(Clauses *cls)
{
    Clauses *cp = cls;
    while (cp != NULL)
    {
        if(cp->head == NULL)
        return 1;
        cp = cp->nextclause;
    }
    return 0;
}

/**************************************
 * 函数名称：copyclause
 * 参数：Clause*& Clause*
 * 功能: 拷贝一份子句链表到前者链表中
 * 返回值：无
 *************************************/
void copyclause(Clauses *&cls1,Clauses *cls2)
{
    Clauses *cp1 , *cp2;
    Literals *lp1 , *lp2;
    cls1 = (Clauses *)malloc(sizeof(Clauses));
    cls1->head = (Literals *)malloc(sizeof(Literals));
    cls1->nextclause = NULL;
    cls1->head->next = NULL;
    for(cp2 = cls2 , cp1 = cls1;cp2 != NULL;cp1 = cp1->nextclause,cp2 = cp2->nextclause)
    {
        for(lp1 = cp1->head , lp2 = cp2->head; lp2 != NULL;lp2 = lp2 ->next,lp1 = lp1->next)
        {
            lp1->data = lp2->data;
            lp1->next = (Literals*)malloc(sizeof(Literals));
            lp1->next->next = NULL;
            if(lp2->next == NULL)
            {
                free(lp1->next);
                lp1->next = NULL;
            }
        }
        //cp1->head = cp2->head;
        cp1->nextclause = (Clauses*)malloc(sizeof(Clauses));
        cp1->nextclause->head = (Literals*)malloc(sizeof(Literals));
        cp1->nextclause->nextclause = NULL;
        cp1->nextclause->head->next = NULL;
        if(cp2->nextclause == NULL)
        {
            free(cp1->nextclause->head);
            free(cp1->nextclause);
            cp1->nextclause = NULL;
        }
    }
}


/**************************************
 * 函数名称：addclause
 * 参数：Clause*& clause*&
 * 功能: 向前者子句链表中添加后者
 * 返回值：成功返回1，否则返回0
 *************************************/
int addclause(Clauses *&cls,Clauses *&cls_add)
{
    cls_add->nextclause = cls;
    cls = cls_add;
    return 1;
}

/**************************************
 * 函数名称：DPLL
 * 参数：Clause*& int[]
 * 功能: 利用dpll算法求解sat问题
 * 返回值：有解返回1，无解返回0
 *************************************/
int DPLL(Clauses *&cls,int value[])
{
    Clauses *clsp1 = cls , *clsp2 = cls , *clsp3;
    Literals *lp;
    int *count;
    count = (int *)malloc(sizeof(int) * (2 * varible_num + 1));
search:    while(clsp1 != NULL && isunitclause(clsp1->head) == 0)
    clsp1 = clsp1->nextclause;
    if(clsp1 != NULL)//有单子句存在
    {
        //先给单子句中的变量赋值
        if(clsp1->head->data > 0)
        value[clsp1->head->data] = 1;
        else
        value[-clsp1->head->data] = 0;
        int knownnum = clsp1->head->data;//记录一下已赋值变量
        for(clsp1 = cls;clsp1 != NULL;clsp1 = clsp2)
        {
            clsp2 = clsp1->nextclause;
            for(lp = clsp1->head;lp != NULL;lp = lp->next)
            {
                if(lp->data == knownnum)//子句中含有该文字，删除子句
                {
                    eraseclause(clsp1,cls);
                    break;
                }
                if(lp->data == -knownnum)//子句中含有该文字的反文字，删除文字
                {
                    eraseliteral(lp,clsp1->head);
                    break;
                }
            }
        }
        if(cls == NULL)//子句链表清空，该cnf可满足
        {
            free(count);
            return satisfied;
        }
        if(isemptyclause(cls))//若存在空子句，说明该子句无法满足,则cnf无法满足
        {
            free(count);
            cleanclause(cls);
            return unsatisfied;
        }
        clsp1 = cls;
        goto search;//返回继续利用规则化简cnf
    }
    for(int i = 0;i <= 2 * varible_num;i++)
    count[i] = 0;
    //计算各文字出现个数，作为选取标准
    for(clsp2 = cls;clsp2 != NULL;clsp2 = clsp2->nextclause)
    {
        for(lp = clsp2->head;lp != NULL;lp = lp->next)
        {
            if(lp->data > 0)
            count[lp->data]++;
            if(lp->data < 0)
            count[varible_num - lp->data]++;
        }
    }
    int maxcount = 0;
    int word;//记录出现最多的文字

    //此处先找出现次数最多的正文字，遍历完正文字再找负文字，提升了效率，比单纯找最多文字还快
    for(int i = 1;i <= varible_num;i++)
    {
        if(maxcount < count[i])
        {
            maxcount = count[i];
            word = i;
        }
    }
    if(maxcount == 0)//cnf中不存在正文字了
    {
        for(int i = varible_num + 1;i <= varible_num * 2;i++)
        {
            if(maxcount < count[i])
            {
                maxcount = count[i];
                word = varible_num - i;
            }
        }
    }
    /*for(int i = 1;i <= varible_num * 2;i++)
    {
        if(maxcount < count[i])
        {
            maxcount = count[i];
            if(i > varible_num)
            word = varible_num - i;
            else
            word = i;
        }

    }*/
    free(count);
    //建立一个该文字的单子句，添加到子句链表中进行化简
    clsp2 = (Clauses *)malloc(sizeof(Clauses));
    clsp2->head = (Literals *)malloc(sizeof(Literals));
    clsp2->head->next = NULL;
    clsp2->head->data = word;
    clsp2->nextclause = NULL;
    copyclause(clsp3,cls);
    addclause(clsp3,clsp2);
    if(DPLL(clsp3,value) == satisfied)//进入第一分支搜索
    return satisfied;
    cleanclause(clsp3);//释放内存
    //第一分支没有答案，返回到第二分支进行搜索
    clsp2 = (Clauses *)malloc(sizeof(Clauses));
    clsp2->head = (Literals *)malloc(sizeof(Literals));
    clsp2->head->next = NULL;
    clsp2->head->data = -word;
    clsp2->nextclause = NULL;
    copyclause(clsp3,cls);
    addclause(clsp3,clsp2);
    int result = DPLL(clsp3,value);//记录第二分支状态
    cleanclause(clsp3);
    //cleanclause(cls);
    return result;
}

/**************************************
 * 函数名称：saveanswer
 * 参数：int double int*
 * 功能: 将答案保存到同名文件中
 * 返回值：成功返回1，失败返回0
 *************************************/
int saveanswer(int result  , double duration , int value[])
{
    FILE *fp;
    for(int i = 0;filename[i] != '\0';i++)
    {
        if(filename[i] == '.' && filename[i+1] == 't' &&filename[i+2] == 'x' && filename[i + 3] == 't')
        {
            //更换文件名后缀
            filename[i + 1] = 'r';
            filename[i + 2] = 'e';
            filename[i + 3] = 's';
            break;
        }
    }
    fp = fopen(filename,"w");
    if(fp == NULL)
    {
        printf("文件打开失败！\n");
        return 0;
    }
    fprintf(fp,"s %d\n",result);
    fprintf(fp,"v ");
    if(result == 1)
    {
        for(int i = 1;i <= varible_num;i++)
        {
            if(value[i] == 1)
            fprintf(fp,"%d ",i);
            else
            fprintf(fp,"%d ",-i);
        }
    }
    fprintf(fp,"\n");
    fprintf(fp,"t %lf", duration * 1000);
    fclose(fp);
    return 1;
}

/**************************************
 * 函数名称：checkanswer
 * 参数：int* clause* int
 * 功能: 检查求解答案的正确性
 * 返回值：正确返回1，错误返回0 
 *************************************/
int checkanswer(int value[] , Clauses *&cls , int result)
{
    Clauses *clsp;
    Literals *lp;
    if(result == 1)//有解情况
    {
        //遍历所有子句
        for(clsp = cls;clsp != NULL;clsp = clsp->nextclause)
        {
            for(lp = cls->head;lp != NULL;lp = lp->next)
            {
                if(lp->data > 0 && value[lp->data] == 1)
                break;
                else if(lp->data < 0 && value[-(lp->data)] == 0)
                break;
                else if(lp -> next == NULL)//如果遍历到一个子句的末尾还没找到满足子句的文字
                return FALSE;
            }

        }
        return TRUE;
    }
    else if(result == 0)//无解情况
    {
        for(clsp = cls;clsp != NULL;clsp = clsp->nextclause)
        {
            for(lp = cls->head;lp != NULL;lp = lp->next)
            {
                if(value[lp->data] == 1 && lp->data > 0)
                return FALSE;
                else if(value[lp->data] == 0 && lp->data < 0)
                return FALSE;
            }
        }
        return TRUE;
    }
    return UNFEASIBLE;
}

