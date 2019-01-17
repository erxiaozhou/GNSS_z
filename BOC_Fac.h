#pragma once
#ifndef BOC1
#define BOC1


/**************************************************
����BOC���Ƶ�ʱ������
Ut����Ƶ������Ƶ����֮��U(t)��Ct�����ز�����C(t)��TM_Ser��S(t)���У�size�������С
***************************************************/
void Create_boc(int a, int b, int * BUFF, double * TM, int size, int WIDTH, int *Ut);


/****************************************************
��ӡS(t)����
*****************************************************/
void Print_boc(int * BUFF, double * TM, int size);

/****************************************************
����BOC�Ĺ����׺���
*****************************************************/
void Create_boc_spectrum(int a, int b, double * BUFF, double *TM, int size);

/****************************************************
��ӡBOC�Ĺ������ܶ�
*****************************************************/
void Print_boc_spectrum(double * BUFF, double * TM, int size);


/****************************************************
��������غ�������
a�Ǧ���b�Ǧ£�BUFF�Ǵ洢����غ��������У�size������Ĵ�С
*****************************************************/
void Sel_boc_func(int a, int b, double* BUFF, int size, double * TM);



/****************************************************
��ӡ����غ�������
******************************************************/
void Print_boc_self(double * BUFF, double * TM, int size);

/***************************************************
������޹���beta��Ԥ���ô���
****************************************************/
double Lambda_boc(double Beta, double fs, double fc);

//BOC�������ܶȵı��ʽ
double OBOCF_(double f, double fc, double fs);//������������ʽ
double EBOCF_(double f, double fc, double fs);//ż����������ʽ

//BOC�Ĺ������ܶȵı��ʽ
double G_boc(double f, int a, int b);

/**************************************************
����BOC��90%���ʵĴ���
***************************************************/
double Band_width_0_9_boc(int a, int b, double Br);


#endif // !BOC1
#pragma once
