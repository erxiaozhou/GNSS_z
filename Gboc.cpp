#include<stdio.h>
#include<math.h>
#include"Gboc.h"
//#define F(f) 10*(log10(fc*pow((tan((pi*f) / (2 * fs))*cos((pi*f) / fc)) / (pi*f), 2)))

//--------积分用算式-------------//
double OBOCF_2(double f,double fs,double fc)//奇数功率谱算式
{
    if (f == 0) return fc / (4 * pow(fs, 2.0));//避免出现nan
    return fc*pow((tan((pi*f) / (2 * fs))*cos((pi*f) / fc)) / (pi*f), 2);
}

double EBOCF_2(double f, double fs, double fc)//偶数功率谱算式
{
    if (f == 0) return 0;
    return fc*pow((tan((pi*f) / (2 * fs))*sin((pi*f) / fc)) / (pi*f), 2);
}

double CEJRST(double f, double t, double lamudaneed, double fs, double fc)//n为偶数实部积分算式
{
	return lamudaneed * fc*pow((tan((pi*f) / (2 * fs))*sin((pi*f) / fc)) / (pi*f), 2)*cos(2 * pi*f*t);
}

double SEJRST(double f, double t, double lamudaneed, double fs, double fc)//n为偶数虚部积分算式
{
	return lamudaneed * fc*pow((tan((pi*f) / (2 * fs))*sin((pi*f) / fc)) / (pi*f), 2)*sin(2 * pi*f*t);
}

double COJRST(double f, double t, double lamudaneed, double fs, double fc)//n为奇数实部积分算式
{
	return lamudaneed * fc*pow((tan((pi*f) / (2 * fs))*cos((pi*f) / fc)) / (pi*f), 2)*cos(2 * pi*f*t);
}

double SOJRST(double f, double t, double lamudaneed, double fs, double fc)//n为奇数虚部积分算式
{
	return lamudaneed * fc*pow((tan((pi*f) / (2 * fs))*cos((pi*f) / fc)) / (pi*f), 2)*sin(2 * pi*f*t);
}

double OBRMS(double f, double lamudaneed, double fs, double fc)//带限自相关函数n为奇时计算
{
	return pow(f, 2)*lamudaneed*fc*pow((tan((pi*f) / (2 * fs))*cos((pi*f) / fc)) / (pi*f), 2);
}

double EBRMS(double f, double lamudaneed, double fs, double fc)//带限自相关函数n为偶数时计算
{
	return pow(f, 2)*lamudaneed*fc*pow((tan((pi*f) / (2 * fs))*sin((pi*f) / fc)) / (pi*f), 2);
}

//---------功率谱计算-------------//
void EBOCf_(double fs, double fc, double * DBGbocf)//n为偶数时boc功率谱计算
{
	double f;
	int i;

	for (i = 0; i < Ni; i++)
	{
		f = (i - Ni/2) * (30000000/Ni);//将30Mhz频率按数组大小划分为小段
	
		if(f == 0)
		{
			DBGbocf[i] = DBGbocf[i - 1];//避免出现无穷小
		}
		else
		{
			DBGbocf[i] = 10 * (log10(fc*pow((tan((pi*f) / (2 * fs))*sin((pi*f) / fc)) / (pi*f), 2)));

		}


	}
}

void OBOCf_(double fs, double fc, double * DBGbocf)//n为奇数时boc功率谱计算
{
	double f;
	int i;

	for (i = 0; i < Ni; i++)
	{
		f = (i - Ni/2) * (30000000/Ni);
		
		if (f == 0)
		{
			DBGbocf[i]= DBGbocf[i-1];//避免出现无意义的值
		}
		else
		{
			DBGbocf[i] = 10*(log10(fc*pow((tan((pi*f) / (2 * fs))*cos((pi*f) / fc)) / (pi*f), 2)));

		}
		
	}
}

void BOCf(double fs, double fc, double *DBGbocf)
{
	int n;
	n = (2 * fs) / fc;
	if ((n % 2 == 0))
	{
        EBOCf_(fs, fc, DBGbocf);
	}
	else
	{
        OBOCf_(fs, fc, DBGbocf);
	}
}

