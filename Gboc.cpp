#include<stdio.h>
#include<math.h>
#include"Gboc.h"
//#define F(f) 10*(log10(fc*pow((tan((pi*f) / (2 * fs))*cos((pi*f) / fc)) / (pi*f), 2)))

//--------��������ʽ-------------//
double OBOCF_2(double f,double fs,double fc)//������������ʽ
{
    if (f == 0) return fc / (4 * pow(fs, 2.0));//�������nan
    return fc*pow((tan((pi*f) / (2 * fs))*cos((pi*f) / fc)) / (pi*f), 2);
}

double EBOCF_2(double f, double fs, double fc)//ż����������ʽ
{
    if (f == 0) return 0;
    return fc*pow((tan((pi*f) / (2 * fs))*sin((pi*f) / fc)) / (pi*f), 2);
}

double CEJRST(double f, double t, double lamudaneed, double fs, double fc)//nΪż��ʵ��������ʽ
{
	return lamudaneed * fc*pow((tan((pi*f) / (2 * fs))*sin((pi*f) / fc)) / (pi*f), 2)*cos(2 * pi*f*t);
}

double SEJRST(double f, double t, double lamudaneed, double fs, double fc)//nΪż���鲿������ʽ
{
	return lamudaneed * fc*pow((tan((pi*f) / (2 * fs))*sin((pi*f) / fc)) / (pi*f), 2)*sin(2 * pi*f*t);
}

double COJRST(double f, double t, double lamudaneed, double fs, double fc)//nΪ����ʵ��������ʽ
{
	return lamudaneed * fc*pow((tan((pi*f) / (2 * fs))*cos((pi*f) / fc)) / (pi*f), 2)*cos(2 * pi*f*t);
}

double SOJRST(double f, double t, double lamudaneed, double fs, double fc)//nΪ�����鲿������ʽ
{
	return lamudaneed * fc*pow((tan((pi*f) / (2 * fs))*cos((pi*f) / fc)) / (pi*f), 2)*sin(2 * pi*f*t);
}

double OBRMS(double f, double lamudaneed, double fs, double fc)//��������غ���nΪ��ʱ����
{
	return pow(f, 2)*lamudaneed*fc*pow((tan((pi*f) / (2 * fs))*cos((pi*f) / fc)) / (pi*f), 2);
}

double EBRMS(double f, double lamudaneed, double fs, double fc)//��������غ���nΪż��ʱ����
{
	return pow(f, 2)*lamudaneed*fc*pow((tan((pi*f) / (2 * fs))*sin((pi*f) / fc)) / (pi*f), 2);
}

//---------�����׼���-------------//
void EBOCf_(double fs, double fc, double * DBGbocf)//nΪż��ʱboc�����׼���
{
	double f;
	int i;

	for (i = 0; i < Ni; i++)
	{
		f = (i - Ni/2) * (30000000/Ni);//��30MhzƵ�ʰ������С����ΪС��
	
		if(f == 0)
		{
			DBGbocf[i] = DBGbocf[i - 1];//�����������С
		}
		else
		{
			DBGbocf[i] = 10 * (log10(fc*pow((tan((pi*f) / (2 * fs))*sin((pi*f) / fc)) / (pi*f), 2)));

		}


	}
}

