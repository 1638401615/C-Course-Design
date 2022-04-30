#ifndef _def_h_
#define _def_h_
#include<stdio.h>
#include<string.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#define OK 1
#define ERROR -1
#define TRUE 1
#define FALSE 0
#define satisfied 1
#define unsatisfied 0
#define UNFEASIBLE 2
typedef struct  Literals
{
    int data;
    struct Literals *next;
}Literals;//以文字构成的链表表示子句
typedef struct Clauses
{
    
    struct Literals *head;//指向此子句的第一个变量
    struct Clauses *nextclause;
}Clauses;//由一个个子句构成的公式


char filename[100];//文件名称
int clause_num;//子句数量
int varible_num;//变量个数
int result_num;//解的个数
int puzzle[9][9] = {0};//数独格局
int dighole[9][9] = {0};
    int chess[9][9] = {0};
//函数声明
int loadcnf(Clauses *&cls);                         //读入cnf文件中的数据到子句链表中
void cleanclause(Clauses *&cls);                    //清除所有子句
int eraseclause(Clauses *&cls , Clauses *&fcls);    //清除满足条件的某个子句
int eraseliteral(Literals *&lit,Literals *&flit);   //清除指定子句中的特定文字
int isunitclause(Literals *lit);                    //判断是否为单子句
//int evaluateclause(Literals *lit,int v[]);          //评估子句的真假
int isemptyclause(Clauses *cls);                    //判断是否有子句被删空了
int DPLL(Clauses *&cls,int value[]);                //利用dpll算法对子句进行化简运算
void copyclause(Clauses *&cls1 , Clauses *cls2);    //把cls2拷贝到cls1
int addclause(Clauses *&cls,Clauses *&cls_add);     //添加子句
int saveanswer(int result , double duration , int value[]);//保存答案到同名文件
int checkanswer(int value[] , Clauses *&cls , int result);        //检查SAT问题求解答案的正确与否
void createpuzzle(Clauses *&cls , int value[]);                                 //创建数独格局
int createrule(Clauses *&cls);
int dfs(Clauses *&cls , int value[]);               //求解答案个数
int dig_hole(Clauses *&cls , int value[]);                        //对生成的数独格局进行挖洞
#endif
