#include "BOC_Fac.h"
#include "BPSK_Fac.h"
#include "integration.h"
#include "math.h"
#include "stdio.h"
#include "NELP.h"
#include <thread>
#include <iostream>

#define BUFF_SIZE 1000
#define pi 3.1415926535
#define C 300000000.0

double Br;//积分上下限
int a, b, size, m;//分别表示α， β， 数组大小， BPSK的扩频码速率
double d_t1, d_t2, d_t3, d_t4;//计算式子中的△的临时变量

double func1_boc(double f) {
	return G_boc(f, a, b) * pow(sin(pi * f * d_t1), 2);
}
double func2_boc(double f) {
	return G_boc(f, a, b) * sin(pi * f * d_t2) * f;
}
double func3_boc(double f) {
	return G_boc(f, a, b) * pow(cos(pi * f * d_t3), 2);
}
double func4_boc(double f) {
	return G_boc(f, a, b) * cos(pi * f * d_t4);
}


double func1_bpsk(double f) {
	return G_bpsk(f, m * 1023000.0) * pow(sin(pi * f * d_t1), 2);
}
double func2_bpsk(double f) {
	return G_bpsk(f, m * 1023000.0) * sin(pi * f * d_t2) * f;
}
double func3_bpsk(double f) {
	return G_bpsk(f, m * 1023000.0) * pow(cos(pi * f * d_t3), 2);
}
double func4_bpsk(double f) {
	return G_bpsk(f, m * 1023000.0) * cos(pi * f * d_t4);
}


void thread1_boc(double * BUFF) {

	int i;
	double k;

	k = D_TM / size;

	for (i = 0; i < size; i++) {
		d_t1 = k * i;
		BUFF[i] = Romberg(Br, func1_boc);
	}
}

void thread2_boc(double * BUFF) {

	int i;
	double k;

	k = D_TM / size;

	for (i = 0; i < size; i++) {
		d_t2 = k * i;
		if (d_t2 == 0) d_t2 = k;
		BUFF[i] = Romberg(Br, func2_boc);
	}
}

void thread3_boc(double * BUFF) {

	int i;
	double k;

	k = D_TM / size;

	for (i = 0; i < size; i++) {
		d_t3 = k * i;
		BUFF[i] = Romberg(Br, func3_boc);
	}
}

void thread4_boc(double * BUFF) {

	int i;
	double k;

	k = D_TM / size;

	for (i = 0; i < size; i++) {
		d_t4 = k * i;
		BUFF[i] = Romberg(Br, func4_boc);
	}
}


void thread1_bpsk(double * BUFF) {

	int i;
	double k;

	k = D_TM / size;

	for (i = 0; i < size; i++) {
		d_t1 = k * i;
		BUFF[i] = Romberg(Br, func1_bpsk);
	}
}

void thread2_bpsk(double * BUFF) {

	int i;
	double k;

	k = D_TM / size;

	for (i = 0; i < size; i++) {
		d_t2 = k * i;
		if (d_t2 == 0) d_t2 = k;
		BUFF[i] = Romberg(Br, func2_bpsk);
	}
}

void thread3_bpsk(double * BUFF) {

	int i;
	double k;

	k = D_TM / size;

	for (i = 0; i < size; i++) {
		d_t3 = k * i;
		BUFF[i] = Romberg(Br, func3_bpsk);
	}
}

void thread4_bpsk(double * BUFF) {

	int i;
	double k;

	k = D_TM / size;

	for (i = 0; i < size; i++) {
		d_t4 = k * i;
		BUFF[i] = Romberg(Br, func4_bpsk);
	}
}

//计算BOC的码跟踪精度
void Sigma_NELP_boc(int A, int B, double * BUFF, double BR, int SIZE, double * TM, double BL, double C_N, double T) {

	//double BL;//码跟踪环单边噪声等效矩形带宽，这里假定它是1HZ
	//double C_N;//信噪比，这里假定为30dB-HZ
	//double T;//电文的数据速率，分20ms和50ms两种
	double F1[BUFF_SIZE], F2[BUFF_SIZE], F3[BUFF_SIZE], F4[BUFF_SIZE];

	int i;

	double temp1, temp2;
	double d_t;

	a = A;
	b = B;
	Br = BR;
	size = SIZE;
	d_t = D_TM / size;

	std::thread t1(thread1_boc, F1);
	std::thread t2(thread2_boc, F2);
	std::thread t3(thread3_boc, F3);
	std::thread t4(thread4_boc, F4);

	t1.join();
	t2.join();
	t3.join();
	t4.join();

	//BL = 1.0;
	//C_N = 1000.0;
	//T = 0.02;

	for (i = 0; i < size; i++) {

		TM[i] = d_t * i;

		if (i == 0) {
			temp1 = (BL * (1 - 0.25 * BL * T) * F1[1]) / (C_N * pow(2 * pi * F2[1], 2));
			temp2 = F3[1] / (T * C_N * pow(F4[1], 2.0)) + 1;
		}
		else {
			temp1 = (BL * (1 - 0.25 * BL * T) * F1[i]) / (C_N * pow(2 * pi * F2[i], 2));
			temp2 = F3[i] / (T * C_N * pow(F4[i], 2.0)) + 1;
		}
		
		BUFF[i] = sqrt(temp1 * temp2) * C;
	}
}

//计算BOC的码跟踪精度
void Sigma_NELP_bpsk(int M, double * BUFF, double BR, int SIZE, double * TM, double BL, double C_N, double T) {

	//double BL;//码跟踪环单边噪声等效矩形带宽，这里假定它是1HZ
	//double C_N;//信噪比，这里假定为30dB-HZ
	//double T;//电文的数据速率，分20ms和50ms两种
	double F1[BUFF_SIZE], F2[BUFF_SIZE], F3[BUFF_SIZE], F4[BUFF_SIZE];

	int i;
	double d_t;

	double temp1, temp2;

	m = M;
	Br = BR;
	size = SIZE;
	d_t = D_TM / size;

	std::thread t1(thread1_bpsk, F1);
	std::thread t2(thread2_bpsk, F2);
	std::thread t3(thread3_bpsk, F3);
	std::thread t4(thread4_bpsk, F4);

	t1.join();
	t2.join();
	t3.join();
	t4.join();

	//BL = 1.0;
	//C_N = 1000.0;
	//T = 0.02;

	for (i = 0; i < size; i++) {

		TM[i] = d_t * i;

		if (i == 0) {
			temp1 = (BL * (1 - 0.25 * BL * T) * F1[1]) / (C_N * pow(2 * pi * F2[1], 2));
			temp2 = F3[1] / (T * C_N * pow(F4[1], 2.0)) + 1;
		}
		else {
			temp1 = (BL * (1 - 0.25 * BL * T) * F1[i]) / (C_N * pow(2 * pi * F2[i], 2));
			temp2 = F3[i] / (T * C_N * pow(F4[i], 2.0)) + 1;
		}
		BUFF[i] = sqrt(temp1 * temp2) * C;
	}
}

//打印码跟踪精度
void Print_Sigma_NELP(double * BUFF, int size, double * TM) {

	int i;
	for (i = 0; i < size; i++) {
		printf("△ = %.12lf,\tσ = %.20lf\n", TM[i], BUFF[i]);
	}
}
