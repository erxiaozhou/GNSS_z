#pragma once
#ifndef BOC1
#define BOC1


/**************************************************
生成BOC调制的时间序列
Ut是扩频码与扩频符号之积U(t)，Ct是亚载波序列C(t)，TM_Ser是S(t)序列，size是数组大小
***************************************************/
void Create_boc(int a, int b, int * BUFF, double * TM, int size, int WIDTH, int *Ut);


/****************************************************
打印S(t)序列
*****************************************************/
void Print_boc(int * BUFF, double * TM, int size);

/****************************************************
生成BOC的功率谱函数
*****************************************************/
void Create_boc_spectrum(int a, int b, double * BUFF, double *TM, int size);

/****************************************************
打印BOC的功率谱密度
*****************************************************/
void Print_boc_spectrum(double * BUFF, double * TM, int size);


/****************************************************
生成自相关函数序列
a是α，b是β，BUFF是存储自相关函数的序列，size是数组的大小
*****************************************************/
void Sel_boc_func(int a, int b, double* BUFF, int size, double * TM);



/****************************************************
打印自相关函数序列
******************************************************/
void Print_boc_self(double * BUFF, double * TM, int size);

/***************************************************
计算带限功率beta是预设置带宽
****************************************************/
double Lambda_boc(double Beta, double fs, double fc);

//BOC功率谱密度的表达式
double OBOCF_(double f, double fc, double fs);//奇数功率谱算式
double EBOCF_(double f, double fc, double fs);//偶数功率谱算式

//BOC的功率谱密度的表达式
double G_boc(double f, int a, int b);

/**************************************************
计算BOC的90%功率的带宽
***************************************************/
double Band_width_0_9_boc(int a, int b, double Br);


#endif // !BOC1
#pragma once
