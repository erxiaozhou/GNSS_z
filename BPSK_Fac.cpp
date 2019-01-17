#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "BPSK_Fac.h"
#include "time.h"



#define F 5000000.0
#define pi  3.14159265358979323846
#define BUFF_SIZE 3000
#define COUNT 20
#define epsilon 0.001


//bpsk�Ĺ������ܶȵı��ʽ
double G_bpsk(double f, double Fc) {

    if (f == 0) return 1.0 / Fc;
    else return (1 / Fc)* pow(sin(pi * f / Fc) / (pi * f / Fc), 2.0);
}
//bpsk������غ������ʽ
double R_bpsk(double t, double tc) {

    return  1.0 - fabs(t) / tc;
}



//bpsk����غ���ʣ�๦�ʵ�ʵ�����ʽ
double Rs_bpsk_r(double LAMDA, double fc, double f, double tm) {

    if (f == 0) return 1.0 / LAMDA * 1 / fc;
    else return (1 / LAMDA) * (1 / fc)* pow(sin(pi * f / fc) / (pi * f / fc), 2.0) * cos(2 * pi * f * tm);
}



//bpsk����غ���ʣ�๦�ʵ��鲿��ʾ
double Rs_bpsk_i(double LAMDA, double fc, double f, double tm) {

    if (f == 0) return 0;
    else return (1 / LAMDA) * (1 / fc)* pow(sin(pi * f / fc) / (pi * f / fc), 2.0) * sin(2 * pi * f * tm);
}



//����bpsk��ʣ�๦�ʵ�����غ�����ʵ������
double inte_Rs_bpsk_r(double BR, double LAMDA, double fc, double f, double tm) {

    double hign, low;

    int i, n;
    double h, t0, t, g;
    n = 1;                             //����ֵ

    hign = BR / 2.0;
    low = -BR / 2.0;
    h = BR / 2;

    t = h * (Rs_bpsk_r(LAMDA, fc, low, tm) + Rs_bpsk_r(LAMDA, fc, hign, tm));
    do
    {
        t0 = t;
        g = 0;
        for (i = 1; i <= n; i++)
            g += Rs_bpsk_r(LAMDA, fc, (low + (2 * i - 1)*h), tm);
        t = (t0 / 2) + (h*g);                //�������ι�ʽ
        n *= 2;
        h /= 2;
    } while (fabs(t - t0) > 0.01);             //�Զ��������e

    return t;
}

//����bpsk��ʣ�๦�ʵ�����غ������鲿����
double inte_Rs_bpsk_i(double BR, double LAMDA, double fc, double f, double tm) {

    double hign, low;
    int i, n;
    double h, t0, t, g;
    n = 1;                             //����ֵ

    hign = BR / 2.0;
    low = -BR / 2.0;
    h = BR / 2;

    t = h * (Rs_bpsk_i(LAMDA, fc, low, tm) + Rs_bpsk_i(LAMDA, fc, hign, tm));
    do
    {
        t0 = t;
        g = 0;
        for (i = 1; i <= n; i++)
            g += Rs_bpsk_i(LAMDA, fc, (low + (2 * i - 1)*h), tm);
        t = (t0 / 2) + (h*g);                //�������ι�ʽ
        n *= 2;
        h /= 2;
    } while (fabs(t - t0) > 0.01);             //�Զ��������e

    return t;
}

//f^2 * G(f)�����ı��ʽ
double RMS_func(double fc, double f, double LAMDA) {

    if (f == 0) return 0;
    else return (1 / LAMDA) * (1 / fc)* pow(sin(pi * f / fc) / (pi * f / fc), 2.0) * pow(f, 2.0);

}


