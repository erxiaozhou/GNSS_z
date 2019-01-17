#include "math.h"

#define epsilon 0.001
#define COUNT 10


double Romberg(double B, double(*F)(double f)) {

	int m, n;//m���Ƶ�������, ��n���Ƹ������λ��ֵķֵ���. n=2^m
	double h, x;
	double s, q;
	double ep; //����Ҫ��
	double y[COUNT];//Ϊ��ʡ�ռ�,ֻ��һά����
	double p;//p����ָʾ������Ԫ�ص�ǰһ��Ԫ��(ͬһ��)
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
		for (int i = 0; i < n; i++)//��Hn
		{
			x = low + (i + 0.5)*h;
			p = p + F(x);
		}
		p = (y[0] + h * p) / 2.0;//��T2n = 1/2(Tn+Hn),��pָʾ 

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
