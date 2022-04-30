
/**************************************
 * �������ƣ�createrule
 * ������Clause*& int[]
 * ����: ����dpll�㷨���sat����,����𰸸���
 * ����ֵ��Ψһ�ⷵ��1�����򷵻�0
 *************************************/
int createrule(Clauses *&cls)
{
    int x , y , z , i , j , k , l;
    FILE *fp;
    fp = fopen("D:\\sudoku_rule.txt", "w");
    fprintf(fp, "p cnf 729 10287\n");
    for (x = 0; x < 9; ++x)
        for (y = 0; y < 9; ++y)
            for (z = 1; z <= 8; ++z) {
                for (i = z + 1; i <= 9; ++i)
                    fprintf(fp, "%d %d 0\n", -(81 * x + 9 * y + z), -(81 * x + 9 * y + i));//ÿ��λ�ã�����1~9�������һ��
            }
    for (x = 0; x < 9; ++x)
        for (z = 1; z <= 9; ++z)
            for (y = 0; y < 8; ++y) {
                for (i = y + 1; i < 9; ++i)
                    fprintf(fp, "%d %d 0\n", -(81 * x + 9 * y + z), -(81 * x + 9 * i + z));//ÿһ�У�����1~9�������һ��
            }
    for (y = 0; y < 9; ++y)
        for (z = 1; z <= 9; ++z)
            for (x = 0; x < 8; ++x) {
                for (i = x + 1; i < 9; ++i)
                    fprintf(fp, "%d %d 0\n", -(81 * x + 9 * y + z), -(81 * i + 9 * y + z));//ÿһ�У�����1~9�������һ��
            }
    for (z = 1; z <= 9; ++z)
        for (i = 0; i < 3; ++i)
            for (j = 0; j < 3; ++j) {
                for (x = 0; x < 3; ++x)
                    for (y = 0; y < 3; ++y)
                        fprintf(fp, "%d ", 81 * (3 * i + x) + 9 * (3 * j + y) + z);//����1~9��ÿ��3��3���������ٳ���һ��
                fprintf(fp, "0\n");
                for (x = 0; x < 3; ++x) {
                    for (y = 0; y < 3; ++y) {
                        for (k = x + 1; k < 3; ++k)
                            for (l = 0; l < 3; ++l)
                                if (l != y)
                                    fprintf(fp, "%d %d 0\n", -(81 * (3 * i + x) + 9 * (3 * j + y) + z),
                                            -(81 * (3 * i + k) + 9 * (3 * j + l) + z));//����1~9��ÿ��3��3�������������һ��
                    }
                }
            }
    fclose(fp);
    strcpy(filename,"D:\\sudoku_rule.txt");
    FILE *fp1;
    char ch;
    char chs[30];
    Clauses *cp;
    Literals *lp;
    int num;
    fp1 = fopen(filename,"r");
    if(fp1 == NULL)
    {
        printf("�ļ���ʧ�ܣ�\n");
        return 0;
    }
    while ((ch = getc(fp1)) == 'c')//��������Ϊע���е�ʱ��
    {
        while ((ch = getc(fp1)) != '\n')//�ظ����룬������������ֱ������
        {
            continue;
        }
    }
    fscanf(fp1,"%[ cnf ]",chs);    //��ȡcnf����ַ���
    fscanf(fp1,"%d",&varible_num); //��ȡ��������
    fscanf(fp1,"%d",&clause_num);  //��ȡ�־����
    cls = (Clauses *)malloc(sizeof(Clauses));
    cls ->nextclause = NULL;
    cls->head = (Literals *)malloc(sizeof(Literals));
    cls->head->next = NULL;
    cp = cls;      //ָ���һ���Ӿ� 
    lp = cls->head;//ָ���һ���Ӿ�ĵ�һ������

    for(int i = 1;i <= clause_num;i++)
    {
        
        fscanf(fp1,"%d",&num);
        while (num != 0)
        {
            lp->data = num;
            
            fscanf(fp1,"%d",&num);
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
    fclose(fp1);
    return 1;
}
/**************************************
 * �������ƣ�dfs
 * ������Clause*& int[]
 * ����: ����dpll�㷨���sat����,����𰸸���
 * ����ֵ��Ψһ�ⷵ��1�����򷵻�0
 *************************************/
int dfs(Clauses *&cls,int value[])
{
    Clauses *clsp1 = cls , *clsp2 = cls , *clsp3;
    Literals *lp;
    int *count;
    int re;
    count = (int *)malloc(sizeof(int) * (2 * varible_num + 1));
find:    while(clsp1 != NULL && isunitclause(clsp1->head) == 0)
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
            result_num++;
            
            return satisfied;
        }
        if(isemptyclause(cls))//�����ڿ��Ӿ䣬˵�����Ӿ��޷�����,��cnf�޷�����
        {
            free(count);
            cleanclause(cls);
            return unsatisfied;
        }
        clsp1 = cls;
        goto find;//���ؼ������ù��򻯼�cnf
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
    free(count);
    //����һ�������ֵĵ��Ӿ䣬��ӵ��Ӿ������н��л���
    clsp2 = (Clauses *)malloc(sizeof(Clauses));
    clsp2->head = (Literals *)malloc(sizeof(Literals));
    clsp2->head->next = NULL;
    clsp2->head->data = word;
    clsp2->nextclause = NULL;
    copyclause(clsp3,cls);
    addclause(clsp3,clsp2);
    if(result_num > 1)
    {
        
        return 0;
    }
    if(dfs(clsp3,value) == 0)//�����һ��֧����
    return 0;
    cleanclause(clsp3);//�ͷ��ڴ�
    if(result_num > 1)
    {
        
        return 0;
    }
    //��һ��֧�����꣬���ص��ڶ���֧��������
    clsp2 = (Clauses *)malloc(sizeof(Clauses));
    clsp2->head = (Literals *)malloc(sizeof(Literals));
    clsp2->head->next = NULL;
    clsp2->head->data = -word;
    clsp2->nextclause = NULL;
    copyclause(clsp3,cls);
    addclause(clsp3,clsp2);
    if(dfs(clsp3,value) == 0)//����ڶ���֧
    return 0;
    cleanclause(clsp3);
    //cleanclause(cls);
    if(result_num > 1)
    {
        
        return 0;
    }
    return 1;
}

/**************************************
 * �������ƣ�createpuzzle
 * ������Clauses *& int[]
 * ����: ������������
 * ����ֵ����
 *************************************/
void createpuzzle(Clauses *&cls , int value[])
{
    
    int x, y, z, i, j, k, l;
    int dig_order[82], index;
    
    Clauses *clsp1 = NULL, *clsp2 = NULL;
    int cnt = 0;//��¼�����������
    copyclause(clsp1,cls);//�ȿ���һ����������
    srand((unsigned int)time(0));
    while(cnt != 4)//�ĸ��������һ����Ⲣ��û���޽���������ֵ
    {
        x = rand() % 9;
        y = rand() % 9;
        z = rand() % 8 + 1;
        if(puzzle[x][y] == 0)
        {
            puzzle[x][y] = z;
            cnt++;
        }
        clsp2 = (Clauses *)malloc(sizeof(Clauses));
        clsp2->head = (Literals *)malloc(sizeof(Literals));
        clsp2->head->next = NULL;
        clsp2->head->data = x * 81 + y * 9 + z;
        clsp2->nextclause = NULL;
        addclause(clsp1,clsp2);
    }
    cnt = 0;
    DPLL(clsp1,value);
    cleanclause(clsp1);//��ʱ�ͷ��ڴ�
    for(x = 0;x <= 8;x++)//�����ȸ�ִ�������
    {
        for(y = 0;y <= 8;y++)
        {
            for(z = 1;z <= 9;z++)
            if(value[x * 81 + y * 9 + z] == 1)
            puzzle[x][y] = z;
        }
    }
}


/**************************************
 * �������ƣ�dighole
 * ������Clauses *& int[]
 * ����: �����ɵ����������ڶ�
 * ����ֵ����
 *************************************/
int dig_hole(Clauses *&cls , int value[])
{
    clock_t begin , finish;
    double t;
    int x , y , z;
    int cnt = 0;
    srand((unsigned int)time(0));
    Clauses *clsp1  = NULL, *clsp2 = NULL;//clsp1����copy cls��clsp2��������Ӿ�;
    copyclause(clsp1,cls);
    do{
        result_num = 0;
       /* if(clsp1 != NULL)//��������һ��ѭ����clsp1û��ʵ��Ψһ�⣬���clsp1��dighole������һ���ڶ�
        {
            cleanclause(clsp1);
            for(int i = 0;i <= 9;i++)
                for(int j = 0;j <= 8;j++)
                    dighole[x][y] = 0;
        }*/
            x = rand() % 9;
            y = rand() % 9;
            if(dighole[x][y] == 0)
            {
                dighole[x][y] = 1;
                
                clsp2 = (Clauses *)malloc(sizeof(Clauses));
                clsp2->head = (Literals *)malloc(sizeof(Literals));
                clsp2->head->next = NULL;
                clsp2->head->data = x * 81 + y * 9 + puzzle[x][y];
                clsp2->nextclause = NULL;
                addclause(clsp1,clsp2);
            }
        
    }while(dfs(clsp1,value) != 1);
    begin = clock();
    DPLL(clsp1,value);
    finish = clock();
    t = (double)(finish - begin)/CLOCKS_PER_SEC;
    
    cleanclause(cls);
    cleanclause(clsp1);
    printf("��������������������������������������������������������������������������\n");
    for(int i = 0;i <= 8;i++)
    {
        
        for(int j = 0;j <= 8;j++)
        {
            if(j == 0)
            printf("| ");
            if(dighole[i][j] == 1)
            {
                chess[i][j] = puzzle[i][j];
                printf("%d ",puzzle[i][j]);
                cnt++;
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
    printf("������̻���ʱ��Ϊ��%lf ms\n",t*1000);
    printf("�ڶ�����%d\n",cnt);
    
    
    return cnt;
}