void OBOCf_(double fs, double fc, double * DBGbocf)//nΪ����ʱboc�����׼���
{
	double f;
	int i;

	for (i = 0; i < Ni; i++)
	{
		f = (i - Ni/2) * (30000000/Ni);
		
		if (f == 0)
		{
			DBGbocf[i]= DBGbocf[i-1];//��������������ֵ
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

//----------���޹��ʼ���------------//
/*double Lamuda(double aa, double bb, double fs,double fc)
{ //aa,bb ����������
	int m, n,l;//m���Ƶ�������, ��n���Ƹ������λ��ֵķֵ���. n=2^m
	double h, x;
	double s, q;
	double ep; //����Ҫ��
	double y[COUNT];//Ϊ��ʡ�ռ�,ֻ��һά����
	//ÿ��ѭ�����δ洢Romberg������ÿ��Ԫ��,�Թ�������һ��,��������
	double p;//p����ָʾ������Ԫ�ص�ǰһ��Ԫ��(ͬһ��)

	//������ֵ
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

	//��������
	while ((ep >= epsilon) && (m < COUNT))
	{
		//�������ֹ�ʽ��T2n(Romberg������еĵ�һ��),n��ʼΪ1,�Ժ���
		p = 0.0;
		for (int i = 0; i < n; i++)//��Hn
		{
			x = aa + (i + 0.5)*h;
			p = p + BOCFL(x,fc,fs);
		}
		p = (y[0] + h * p) / 2.0;//��T2n = 1/2(Tn+Hn),��pָʾ

		//���m��Ԫ��,����Romberg������е�ǰһ��Ԫ��(pָʾ),
		//����һ�����Ͻ�Ԫ��(y[k-1]ָʾ)���.        
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
			if (Down == 0)//��ֹ����nan
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

//----------��������غ���----------//
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

//-----------��������غ����Ļ���--------------//
double EJrst(double t,double Beta,double lamudaneed, double fs, double fc)//�����������غ���ʱż��������õ��Ļ���ʽ
{
	double h, cejrst,sejrst, Down,Result;//DownΪһ���ݴ�ֵ
	int i, n;
	/*���㲽��*/
	h = (2 * Beta) / N;
	//-----����ʵ������----------//
	cejrst = CEJRST(-Beta,t,lamudaneed, fs, fc)*h;
	for (i = 1; i < N; i++)
	{
		Down = -Beta + i * h;
		if (Down == 0)//��ֹ����nan
		{
			cejrst = cejrst + 0;
		}
		else
		{
			cejrst = cejrst + CEJRST(Down,t,lamudaneed, fs, fc)*h;
		}

	}
	//------�����鲿����-----------//
	sejrst = SEJRST(-Beta, t, lamudaneed, fs, fc)*h;
	for (i = 1; i < N; i++)
	{
		Down = -Beta + i * h;
		if (Down == 0)//��ֹ����nan
		{
			sejrst = sejrst + 0;
		}
		else
		{
			sejrst = sejrst + SEJRST(Down,t,lamudaneed, fs, fc)*h;
		}

	}
	//-------��ʵ���鲿��ģ---------//
	Result = sqrt(pow(cejrst, 2) + pow(sejrst, 2));
	
	return Result;
}

double OJrst(double t, double Beta, double lamudaneed, double fs, double fc)//�����������غ���ʱ����������õ��Ļ���ʽ
{
	double h, cojrst, sojrst, Down, Result;//DownΪһ���ݴ�ֵ
	int i, n; 
	/*���㲽��*/
	h = (2 * Beta) / N;
	//-----����ʵ������----------//
	cojrst = COJRST(-Beta, t, lamudaneed, fs, fc)*h;
	for (i = 1; i < N; i++)
	{
		Down = -Beta + i * h;
		if (Down == 0)//��ֹ����nan
		{
			cojrst = cojrst + 0;
		}
		else
		{
			cojrst = cojrst + COJRST(Down, t, lamudaneed, fs, fc)*h;
		}

	}
	//------�����鲿����-----------//
	sojrst = SOJRST(-Beta, t, lamudaneed, fs, fc)*h;
	for (i = 1; i < N; i++)
	{
		Down = -Beta + i * h;
		if (Down == 0)//��ֹ����nan
		{
			sojrst = sojrst + 0;
		}
		else
		{
			sojrst = sojrst + SOJRST(Down, t, lamudaneed, fs, fc)*h;
		}

	}
	//-------��ʵ���鲿��ģ---------//
	Result = sqrt(pow(cojrst, 2) + pow(sojrst, 2));

	return Result;

}

//-----------�����źŵľ���������--------------//
double Brms(double Beta, double fs, double fc)
{
	double h, s, Down,lamudaneed;
	int i, n;

	lamudaneed = 1 / Lamuda(Beta, fs, fc);

	n = (2 * fs) / fc;
	/*���㲽��*/
	h = (2 * Beta) / N;
	if (n % 2 == 0)
	{
		s = EBRMS(-Beta,lamudaneed, fs, fc)*h;
		for (i = 1; i < N; i++)
		{
			Down = -Beta + i * h;
			if (Down == 0)//��ֹ����nan
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

//-----------�����ܶȵ���Ч���δ���------------//
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

//-----------�������ʧ-------------//
double Wideoutloss(double Beta, double fs, double fc)
{
	double lamudaout,lamudaout1,lamudaout2;
	lamudaout1 = Lamuda(Beta,fs,fc);
	lamudaout2 = Lamuda(15000000,fs,fc);
	lamudaout = lamudaout1 / lamudaout2;
	return -10 * log10(lamudaout);
}

//-----------����������ܶ�-----------//
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

//-----------Ƶ�������Ƶ�����ĵ�Ƶƫ-------------//
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
