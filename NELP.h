#pragma once
#ifndef NELP
#define NELP

#define D_TM 0.0000001


/******************************************************
计算BPSK的码跟踪精度(σNELP)^2
*******************************************************/
void Sigma_NELP_bpsk(int M, double * BUFF, double BR, int SIZE, double * TM, double BL, double C_N, double T);


/******************************************************
计算BOC的码跟踪精度(σNELP)^2
*******************************************************/
void Sigma_NELP_boc(int A, int B, double * BUFF, double BR, int SIZE, double * TM, double BL, double C_N, double T);

/******************************************************
打印码跟踪精度误差
*******************************************************/
void Print_Sigma_NELP(double * BUFF, int size, double * TM);


#endif // !NELP
#pragma once