//----------带限功率计算------------//
/*double Lamuda(double aa, double bb, double fs,double fc)
{ //aa,bb 积分上下限
	int m, n,l;//m控制迭代次数, 而n控制复化梯形积分的分点数. n=2^m
	double h, x;
	double s, q;
	double ep; //精度要求
	double y[COUNT];//为节省空间,只需一维数组
	//每次循环依次存储Romberg计算表的每行元素,以供计算下一行,算完后更新
	double p;//p总是指示待计算元素的前一个元素(同一行)

	//迭代初值
	l = 2 * fs / fs;

	double(*BOCFL)(double f, double fc, double fs);

	

	if (l % 2 == 0)
	{
		BOCFL = EBOCF;
	}
	else
	{
		BOCFL = OBOCF;
	}
	h = bb - aa;
	y[0] = h * (BOCFL(aa,fc,fs)+BOCFL(bb,fc,fs)) / 2.0;
	m = 1;
	n = 1;
	ep = epsilon + 1.0;

	//迭代计算
	while ((ep >= epsilon) && (m < COUNT))
	{
		//复化积分公式求T2n(Romberg计算表中的第一列),n初始为1,以后倍增
		p = 0.0;
		for (int i = 0; i < n; i++)//求Hn
		{
			x = aa + (i + 0.5)*h;
			p = p + BOCFL(x,fc,fs);
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
	return (q);
}
*/

double Lamuda(double Beta,double fs,double fc)
{
	double h, s,Down;
	int i,n;
	n = (2 * fs) / fc;
	
	h = (2*Beta) / N;
	if(n % 2 == 0)
	{
        s = EBOCF_2(-Beta,fs,fc)*h;
		for (i = 1; i < N; i++)
		{
			Down = -Beta + i * h;
			if (Down == 0)//防止出现nan
			{
				s = s + 0;
			}
			else
			{
                s = s + EBOCF_2(Down, fs, fc)*h;
			}
			
		}
	}
	else
	{
        s = OBOCF_2(-Beta, fs, fc)*h;
		for (i = 1; i < N; i++)
		{
			Down = -Beta + i * h;
			if (Down == 0)
			{
				s = s + fc / (4 * fs*fs);
			}
			else
			{
                s = s + OBOCF_2(Down, fs, fc)*h;
			}
			
		}
	}
	return s;
}

//----------带限自相关函数----------//
void RST(double Beta, double fs, double fc, double * Rst,double* Rst_index,double down,double up)
{
	double lamudaneed,t;
	int i,n;
	
	n = (2 * fs) / fc;

	lamudaneed = 1 / Lamuda(Beta,fs,fc);
	
	if (n % 2 == 0)
	{
		for (i = 0; i < Nr; i++)
		{
			t = down + i * ((up - down) / Nr);
			Rst_index[i] = t;
			Rst[i] = EJrst(t,Beta,lamudaneed, fs, fc);
		
		}
	}
	else
	{
		for (i = 0; i < Nr; i++)
		{
			t = down + i * ((up - down) / Nr);
			Rst_index[i] = t;
			Rst[i] = OJrst(t,Beta,lamudaneed, fs, fc);
		}
	}
}

//-----------带限自相关函数的积分--------------//
double EJrst(double t,double Beta,double lamudaneed, double fs, double fc)//计算带限自相关函数时偶数情况下用到的积分式
{
	double h, cejrst,sejrst, Down,Result;//Down为一个暂存值
	int i, n;
	/*计算步长*/
	h = (2 * Beta) / N;
	//-----计算实部积分----------//
	cejrst = CEJRST(-Beta,t,lamudaneed, fs, fc)*h;
	for (i = 1; i < N; i++)
	{
		Down = -Beta + i * h;
		if (Down == 0)//防止出现nan
		{
			cejrst = cejrst + 0;
		}
		else
		{
			cejrst = cejrst + CEJRST(Down,t,lamudaneed, fs, fc)*h;
		}

	}
	//------计算虚部积分-----------//
	sejrst = SEJRST(-Beta, t, lamudaneed, fs, fc)*h;
	for (i = 1; i < N; i++)
	{
		Down = -Beta + i * h;
		if (Down == 0)//防止出现nan
		{
			sejrst = sejrst + 0;
		}
		else
		{
			sejrst = sejrst + SEJRST(Down,t,lamudaneed, fs, fc)*h;
		}

	}
	//-------对实部虚部求模---------//
	Result = sqrt(pow(cejrst, 2) + pow(sejrst, 2));
	
	return Result;
}