//����bpsk������
void Gene_bpsk(int m, double * BUFF, int n, int size, double * TM, int *Ut) {

    int code_width;
    int i, k;
    double w, phase;
//    int Ut[BUFF_SIZE];

    code_width = size / n;//һ����Ԫռ������ĸ���
    k = 0;
    w = 2 * pi / (code_width / m);//�ز��Ľ�Ƶ��

    srand(time(NULL));
    k = 0;

    //����Ut����
    for (i = 0; i < size; i++) {

        if (k == 0) {
            Ut[i] = rand() % 2 ? -1 : 1;
            k++;
        }

        else if (k == code_width - 1) {
            Ut[i] = Ut[i - 1];
            k = 0;
        }

        else {
            Ut[i] = Ut[i - 1];
            k++;
        }
    }


    for (i = 0; i < size; i++) {

        if (i == 0) {
            phase = pi / 2.0;
            k++;
        }

        else if (k == 0) {
            phase = (Ut[i] == Ut[i - 1]) ? phase : phase + pi;
            k++;
        }

        else if (k == code_width - 1) {
            k = 0;
        }

        else k++;

        BUFF[i] = cos(w * i + phase);
    }

    for (i = 0;  i < size; i++) {

        TM[i] = 1 / (m * 1023000.0) / code_width * i;
    }
}

//��ӡbpsk������
void Print_bpsk(double * BUFF, int size, double * TM) {

    int i;

    for (i = 0; i < size; i++) {

        printf("S(t) = %lf, \t t = %.10lf\n", BUFF[i], TM[i]);
    }
}


//����bpsk�Ĺ���������
void P_bpsk(int m, double * P_BPSK, int size, double *Fx) {

    int i;

    double fc, f, k;

    fc = m * 1023000.0;
    k = 30000000.0 / size;//ÿһ�����鵥Ԫ����СƵ���޶�

    for (i = 0; i < size; i++) {

        f = -15000000.0 + k * i;

        P_BPSK[i] = 10 * log10(G_bpsk(f, fc));

        Fx[i] = f;
    }
}

//��ӡbpsk�Ĺ���������
void Print_P_bpsk(int m, double * P_BPSK, int size, double *Fx) {

    int i;

    for (i = 0; i < size; i++) {

        printf("f = %.2lfHZ,\tGbpsk(f) = %.15lf\n", Fx[i], P_BPSK[i]);
    }

}


//����bpsk������غ���
void Self_bpsk_func(int m, double * SELF_BUFF, int size, double * TM) {

    double Tc, k;
    int i;
    double t;

    Tc = 1.0 / (m * 1023000.0);

    k = 2 * Tc / size;

    for (i = 0; i < size; i++) {

        t = -Tc + k * i;

        SELF_BUFF[i] = R_bpsk(t, Tc);

        TM[i] = t;
    }
}

//��ӡbpsk������غ���
void Print_bpsk_self(double *SELF_BUFF, double * TM, int size) {

    int i;

    for (i = 0; i < size; i++) {

        printf("�� = %.10lfs,\tR��(��) = %.4lf\n", TM[i], SELF_BUFF[i]);
    }
}

