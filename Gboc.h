#pragma once
#include<stdio.h>
#define pi 3.14159269358
#define Ni 100000//������ȡ������
#define Nr 1000 //���޺�����غ���tȡ������
//#define down -1
//#define up 1
#define N 10000//���ֵ���

double OBOCF_2(double f,double fs,double fc);//������������ʽ

double EBOCF_2(double f, double fs, double fc);//ż����������ʽ

void EBOCf_(double fs, double fc, double * Gbocf);//nΪż��ʱ�Ĺ�����

void OBOCf_(double fs, double fc, double * Gbocf);//nΪ����ʱ�Ĺ�����

void BOCf(double fs, double fc, double *Gbocf);

double Lamuda(double Beta,double fs, double fc);//����֮�����µĹ��ʲ���

void RST(double Beta, double fs, double fc, double * Rst,double * Rst_index,double down,double up);//����֮�������غ���

double EJrst(double t, double Beta, double lamudaneed, double fs, double fc);//�����������غ���ʱż��������õ��Ļ���ʽ

double OJrst(double t, double Beta, double lamudaneed, double fs, double fc);//�����������غ���ʱ����������õ��Ļ���ʽ

double Brms(double Beta, double fs, double fc);//��������źŵľ���������

double Brect(double Beta, double fs, double fc, double *Gbocf);//���㹦���ܶȵ���Ч���δ���

double Wideoutloss(double Beta, double fs, double fc);//�������ʧ

double Maxgf(double Beta, double fs, double fc, double *DBGbocf);//����������ܶ�

double Fre_Offset(double fs,double fc,double *DBGbocf);//Ƶ�������Ƶ�����ĵ�Ƶƫ
