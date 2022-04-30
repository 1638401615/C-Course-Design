
/**************************************
 * 函数名称：createrule
 * 参数：Clause*& int[]
 * 功能: 利用dpll算法求解sat问题,计算答案个数
 * 返回值：唯一解返回1，否则返回0
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
                    fprintf(fp, "%d %d 0\n", -(81 * x + 9 * y + z), -(81 * x + 9 * y + i));//每个位置，数字1~9至多出现一次
            }
    for (x = 0; x < 9; ++x)
        for (z = 1; z <= 9; ++z)
            for (y = 0; y < 8; ++y) {
                for (i = y + 1; i < 9; ++i)
                    fprintf(fp, "%d %d 0\n", -(81 * x + 9 * y + z), -(81 * x + 9 * i + z));//每一行，数字1~9至多出现一次
            }
    for (y = 0; y < 9; ++y)
        for (z = 1; z <= 9; ++z)
            for (x = 0; x < 8; ++x) {
                for (i = x + 1; i < 9; ++i)
                    fprintf(fp, "%d %d 0\n", -(81 * x + 9 * y + z), -(81 * i + 9 * y + z));//每一列，数字1~9至多出现一次
            }
    for (z = 1; z <= 9; ++z)
        for (i = 0; i < 3; ++i)
            for (j = 0; j < 3; ++j) {
                for (x = 0; x < 3; ++x)
                    for (y = 0; y < 3; ++y)
                        fprintf(fp, "%d ", 81 * (3 * i + x) + 9 * (3 * j + y) + z);//数字1~9在每个3×3数独中至少出现一次
                fprintf(fp, "0\n");
                for (x = 0; x < 3; ++x) {
                    for (y = 0; y < 3; ++y) {
                        for (k = x + 1; k < 3; ++k)
                            for (l = 0; l < 3; ++l)
                                if (l != y)
                                    fprintf(fp, "%d %d 0\n", -(81 * (3 * i + x) + 9 * (3 * j + y) + z),
                                            -(81 * (3 * i + k) + 9 * (3 * j + l) + z));//数字1~9在每个3×3数独中至多出现一次
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
        printf("文件打开失败！\n");
        return 0;
    }
    while ((ch = getc(fp1)) == 'c')//当读入行为注释行的时候
    {
        while ((ch = getc(fp1)) != '\n')//重复读入，抛弃改行内容直至换行
        {
            continue;
        }
    }
    fscanf(fp1,"%[ cnf ]",chs);    //读取cnf这个字符串
    fscanf(fp1,"%d",&varible_num); //读取变量个数
    fscanf(fp1,"%d",&clause_num);  //读取字句个数
    cls = (Clauses *)malloc(sizeof(Clauses));
    cls ->nextclause = NULL;
    cls->head = (Literals *)malloc(sizeof(Literals));
    cls->head->next = NULL;
    cp = cls;      //指向第一个子句 
    lp = cls->head;//指向第一个子句的第一个变量

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
    fclose(fp1);
    return 1;
}
/**************************************
 * 函数名称：dfs
 * 参数：Clause*& int[]
 * 功能: 利用dpll算法求解sat问题,计算答案个数
 * 返回值：唯一解返回1，否则返回0
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
            result_num++;
            
            return satisfied;
        }
        if(isemptyclause(cls))//若存在空子句，说明该子句无法满足,则cnf无法满足
        {
            free(count);
            cleanclause(cls);
            return unsatisfied;
        }
        clsp1 = cls;
        goto find;//返回继续利用规则化简cnf
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
    free(count);
    //建立一个该文字的单子句，添加到子句链表中进行化简
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
    if(dfs(clsp3,value) == 0)//进入第一分支搜索
    return 0;
    cleanclause(clsp3);//释放内存
    if(result_num > 1)
    {
        
        return 0;
    }
    //第一分支搜索完，返回到第二分支进行搜索
    clsp2 = (Clauses *)malloc(sizeof(Clauses));
    clsp2->head = (Literals *)malloc(sizeof(Literals));
    clsp2->head->next = NULL;
    clsp2->head->data = -word;
    clsp2->nextclause = NULL;
    copyclause(clsp3,cls);
    addclause(clsp3,clsp2);
    if(dfs(clsp3,value) == 0)//进入第二分支
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
 * 函数名称：createpuzzle
 * 参数：Clauses *& int[]
 * 功能: 创建数独棋盘
 * 返回值：无
 *************************************/
void createpuzzle(Clauses *&cls , int value[])
{
    
    int x, y, z, i, j, k, l;
    int dig_order[82], index;
    
    Clauses *clsp1 = NULL, *clsp2 = NULL;
    int cnt = 0;//记录随机变量个数
    copyclause(clsp1,cls);//先拷贝一份数独规则
    srand((unsigned int)time(0));
    while(cnt != 4)//四个随机数是一定多解并且没有无解情况的最大值
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
    cleanclause(clsp1);//及时释放内存
    for(x = 0;x <= 8;x++)//把数度格局存入棋盘
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
 * 函数名称：dighole
 * 参数：Clauses *& int[]
 * 功能: 对生成的数独进行挖洞
 * 返回值：无
 *************************************/
int dig_hole(Clauses *&cls , int value[])
{
    clock_t begin , finish;
    double t;
    int x , y , z;
    int cnt = 0;
    srand((unsigned int)time(0));
    Clauses *clsp1  = NULL, *clsp2 = NULL;//clsp1用来copy cls，clsp2用来添加子句;
    copyclause(clsp1,cls);
    do{
        result_num = 0;
       /* if(clsp1 != NULL)//若是在上一轮循环中clsp1没能实现唯一解，清空clsp1和dighole进行下一轮挖洞
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
    printf("—————————————————————————————————————\n");
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
        printf("—————————————————————————————————————\n");
    }
    printf("求解棋盘花费时间为：%lf ms\n",t*1000);
    printf("挖洞数：%d\n",cnt);
    
    
    return cnt;
}