//����֮���ʣ�๦�ʦ�
double Lamda_bpsk(double Br, int M) {

    //int i, n;
    //double h, t0, t, g;
    double hign, low, fc;

    hign = Br / 2.0;
    low = -Br / 2.0;
    fc = M * 1023000.0;

    int m, n;//m���Ƶ�������, ��n���Ƹ������λ��ֵķֵ���. n=2^m
    double h, x;
    double s, q;
    double ep; //����Ҫ��
    double y[COUNT];//Ϊ��ʡ�ռ�,ֻ��һά����
    double p;//p����ָʾ������Ԫ�ص�ǰһ��Ԫ��(ͬһ��)

    //������ֵ
    h = Br;
    y[0] = h * (G_bpsk(low, fc) + G_bpsk(hign, fc)) / 2.0;
    m = 1;
    n = 1;
    ep = epsilon + 1.0;

    //��������
    while ((ep >= epsilon) && (m < COUNT))
    {
        p = 0.0;
        for (int i = 0; i < n; i++)//��Hn
        {
            x = low + (i + 0.5)*h;
            p = p + G_bpsk(x, fc);
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
    //printf("�� = %lf", q);
    return (q);
}

//����ʱ���ʣ�๦�ʵ�����غ���
void Left_self_bpsk(double Br, int m, double * BUFF, int size, double * TM) {

    int i;
    double lamda;
    double R[BUFF_SIZE], I[BUFF_SIZE];
    double f, FC, TC;
    double k, j, t;

    lamda = Lamda_bpsk(Br, m);
    FC = m * 1023000.0;
    TC = 1 / FC;
    k = 2.0 * FC / size;
    j = 2.0 * TC / size;


    for (i = 0; i < size; i++) {

        f = -FC + i * k;
        t = -TC + i * j;

        R[i] = inte_Rs_bpsk_r(Br, lamda, FC, f, t);
        I[i] = inte_Rs_bpsk_i(Br, lamda, FC, f, t);

        TM[i] = t;

        BUFF[i] = sqrt(pow(R[i], 2.0) + pow(I[i], 2.0));

    }
}

//��ӡ����֮���ʣ�๦�ʵ�����غ���
void Print_left_self_bpsk(int m, double * BUFF, int size, double * TM) {

    int i;

    for (i = 0; i < size; i++) {

        printf("t = %.15lf,\tRs(t) = %.10lf\n", TM[i], BUFF[i]);
    }
}



//�����źŵľ�����RMS�����rms
double RMS_bpsk(int M, double Br) {

    int m, n;
    double h, x;
    double s, q;
    double ep;
    double y[COUNT];
    double p;
    double hign, low, lamda;
    double fc;

    hign = Br / 2;
    low = -Br / 2;
    lamda = Lamda_bpsk(Br, M);
    fc = M * 1023000.0;

    h = Br;
    y[0] = h * (RMS_func(fc, low, lamda) + RMS_func(fc, low, lamda)) / 2.0;
    m = 1;
    n = 1;
    ep = epsilon + 1.0;

    while ((ep >= epsilon) && (m < COUNT))
    {
        p = 0.0;
        for (int i = 0; i < n; i++)//��Hn
        {
            x = low + (i + 0.5)*h;
            p = p + RMS_func(fc, x, lamda);
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
    q = sqrt(q);
    printf("��rms = %lf\n", q);
    return (q);
}



//��Ч���δ����rect
double RECT_bpsk(int m, double Br) {

    double lamda, G_max;
    double RECT_bpsk;

    lamda = Lamda_bpsk(Br, m);

    G_max = G_bpsk(0.0, m * 1023000.0);

    RECT_bpsk = lamda / G_max;

    printf("��rect = %lf\n", RECT_bpsk);

    return RECT_bpsk;
}

//�����������ܶ�
double Max_power_bpsk(int m, double Br) {

    int i;
    double temp_max;
    double BPSK_BUFF[BUFF_SIZE];
    double TM[BUFF_SIZE];

    P_bpsk(m, BPSK_BUFF, BUFF_SIZE, TM);

    temp_max = BPSK_BUFF[0];

    for (i = 0; i < BUFF_SIZE; i++) {
        temp_max = (BPSK_BUFF[i] > temp_max) ? BPSK_BUFF[i] : temp_max;
    }

    return temp_max;
}

//�������Ĺ�����ʧ
double LOSS_power_bpsk(int m, double Br) {

    double lamda_30M, lamda_24M;
    double BAND;

    BAND = 30000000.0;
    lamda_30M = Lamda_bpsk(BAND, m);//����30M�����µĹ�һ������
    lamda_24M = Lamda_bpsk(Br, m) / lamda_30M;

    return -10 * log10(lamda_24M);
}



double Band_width_0_9_bpsk(int M, double Br) {

    double lamda_30M, lamda;
    double Fx, fc;

    if(M == 1)Fx = 1500000;
    else if(M == 10)Fx = 12000000;
    else Fx = 1000000.0;

    fc = M * 1023000.0;

    lamda_30M = Lamda_bpsk(30000000.0, M);

    lamda = 0;

    while (fabs(lamda - 0.9) > epsilon) {

        Fx = Fx + 50000;

        lamda = Lamda_bpsk(Fx, M) / lamda_30M;
    }
    return Fx;
}

