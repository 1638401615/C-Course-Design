
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
        printf("         基于DPLL算法的SAT求解程序\n");
        printf("--------------------------------------------------\n");
        printf("1.读取cnf文件                 2.输出子句\n");
        printf("3.求解并保存答案              4.检查答案\n");
        printf("5.数独游戏                    0.退出程序\n");
        printf("--------------------------------------------------\n");
        printf("	请选择你的操作[0~5]:");
        scanf("%d", &op);
		switch (op)
        {
        case 1:
            printf("请输入要读取的文件路径：\n");
            scanf("%s",filename);
            loadcnf(cls);
            getchar();getchar();
            break;
        case 2:
            if(cls == NULL)
            printf("还未读取cnf文件！\n");
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
            printf("文件未导入！\n");
            else
            {
                value = (int *)malloc(sizeof(int) * (varible_num +1));
                for(int i = 0;i <= varible_num;i++)
                value[i] = -1;//只是初始化一下，没有赋值含义！
                start = clock();
                result = DPLL(cls,value);
                finish = clock();
                duration = (double)(finish - start)/ CLOCKS_PER_SEC;//计算时间
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
                    printf("该算例无法满足！\n");
                }
                printf("计算时间：%lf ms\n",duration * 1000);
                if(saveanswer(result,duration,value) == 1)
                printf("求解结果已经保存至同名res文件\n");
                else
                printf("结果保存失败！\n");
            }
            getchar();getchar();
            break;
        case 4:
            re = checkanswer(value,cls,result);
            if(re == TRUE)
            printf("答案正确！\n");
            else if(re == FALSE) 
            printf("答案错误！\n");
            else
            printf("没用可供验证的答案！\n");
            getchar();getchar();
            break;
        case 5:
            option = 1;
            while (option)
            {
                system("cls");
                printf("\n\n");
                printf("                 数独游戏\n");
                printf("--------------------------------------------------\n");
                printf("1.生成规则文件                2.开始游戏\n");
                printf("0.退出程序\n");
                printf("--------------------------------------------------\n");
                printf("	请选择你的操作[0~2]:");
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
                    value[i] = -1;//只是初始化一下，没有赋值含义！
                    a = clock();
            
            
                    createpuzzle(cls,value);
                    covernum = dig_hole(cls,value);
                    b = clock();
                    duration = (double)(b - a)/ CLOCKS_PER_SEC;
                    printf("生成棋盘所花时间为: %lf ms\n",duration*1000);
                    while (1)
                    {
                        printf("请输入你想填入的坐标和数字:");
                        scanf("%d %d %d",&x , &y , &z);
                        if(x == 0 && y == 0 && z == 0)//没做出来又不想玩了
                        break;
                        if(x <= 0 || x > 9 || y <= 0 || y > 9 || z > 9 || z <= 0)
                        {
                            printf("输入格式错误！\n");
                            continue;
                        }
                        if(dighole[x - 1][y - 1] == 1)//修改到提示数位置去了
                        {
                            printf("该点为题目提示数，不可以修改！\n");
                            continue;
                        }
                        else if(chess[x - 1][y - 1] == 0)//此处还没填写数字
                        {
                            chess[x - 1][y - 1] = z;
                            covernum++;
                        }
                        else//修改自己填的数字
                        {
                            chess[x - 1][y - 1] = z;
                        }
                        //输出当前情况下的棋盘
                        printf("—————————————————————————————————————\n");
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
                            printf("—————————————————————————————————————\n");
                        }
                        //比对数字正误
                        if(covernum < 81 && chess[x - 1][y - 1] == puzzle[x - 1][y - 1])
                        printf("恭喜你，这个位置填对啦！\n");
                        else if(chess[x - 1][y - 1] != puzzle[x - 1][y - 1])
                        {
                            printf("很遗憾，这个数字填错了！\n");
                        }
                        else if(covernum == 81 && chess[x - 1][y - 1] == puzzle[x - 1][y - 1])
                        {
                            printf("恭喜你！你已经成功做出了这个数独！\n");
                            break;
                        }
                    }
                    printf("按下enter键输出答案");
                    getchar();getchar();
                    printf("—————————————————————————————————————\n");
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
                        printf("—————————————————————————————————————\n");
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
