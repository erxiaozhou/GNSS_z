#include "math.h"

#define epsilon 0.001
#define COUNT 10


double Romberg(double B, double(*F)(double f)) {

	int m, n;//m控制迭代次数, 而n控制复化梯形积分的分点数. n=2^m
	double h, x;
	double s, q;
	double ep; //精度要求
	double y[COUNT];//为节省空间,只需一维数组
	double p;//p总是指示待计算元素的前一个元素(同一行)
	double hign, low;

	h = B;
	hign = B / 2.0;
	low = -B / 2.0;
	y[0] = h * (F(hign) + F(low)) / 2.0;
	m = 1;
	n = 1;
	ep = epsilon + 1.0;

	while ((ep >= epsilon) && (m < COUNT))
	{
		p = 0.0;
		for (int i = 0; i < n; i++)//求Hn
		{
			x = low + (i + 0.5)*h;
			p = p + F(x);
		}
		p = (y[0] + h * p) / 2.0;//求T2n = 1/2(Tn+Hn),用p指示 

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
	return (q);
}
