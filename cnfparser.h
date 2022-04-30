#include"def.h"

//����cnf�ļ�
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
        printf("�ļ���ʧ�ܣ�\n");
        return 0;
    }
    while ((ch = getc(fp)) == 'c')//��������Ϊע���е�ʱ��
    {
        while ((ch = getc(fp)) != '\n')//�ظ����룬������������ֱ������
        {
            continue;
        }
    }
    fscanf(fp,"%[ cnf ]",chs);    //��ȡcnf����ַ���
    fscanf(fp,"%d",&varible_num); //��ȡ��������
    fscanf(fp,"%d",&clause_num);  //��ȡ�־����
    cls = (Clauses *)malloc(sizeof(Clauses));
    cls ->nextclause = NULL;
    cls->head = (Literals *)malloc(sizeof(Literals));
    cls->head->next = NULL;
    cp = cls;      //ָ���һ���Ӿ� 
    lp = cls->head;//ָ���һ���Ӿ�ĵ�һ������

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
       
        if(i == clause_num)//����ȡ�����һ���Ӿ��ʱ��
        {
            cp -> nextclause = NULL;
            break;
        }
        //���������һ���Ӿ��ʱ��
        cp->nextclause = (Clauses *)malloc(sizeof(Clauses));//Ϊ��һ���Ӿ�����ڴ�
        cp->nextclause->head = (Literals *)malloc(sizeof(Literals));//Ϊ��һ���Ӿ�ĵ�һ�����������ڴ� 
        cp = cp->nextclause; //�Ӿ�ָ��ָ����һ���Ӿ�
        lp = cp->head;       //����ָ��ָ�����Ӿ�Ŀ�ͷ
    }
    printf("cnf�ļ����ݶ�ȡ��ϣ�\n");
    fclose(fp);
    return 1;
}
