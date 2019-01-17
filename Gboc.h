#pragma once
#include<stdio.h>
#define pi 3.14159269358
#define Ni 100000//功率谱取样点数
#define Nr 1000 //带限后自相关函数t取样点数
//#define down -1
//#define up 1
#define N 10000//积分点数

double OBOCF_2(double f,double fs,double fc);//奇数功率谱算式

double EBOCF_2(double f, double fs, double fc);//偶数功率谱算式

void EBOCf_(double fs, double fc, double * Gbocf);//n为偶数时的功率谱

void OBOCf_(double fs, double fc, double * Gbocf);//n为奇数时的功率谱

void BOCf(double fs, double fc, double *Gbocf);

double Lamuda(double Beta,double fs, double fc);//带限之后余下的功率部分

void RST(double Beta, double fs, double fc, double * Rst,double * Rst_index,double down,double up);//带限之后的自相关函数

double EJrst(double t, double Beta, double lamudaneed, double fs, double fc);//计算带限自相关函数时偶数情况下用到的积分式

double OJrst(double t, double Beta, double lamudaneed, double fs, double fc);//计算带限自相关函数时奇数情况下用到的积分式

double Brms(double Beta, double fs, double fc);//计算带限信号的均方根带宽

double Brect(double Beta, double fs, double fc, double *Gbocf);//计算功谱密度的有效矩形带宽

double Wideoutloss(double Beta, double fs, double fc);//带外的损失

double Maxgf(double Beta, double fs, double fc, double *DBGbocf);//主瓣最大功谱密度

double Fre_Offset(double fs,double fc,double *DBGbocf);//频谱主瓣距频带中心的频偏
