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


//bpsk的功率谱密度的表达式
double G_bpsk(double f, double Fc) {

    if (f == 0) return 1.0 / Fc;
    else return (1 / Fc)* pow(sin(pi * f / Fc) / (pi * f / Fc), 2.0);
}
//bpsk的自相关函数表达式
double R_bpsk(double t, double tc) {

    return  1.0 - fabs(t) / tc;
}



//bpsk自相关函数剩余功率的实部表达式
double Rs_bpsk_r(double LAMDA, double fc, double f, double tm) {

    if (f == 0) return 1.0 / LAMDA * 1 / fc;
    else return (1 / LAMDA) * (1 / fc)* pow(sin(pi * f / fc) / (pi * f / fc), 2.0) * cos(2 * pi * f * tm);
}



//bpsk自相关函数剩余功率的虚部表示
double Rs_bpsk_i(double LAMDA, double fc, double f, double tm) {

    if (f == 0) return 0;
    else return (1 / LAMDA) * (1 / fc)* pow(sin(pi * f / fc) / (pi * f / fc), 2.0) * sin(2 * pi * f * tm);
}



//计算bpsk的剩余功率的自相关函数的实部积分
double inte_Rs_bpsk_r(double BR, double LAMDA, double fc, double f, double tm) {

    double hign, low;

    int i, n;
    double h, t0, t, g;
    n = 1;                             //赋初值

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
        t = (t0 / 2) + (h*g);                //复化梯形公式
        n *= 2;
        h /= 2;
    } while (fabs(t - t0) > 0.01);             //自定义误差限e

    return t;
}

//计算bpsk的剩余功率的自相关函数的虚部积分
double inte_Rs_bpsk_i(double BR, double LAMDA, double fc, double f, double tm) {

    double hign, low;
    int i, n;
    double h, t0, t, g;
    n = 1;                             //赋初值

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
        t = (t0 / 2) + (h*g);                //复化梯形公式
        n *= 2;
        h /= 2;
    } while (fabs(t - t0) > 0.01);             //自定义误差限e

    return t;
}

//f^2 * G(f)函数的表达式
double RMS_func(double fc, double f, double LAMDA) {

    if (f == 0) return 0;
    else return (1 / LAMDA) * (1 / fc)* pow(sin(pi * f / fc) / (pi * f / fc), 2.0) * pow(f, 2.0);

}


//生成bpsk的序列
void Gene_bpsk(int m, double * BUFF, int n, int size, double * TM, int *Ut) {

    int code_width;
    int i, k;
    double w, phase;
//    int Ut[BUFF_SIZE];

    code_width = size / n;//一个码元占得数组的个数
    k = 0;
    w = 2 * pi / (code_width / m);//载波的角频率

    srand(time(NULL));
    k = 0;

    //生成Ut序列
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

//打印bpsk的序列
void Print_bpsk(double * BUFF, int size, double * TM) {

    int i;

    for (i = 0; i < size; i++) {

        printf("S(t) = %lf, \t t = %.10lf\n", BUFF[i], TM[i]);
    }
}


//生成bpsk的功率谱序列
void P_bpsk(int m, double * P_BPSK, int size, double *Fx) {

    int i;

    double fc, f, k;

    fc = m * 1023000.0;
    k = 30000000.0 / size;//每一个数组单元的最小频率限度

    for (i = 0; i < size; i++) {

        f = -15000000.0 + k * i;

        P_BPSK[i] = 10 * log10(G_bpsk(f, fc));

        Fx[i] = f;
    }
}

//打印bpsk的功率谱序列
void Print_P_bpsk(int m, double * P_BPSK, int size, double *Fx) {

    int i;

    for (i = 0; i < size; i++) {

        printf("f = %.2lfHZ,\tGbpsk(f) = %.15lf\n", Fx[i], P_BPSK[i]);
    }

}


//生成bpsk的自相关函数
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

//打印bpsk的自相关函数
void Print_bpsk_self(double *SELF_BUFF, double * TM, int size) {

    int i;

    for (i = 0; i < size; i++) {

        printf("τ = %.10lfs,\tRτ(τ) = %.4lf\n", TM[i], SELF_BUFF[i]);
    }
}

//带限之后的剩余功率λ
double Lamda_bpsk(double Br, int M) {

    //int i, n;
    //double h, t0, t, g;
    double hign, low, fc;

    hign = Br / 2.0;
    low = -Br / 2.0;
    fc = M * 1023000.0;

    int m, n;//m控制迭代次数, 而n控制复化梯形积分的分点数. n=2^m
    double h, x;
    double s, q;
    double ep; //精度要求
    double y[COUNT];//为节省空间,只需一维数组
    double p;//p总是指示待计算元素的前一个元素(同一行)

    //迭代初值
    h = Br;
    y[0] = h * (G_bpsk(low, fc) + G_bpsk(hign, fc)) / 2.0;
    m = 1;
    n = 1;
    ep = epsilon + 1.0;

    //迭代计算
    while ((ep >= epsilon) && (m < COUNT))
    {
        p = 0.0;
        for (int i = 0; i < n; i++)//求Hn
        {
            x = low + (i + 0.5)*h;
            p = p + G_bpsk(x, fc);
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
    //printf("λ = %lf", q);
    return (q);
}

//带限时候的剩余功率的自相关函数
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

//打印带限之后的剩余功率的自相关函数
void Print_left_self_bpsk(int m, double * BUFF, int size, double * TM) {

    int i;

    for (i = 0; i < size; i++) {

        printf("t = %.15lf,\tRs(t) = %.10lf\n", TM[i], BUFF[i]);
    }
}



//带限信号的均方根RMS带宽βrms
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
        for (int i = 0; i < n; i++)//求Hn
        {
            x = low + (i + 0.5)*h;
            p = p + RMS_func(fc, x, lamda);
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
    q = sqrt(q);
    printf("βrms = %lf\n", q);
    return (q);
}



//有效矩形带宽βrect
double RECT_bpsk(int m, double Br) {

    double lamda, G_max;
    double RECT_bpsk;

    lamda = Lamda_bpsk(Br, m);

    G_max = G_bpsk(0.0, m * 1023000.0);

    RECT_bpsk = lamda / G_max;

    printf("βrect = %lf\n", RECT_bpsk);

    return RECT_bpsk;
}

//主瓣的最大功率密度
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

//计算带外的功率损失
double LOSS_power_bpsk(int m, double Br) {

    double lamda_30M, lamda_24M;
    double BAND;

    BAND = 30000000.0;
    lamda_30M = Lamda_bpsk(BAND, m);//计算30M带宽下的归一化功率
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

