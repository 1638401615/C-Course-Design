
#include"solver.h"
#include "sudoku.h"
int main()
{
    Clauses *cls = NULL;
    Clauses *clsp;
    Literals *lit = NULL;
    clock_t start , finish;
    clock_t a , b;
    double duration;
    int op = 1,result = 2, re = 0 , option = 1 , covernum = 0;
    int x , y , z;
    int *value;
    while (op)
    {
        system("cls");
        printf("\n\n");
        printf("         ����DPLL�㷨��SAT������\n");
        printf("--------------------------------------------------\n");
        printf("1.��ȡcnf�ļ�                 2.����Ӿ�\n");
        printf("3.��Ⲣ�����              4.����\n");
        printf("5.������Ϸ                    0.�˳�����\n");
        printf("--------------------------------------------------\n");
        printf("	��ѡ����Ĳ���[0~5]:");
        scanf("%d", &op);
		switch (op)
        {
        case 1:
            printf("������Ҫ��ȡ���ļ�·����\n");
            scanf("%s",filename);
            loadcnf(cls);
            getchar();getchar();
            break;
        case 2:
            if(cls == NULL)
            printf("��δ��ȡcnf�ļ���\n");
            else
            {
                for(clsp = cls;clsp != NULL;clsp = clsp->nextclause)
                {
                    for(lit = clsp->head;lit != NULL;lit = lit->next)
                    {
                        printf("%d ",lit->data);
                    }
                    printf("\n");
                }
            }
            getchar();getchar();
            break;
        case 3:
            if(cls == NULL)
            printf("�ļ�δ���룡\n");
            else
            {
                value = (int *)malloc(sizeof(int) * (varible_num +1));
                for(int i = 0;i <= varible_num;i++)
                value[i] = -1;//ֻ�ǳ�ʼ��һ�£�û�и�ֵ���壡
                start = clock();
                result = DPLL(cls,value);
                finish = clock();
                duration = (double)(finish - start)/ CLOCKS_PER_SEC;//����ʱ��
                if(result == satisfied)
                {
                    for(int i = 1;i <= varible_num;i++)
                    {
                        if(value[i] == 1)
                        printf("%d ",i);
                        else
                        printf("%d ",-i);
                    }
                    printf("\n");
                }
                else
                {
                    printf("�������޷����㣡\n");
                }
                printf("����ʱ�䣺%lf ms\n",duration * 1000);
                if(saveanswer(result,duration,value) == 1)
                printf("������Ѿ�������ͬ��res�ļ�\n");
                else
                printf("�������ʧ�ܣ�\n");
            }
            getchar();getchar();
            break;
        case 4:
            re = checkanswer(value,cls,result);
            if(re == TRUE)
            printf("����ȷ��\n");
            else if(re == FALSE) 
            printf("�𰸴���\n");
            else
            printf("û�ÿɹ���֤�Ĵ𰸣�\n");
            getchar();getchar();
            break;
        case 5:
            option = 1;
            while (option)
            {
                system("cls");
                printf("\n\n");
                printf("                 ������Ϸ\n");
                printf("--------------------------------------------------\n");
                printf("1.���ɹ����ļ�                2.��ʼ��Ϸ\n");
                printf("0.�˳�����\n");
                printf("--------------------------------------------------\n");
                printf("	��ѡ����Ĳ���[0~2]:");
                scanf("%d",&option);
                switch (option)
                {
                case 1:
                    createrule(cls);
                    getchar();getchar();
                    break;
                case 2:
                    value = (int *)malloc(sizeof(int) * (varible_num +1));
                    for(int i = 0;i <= varible_num;i++)
                    value[i] = -1;//ֻ�ǳ�ʼ��һ�£�û�и�ֵ���壡
                    a = clock();
            
            
                    createpuzzle(cls,value);
                    covernum = dig_hole(cls,value);
                    b = clock();
                    duration = (double)(b - a)/ CLOCKS_PER_SEC;
                    printf("������������ʱ��Ϊ: %lf ms\n",duration*1000);
                    while (1)
                    {
                        printf("������������������������:");
                        scanf("%d %d %d",&x , &y , &z);
                        if(x == 0 && y == 0 && z == 0)//û�������ֲ�������
                        break;
                        if(x <= 0 || x > 9 || y <= 0 || y > 9 || z > 9 || z <= 0)
                        {
                            printf("�����ʽ����\n");
                            continue;
                        }
                        if(dighole[x - 1][y - 1] == 1)//�޸ĵ���ʾ��λ��ȥ��
                        {
                            printf("�õ�Ϊ��Ŀ��ʾ�����������޸ģ�\n");
                            continue;
                        }
                        else if(chess[x - 1][y - 1] == 0)//�˴���û��д����
                        {
                            chess[x - 1][y - 1] = z;
                            covernum++;
                        }
                        else//�޸��Լ��������
                        {
                            chess[x - 1][y - 1] = z;
                        }
                        //�����ǰ����µ�����
                        printf("��������������������������������������������������������������������������\n");
                        for(int i = 0;i <= 8;i++)
                        {
        
                            for(int j = 0;j <= 8;j++)
                            {
                                if(j == 0)
                                printf("| ");
                                if(chess[i][j] != 0)
                                {                    
                                    printf("%d ",chess[i][j]);                   
                                }
                                else
                                printf("  ");
                                if(j != 8)
                                printf("| ");
                                else 
                                printf("|");
                            }   
                            printf("\n");
                            printf("��������������������������������������������������������������������������\n");
                        }
                        //�ȶ���������
                        if(covernum < 81 && chess[x - 1][y - 1] == puzzle[x - 1][y - 1])
                        printf("��ϲ�㣬���λ���������\n");
                        else if(chess[x - 1][y - 1] != puzzle[x - 1][y - 1])
                        {
                            printf("���ź��������������ˣ�\n");
                        }
                        else if(covernum == 81 && chess[x - 1][y - 1] == puzzle[x - 1][y - 1])
                        {
                            printf("��ϲ�㣡���Ѿ��ɹ����������������\n");
                            break;
                        }
                    }
                    printf("����enter�������");
                    getchar();getchar();
                    printf("��������������������������������������������������������������������������\n");
                    for(int i = 0;i <= 8;i++)
                    {
                        for(int j = 0;j <= 8;j++)
                        {
                            if(j == 0)
                            printf("| ");
                            printf("%d ",puzzle[i][j]);
                            if(j != 8)
                            printf("| ");
                            else
                            printf("|");
                        }
                        printf("\n");
                        printf("��������������������������������������������������������������������������\n");
                    }
                    getchar();getchar();
                    break;
                case 0:
                    break;
                }
            }
            getchar();
            break;
        case 0:
            break;
        }
    }
    return 0;
}
