//#include"def.h"
#include"cnfparser.h"

/**************************************
 * �������ƣ�cleanclause
 * ������Clauses*&(�Ӿ������ָ�������)
 * ���ܣ������ǰ����������Ӿ�����
 * ����ֵ����
 *************************************/
 
void cleanclause(Clauses *&cls)//��������Ӿ�
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
 * �������ƣ�isunitclause
 * ������Literal*(���������ָ��)
 * ���ܣ��ж��Ƿ�Ϊ���Ӿ�
 * ����ֵ���ɹ�����1���񷵻�0
 *************************************/
int isunitclause(Literals *lit)
{
    if(lit != NULL && lit->next == NULL)
    return 1;
    else
    return 0;
}

/**************************************
 * �������ƣ�eraseclause
 * ������Clauses*& Clauses*&(�Ӿ������ָ�������) ��һ���Ӿ�ָ��ָ��Ҫɾ���Ӿ� �ڶ���ָ��ָ���Ӿ������ͷ��
 * ���ܣ��������������ĳ���Ӿ�
 * ����ֵ���ɹ�����1��ʧ�ܷ���0
 *************************************/
int eraseclause(Clauses *&cls, Clauses *&fcls)
{
    Clauses *cp = fcls;
    if(cp == cls)//ɾ���Ӿ�Ϊͷ���Ӿ�
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
 * �������ƣ�eraseliteral
 * ������Literal*& Literal*&(���������ָ�������) ��һ���Ӿ�ָ��ָ��Ҫɾ������ �ڶ���ָ��ָ�����������ͷ��
 * ���ܣ����ָ���Ӿ��е��ض�����
 * ����ֵ���ɹ�����1��ʧ�ܷ���0
 *************************************/
int  eraseliteral(Literals *&lit,Literals *&flit)
{
    Literals *lp = flit;
    if(lp == lit)//ɾ��������������ͷ��
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
 * �������ƣ�isemptyclause
 * ������Clause*
 * ����: �ж��Ƿ����Ӿ��ǿյ�
 * ����ֵ���Ƿ���1���񷵻�0
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
 * �������ƣ�copyclause
 * ������Clause*& Clause*
 * ����: ����һ���Ӿ�����ǰ��������
 * ����ֵ����
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
 * �������ƣ�addclause
 * ������Clause*& clause*&
 * ����: ��ǰ���Ӿ���������Ӻ���
 * ����ֵ���ɹ�����1�����򷵻�0
 *************************************/
int addclause(Clauses *&cls,Clauses *&cls_add)
{
    cls_add->nextclause = cls;
    cls = cls_add;
    return 1;
}

/**************************************
 * �������ƣ�DPLL
 * ������Clause*& int[]
 * ����: ����dpll�㷨���sat����
 * ����ֵ���нⷵ��1���޽ⷵ��0
 *************************************/
int DPLL(Clauses *&cls,int value[])
{
    Clauses *clsp1 = cls , *clsp2 = cls , *clsp3;
    Literals *lp;
    int *count;
    count = (int *)malloc(sizeof(int) * (2 * varible_num + 1));
search:    while(clsp1 != NULL && isunitclause(clsp1->head) == 0)
    clsp1 = clsp1->nextclause;
    if(clsp1 != NULL)//�е��Ӿ����
    {
        //�ȸ����Ӿ��еı�����ֵ
        if(clsp1->head->data > 0)
        value[clsp1->head->data] = 1;
        else
        value[-clsp1->head->data] = 0;
        int knownnum = clsp1->head->data;//��¼һ���Ѹ�ֵ����
        for(clsp1 = cls;clsp1 != NULL;clsp1 = clsp2)
        {
            clsp2 = clsp1->nextclause;
            for(lp = clsp1->head;lp != NULL;lp = lp->next)
            {
                if(lp->data == knownnum)//�Ӿ��к��и����֣�ɾ���Ӿ�
                {
                    eraseclause(clsp1,cls);
                    break;
                }
                if(lp->data == -knownnum)//�Ӿ��к��и����ֵķ����֣�ɾ������
                {
                    eraseliteral(lp,clsp1->head);
                    break;
                }
            }
        }
        if(cls == NULL)//�Ӿ�������գ���cnf������
        {
            free(count);
            return satisfied;
        }
        if(isemptyclause(cls))//�����ڿ��Ӿ䣬˵�����Ӿ��޷�����,��cnf�޷�����
        {
            free(count);
            cleanclause(cls);
            return unsatisfied;
        }
        clsp1 = cls;
        goto search;//���ؼ������ù��򻯼�cnf
    }
    for(int i = 0;i <= 2 * varible_num;i++)
    count[i] = 0;
    //��������ֳ��ָ�������Ϊѡȡ��׼
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
    int word;//��¼������������

    //�˴����ҳ��ִ������������֣����������������Ҹ����֣�������Ч�ʣ��ȵ�����������ֻ���
    for(int i = 1;i <= varible_num;i++)
    {
        if(maxcount < count[i])
        {
            maxcount = count[i];
            word = i;
        }
    }
    if(maxcount == 0)//cnf�в�������������
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
    //����һ�������ֵĵ��Ӿ䣬��ӵ��Ӿ������н��л���
    clsp2 = (Clauses *)malloc(sizeof(Clauses));
    clsp2->head = (Literals *)malloc(sizeof(Literals));
    clsp2->head->next = NULL;
    clsp2->head->data = word;
    clsp2->nextclause = NULL;
    copyclause(clsp3,cls);
    addclause(clsp3,clsp2);
    if(DPLL(clsp3,value) == satisfied)//�����һ��֧����
    return satisfied;
    cleanclause(clsp3);//�ͷ��ڴ�
    //��һ��֧û�д𰸣����ص��ڶ���֧��������
    clsp2 = (Clauses *)malloc(sizeof(Clauses));
    clsp2->head = (Literals *)malloc(sizeof(Literals));
    clsp2->head->next = NULL;
    clsp2->head->data = -word;
    clsp2->nextclause = NULL;
    copyclause(clsp3,cls);
    addclause(clsp3,clsp2);
    int result = DPLL(clsp3,value);//��¼�ڶ���֧״̬
    cleanclause(clsp3);
    //cleanclause(cls);
    return result;
}

/**************************************
 * �������ƣ�saveanswer
 * ������int double int*
 * ����: ���𰸱��浽ͬ���ļ���
 * ����ֵ���ɹ�����1��ʧ�ܷ���0
 *************************************/
int saveanswer(int result  , double duration , int value[])
{
    FILE *fp;
    for(int i = 0;filename[i] != '\0';i++)
    {
        if(filename[i] == '.' && filename[i+1] == 't' &&filename[i+2] == 'x' && filename[i + 3] == 't')
        {
            //�����ļ�����׺
            filename[i + 1] = 'r';
            filename[i + 2] = 'e';
            filename[i + 3] = 's';
            break;
        }
    }
    fp = fopen(filename,"w");
    if(fp == NULL)
    {
        printf("�ļ���ʧ�ܣ�\n");
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
 * �������ƣ�checkanswer
 * ������int* clause* int
 * ����: ������𰸵���ȷ��
 * ����ֵ����ȷ����1�����󷵻�0 
 *************************************/
int checkanswer(int value[] , Clauses *&cls , int result)
{
    Clauses *clsp;
    Literals *lp;
    if(result == 1)//�н����
    {
        //���������Ӿ�
        for(clsp = cls;clsp != NULL;clsp = clsp->nextclause)
        {
            for(lp = cls->head;lp != NULL;lp = lp->next)
            {
                if(lp->data > 0 && value[lp->data] == 1)
                break;
                else if(lp->data < 0 && value[-(lp->data)] == 0)
                break;
                else if(lp -> next == NULL)//���������һ���Ӿ��ĩβ��û�ҵ������Ӿ������
                return FALSE;
            }

        }
        return TRUE;
    }
    else if(result == 0)//�޽����
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

