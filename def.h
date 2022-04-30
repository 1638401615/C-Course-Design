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
}Literals;//�����ֹ��ɵ������ʾ�Ӿ�
typedef struct Clauses
{
    
    struct Literals *head;//ָ����Ӿ�ĵ�һ������
    struct Clauses *nextclause;
}Clauses;//��һ�����Ӿ乹�ɵĹ�ʽ


char filename[100];//�ļ�����
int clause_num;//�Ӿ�����
int varible_num;//��������
int result_num;//��ĸ���
int puzzle[9][9] = {0};//�������
int dighole[9][9] = {0};
    int chess[9][9] = {0};
//��������
int loadcnf(Clauses *&cls);                         //����cnf�ļ��е����ݵ��Ӿ�������
void cleanclause(Clauses *&cls);                    //��������Ӿ�
int eraseclause(Clauses *&cls , Clauses *&fcls);    //�������������ĳ���Ӿ�
int eraseliteral(Literals *&lit,Literals *&flit);   //���ָ���Ӿ��е��ض�����
int isunitclause(Literals *lit);                    //�ж��Ƿ�Ϊ���Ӿ�
//int evaluateclause(Literals *lit,int v[]);          //�����Ӿ�����
int isemptyclause(Clauses *cls);                    //�ж��Ƿ����Ӿ䱻ɾ����
int DPLL(Clauses *&cls,int value[]);                //����dpll�㷨���Ӿ���л�������
void copyclause(Clauses *&cls1 , Clauses *cls2);    //��cls2������cls1
int addclause(Clauses *&cls,Clauses *&cls_add);     //����Ӿ�
int saveanswer(int result , double duration , int value[]);//����𰸵�ͬ���ļ�
int checkanswer(int value[] , Clauses *&cls , int result);        //���SAT�������𰸵���ȷ���
void createpuzzle(Clauses *&cls , int value[]);                                 //�����������
int createrule(Clauses *&cls);
int dfs(Clauses *&cls , int value[]);               //���𰸸���
int dig_hole(Clauses *&cls , int value[]);                        //�����ɵ�������ֽ����ڶ�
#endif
