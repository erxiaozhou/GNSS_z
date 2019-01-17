#include "stdlib.h"
#include "stdio.h"
#include "BOC_Fac.h"
#include "math.h"
#include "BPSK_Fac.h"
#include "time.h"

#define BUFF_SIZE 10000
#define COUNT 20
#define epsilon 0.001
#define pi 3.1415926535
//生成BOC的自相关序列
void Sel_boc_func(int a, int b, double* BUFF, int size, double *TM) {

	double A, B, SIZE;
	double p, k, temp;
	int i;

	A = a;
	B = b;
	SIZE = size;

	p = A / B;

	for (i = 0; i < size; i++) {
		TM[i] = -1.0 + (2.0 / SIZE) * i;
	}//生成时间序列

	for (i = 0; i < size; i++) {

		temp = fabs(TM[i]);
		k = 2.0 * p * temp;
		k = ceil(k);

		BUFF[i] = pow(-1.0, k + 1.0)*((1.0 / p)*(2.0 * k * p + k - p - pow(k, 2.0)) - (4.0 * p - 2.0 * k + 1.0) * fabs(TM[i]));
	}
}

//打印BOC的自相关函数
void Print_boc_self(double * BUFF, double * TM, int size) {

	int i;

	for (i = 0; i < size; i++) {

		printf("τ = %lf,\t Rτ(τ) = %lf\n", TM[i], BUFF[i]);
	}
}


void Create_boc(int a, int b, int * BUFF, double * TM, int size, int WIDTH, int *Ut) {

	int i, k;
    int n, m;
    int Ct[BUFF_SIZE];
	srand(time(NULL));
	k = 0;

	//生成Ut序列
	for (i = 0; i < size; i++) {

		if (k == 0) {
			Ut[i] = rand() % 2 ? -1 : 1;
			k++;
		}

		else if (k == WIDTH - 1) {
			Ut[i] = Ut[i - 1];
			k = 0;
		}

		else {
			Ut[i] = Ut[i - 1];
			k++;
		}
	}

    n = 2 * a / b;
	m = WIDTH / n;
	k = 0;
	//生成Ct序列
	for (i = 0; i < size; i++) {

		if (i == 0) {
			Ct[i] = 1;
			k++;
		}

		else if (k == 0) {
			Ct[i] = -Ct[i - 2];
			k++;
		}
		else if (k == m - 1) {
			Ct[i] = Ct[i - 1];
			k = 0;
		}
		else {
			k++;
			Ct[i] = Ct[i - 1];
		}
	}

	for (i = 0; i < size; i++) {

		BUFF[i] = Ut[i] * Ct[i];
	}
	//生成时间序列
	for (i = 0; i < size; i++) {
		TM[i] = 1 / (1023000.0 * b) / (WIDTH / n) * i;
	}

    for (i = 0; i < size; i++){
        printf("Ut = %d,Ct = %d,St = %d\n", Ut[i],Ct[i],BUFF[i]);
    }
}


void Print_boc(int * BUFF, double * TM, int size) {

	int i;
	for (i = 0; i < size; i++) {

		printf("t = %.10lf, S(t) = %d\n", TM[i], BUFF[i]);
	}
}



double OBOCF_(double f, double fc, double fs)//奇数功率谱算式
{
	if (f == 0) return fc / (4 * pow(fs, 2.0));
	else return fc * pow((tan((pi*f) / (2 * fs))*cos((pi*f) / fc)) / (pi*f), 2);
}



double EBOCF_(double f, double fc, double fs)//偶数功率谱算式
{
	if (f == 0) return 0;
	else return fc * pow((tan((pi*f) / (2 * fs))*sin((pi*f) / fc)) / (pi*f), 2);
}


void Create_boc_spectrum(int a, int b, double * BUFF, double *TM, int size) {

	double f;
	int i;
	double k;

	k = 30000000.0 / size;

	for (i = 0; i < size; i++)
	{
		f = -15000000.0 + k * i;

		BUFF[i] = 10 * log10(G_boc(f, a, b));
		TM[i] = f;
	}
}


void Print_boc_spectrum(double * BUFF, double * TM, int size) {

	int i;

	for (i = 0; i < size; i++) {
		printf("t = %.2lfHZ\t Gboc(f) = %lf\n", TM[i], BUFF[i]);
	}
}



double G_boc(double f, int a, int b) {

	double fs, fc;
	int N;
	double(*BOCF) (double f, double fc, double fs);

	fs = a * 1023000.0;
	fc = b * 1023000.0;

	N = 2 * fs / fc;

    if (N % 2) BOCF = OBOCF_;
    else BOCF = EBOCF_;

	return (*BOCF)(f, fc, fs);
}


double Lambda_boc(double Beta, double fs, double fc)
{
	double hign, low;

	int N;

	double(*BOCF) (double f, double fc, double fs);


	int m, n;//m控制迭代次数, 而n控制复化梯形积分的分点数. n=2^m
	double h, x;
	double s, q;
	double ep; //精度要求
	double y[COUNT];//为节省空间,只需一维数组
	double p;//p总是指示待计算元素的前一个元素(同一行)

	//迭代初值
	h = Beta;
	m = 1;
	n = 1;
	ep = epsilon + 1.0;

	hign = Beta / 2.0;
	low = -Beta / 2.0;

	N = 2 * fs / fc;

    if (N % 2) BOCF = OBOCF_;
    else BOCF = EBOCF_;

	y[0] = h * ((*BOCF) (low, fc, fs) + (*BOCF) (hign, fc, fs)) / 2.0;

	//迭代计算
	while ((ep >= epsilon) && (m < COUNT))
	{
		//复化积分公式求T2n(Romberg计算表中的第一列),n初始为1,以后倍增
		p = 0.0;
		for (int i = 0; i < n; i++)//求Hn
		{
			x = low + (i + 0.5)*h;
			p = p + (*BOCF) (x, fc, fs);
		}
		p = (y[0] + h * p) / 2.0;//求T2n = 1/2(Tn+Hn),用p指示

		//求第m行元素,根据Romberg计算表本行的前一个元素(p指示),
		//和上一行左上角元素(y[k-1]指示)求得.        
		s = 1.0;
		for (int k = 1; k <= m; k++)
		{
			s = 4.0*s;
			q = (s*p - y[k - 1]) / (s - 1.0);
			y[k - 1] = p;
			p = q;
		}

		p = fabs(q - y[m - 1]);
		m = m + 1;
		y[m - 1] = q;
		n = n + n; h = h / 2.0;
	}
	//printf("λ = %lf\n", q);
	return (q);
}


double Band_width_0_9_boc(int a, int b, double Br) {

	double lamda_30M, lamda;
	double Fx, fc, fs;
	int N;
	double(*BOCF) (double f, double fc, double fs);

    if(a == 5 && b == 2)Fx = 119000000.0;
    else if(a == 8 && b == 4)Fx = 18900000.0;
    else if(a == 10 && b == 5)Fx = 23600000.0;
    else Fx = 10000000.0;

	fc = b * 1023000.0;
	fs = a * 1023000.0;
	N = 2 * a / b;
	//迭代初值

	lamda_30M = Lambda_boc(30000000.0, fs, fc);

	lamda = 0;

    if (N % 2) BOCF = OBOCF_;
    else BOCF = EBOCF_;


	while (fabs(lamda - 0.9) > epsilon && lamda < 0.91) {

		Fx = Fx + 40000;
		lamda = Lambda_boc(Fx, fs, fc) / lamda_30M;
	}
	return Fx;
}