double OJrst(double t, double Beta, double lamudaneed, double fs, double fc)//计算带限自相关函数时奇数情况下用到的积分式
{
	double h, cojrst, sojrst, Down, Result;//Down为一个暂存值
	int i, n; 
	/*计算步长*/
	h = (2 * Beta) / N;
	//-----计算实部积分----------//
	cojrst = COJRST(-Beta, t, lamudaneed, fs, fc)*h;
	for (i = 1; i < N; i++)
	{
		Down = -Beta + i * h;
		if (Down == 0)//防止出现nan
		{
			cojrst = cojrst + 0;
		}
		else
		{
			cojrst = cojrst + COJRST(Down, t, lamudaneed, fs, fc)*h;
		}

	}
	//------计算虚部积分-----------//
	sojrst = SOJRST(-Beta, t, lamudaneed, fs, fc)*h;
	for (i = 1; i < N; i++)
	{
		Down = -Beta + i * h;
		if (Down == 0)//防止出现nan
		{
			sojrst = sojrst + 0;
		}
		else
		{
			sojrst = sojrst + SOJRST(Down, t, lamudaneed, fs, fc)*h;
		}

	}
	//-------对实部虚部求模---------//
	Result = sqrt(pow(cojrst, 2) + pow(sojrst, 2));

	return Result;

}

//-----------带限信号的均方根带宽--------------//
double Brms(double Beta, double fs, double fc)
{
	double h, s, Down,lamudaneed;
	int i, n;

	lamudaneed = 1 / Lamuda(Beta, fs, fc);

	n = (2 * fs) / fc;
	/*计算步长*/
	h = (2 * Beta) / N;
	if (n % 2 == 0)
	{
		s = EBRMS(-Beta,lamudaneed, fs, fc)*h;
		for (i = 1; i < N; i++)
		{
			Down = -Beta + i * h;
			if (Down == 0)//防止出现nan
			{
				s = s + 0;
			}
			else
			{
				s = s + EBRMS(Down,lamudaneed, fs, fc)*h;
			}

		}
	}
	else
	{
		s = OBRMS(-Beta,lamudaneed, fs, fc)*h;
		for (i = 1; i < N; i++)
		{
			Down = -Beta + i * h;
			if (Down == 0)
			{
				s = s + fc / (4 * fs*fs);
			}
			else
			{
				s = s + OBRMS(Down,lamudaneed, fs, fc)*h;
			}
		}
	}
	return sqrt(s);
}

//-----------功谱密度的有效矩形带宽------------//
double Brect(double Beta,double fs, double fc, double *DBGbocf)
{
	double lamudaneed;
	int i,k,x;
	double j,y;
	lamudaneed = Lamuda(Beta, fs, fc);
	BOCf(fs, fc, DBGbocf);
	
	j = DBGbocf[0];
	for (i = 0; i < Ni; i++)
	{
		if (DBGbocf[i] > j)
		{
			j = DBGbocf[i];
			k = i;
		}
	}
	y = DBGbocf[k];
	return lamudaneed / (pow(10,DBGbocf[k]/10));
}

//-----------带外的损失-------------//
double Wideoutloss(double Beta, double fs, double fc)
{
	double lamudaout,lamudaout1,lamudaout2;
	lamudaout1 = Lamuda(Beta,fs,fc);
	lamudaout2 = Lamuda(15000000,fs,fc);
	lamudaout = lamudaout1 / lamudaout2;
	return -10 * log10(lamudaout);
}

//-----------主瓣最大功谱密度-----------//
double Maxgf(double Beta, double fs, double fc, double *DBGbocf)
{
	int i,k;
	double y,j;
	BOCf(fs, fc, DBGbocf);
	j = DBGbocf[0];
	for (i = 0; i < Ni; i++)
	{
		if (DBGbocf[i] >= j)
		{
			j = DBGbocf[i];
			k = i;
		}
	}
	y = DBGbocf[k];
	return y;
}

//-----------频谱主瓣句频带中心的频偏-------------//
double Fre_Offset(double fs,double fc,double *DBGbocf)
{
	int i, k;
	double y, j,R,t,r;
	BOCf(fs, fc, DBGbocf);
	j = DBGbocf[0];
	for (i = 0; i < Ni; i++)
	{
		if (DBGbocf[i] >= j)
		{
			j = DBGbocf[i];
			k = i;
		}
	}
	t = k - (Ni / 2);
	r = fabs(t);
	R=((r * 30000000 / Ni)/1000000);
	return R;
}
