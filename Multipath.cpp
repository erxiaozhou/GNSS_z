#include "stdio.h"
#include "BOC_Fac.h"
#include "BPSK_Fac.h"
#include "math.h"
#include "Multipath.h"
#include "integration.h"
#include <iostream>
#include <thread>

#define pi 3.1415926535
#define e 0.001 //龙贝格积分算法的误差限
#define COUNT 20
#define BUFF_SIZE 1000
#define Tm 300.0 / 300000000.0 //多径反射信号相对于直达信号的时延
#define C 300000000.0

double alpha, delta_Tc;
double tm_1, tm_2;
int A, B;
int M;

using namespace std;

double func1_boc_multi(double f) {

	return G_boc(f, A, B) * sin(pi * f * delta_Tc) * sin(2 * pi * f * tm_1);


}

double func2_boc_multi(double f) {
	return f * G_boc(f, A, B) * sin(pi * f * delta_Tc) * (1 + alpha * cos(2 * pi * f * tm_2));
}


double func1_bpsk_multi(double f) {

	return G_bpsk(f, M * 1023000.0) * sin(pi * f * delta_Tc) * sin(2 * pi * f * tm_1);
	
}

double func2_bpsk_multi(double f) {

	return f * G_bpsk(f, M * 1023000.0) * sin(pi * f * delta_Tc) * (1 + alpha * cos(2 * pi * f * tm_2));
	
}

void thread_boc1(double * BUFF, double * BR, int * size) {

	int i;
	double k;

	k = Tm / (*size);

	for (i = 0; i < (*size); i++) {

		tm_1 = k * i;

		BUFF[i] = Romberg(*BR, func1_boc_multi);
	}
}

void thread_boc2(double * BUFF, double * BR, int * size) {

	int i;
	double k;

	k = Tm / (*size);

	for (i = 0; i < (*size); i++) {

		tm_2 = k * i;

		BUFF[i] = Romberg(*BR, func2_boc_multi);
	}
}

void thread_bpsk1(double * BUFF, double * BR, int * size) {

	int i;
	double k;

	k = Tm / (*size);

	for (i = 0; i < (*size); i++) {

		tm_1 = k * i;

		BUFF[i] = Romberg(*BR, func1_bpsk_multi);
	}
}

void thread_bpsk2(double * BUFF, double * BR, int * size) {

	int i;
	double k;

	k = Tm / (*size);

	for (i = 0; i < (*size); i++) {

		tm_2 = k * i;

		BUFF[i] = Romberg(*BR, func2_bpsk_multi);
	}
}

void Multipath_bpsk(int m, double * BUFF, double Br, int size, double *TM, double ALPHA, double DELTA_TC) {

	int i;
	double k;

	double temp1, temp2;

	double F1[BUFF_SIZE], F2[BUFF_SIZE];
	//double alpha, delta_Tc;

	//alpha = (TYPE == 1) ? 0.5 : -0.5;
	//delta_Tc = 3e-8;

	alpha = ALPHA;
	delta_Tc = DELTA_TC;
	k = 300.0 / size;
	M = m;
	
	std::thread t1(thread_bpsk1, F1, &Br, &size);
	std::thread t2(thread_bpsk2, F2, &Br, &size);

	t1.join();
	t2.join();

	for (i = 0; i < size; i++) {
		TM[i] = k * i;
		temp1 = alpha * F1[i];
		temp2 = 2 * pi * F2[i];
		BUFF[i] = C * temp1 / temp2 ;
	}
}



void Multipath_boc(int a, int b, double * BUFF, double Br, int size, double * TM, double ALPHA, double DELTA_TC) {

	int i;
	double temp1, temp2;
	double F1[BUFF_SIZE], F2[BUFF_SIZE];
	double k;

	//alpha = (TYPE == 1) ? 0.5 : -0.5; //TYPE是α的类型
	k = 300.0 / size;
	alpha = ALPHA;
	delta_Tc = DELTA_TC;

	A = a;
	B = b;

	std::thread t1(thread_boc1, F1, &Br, &size);
	std::thread t2(thread_boc2, F2, &Br, &size);

	t1.join();
	t2.join();

	for (i = 0; i < size; i++) {
		TM[i] = k * i;
		temp1 = alpha * F1[i];
		temp2 = 2 * pi * F2[i];
		BUFF[i] = C * temp1 / temp2;
	}
}


void Print_Multipath(double * BUFF, double * TM, int size) {

	int i;

	for (i = 0; i < size; i++) {
		printf("τm = %lfm, \t τ = %.20lfm\n", TM[i] , BUFF[i]);
	}
}
