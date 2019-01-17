#pragma once
#ifndef MULTIPATH
#define MULTIPATH

//画BPSK的多径包络，m是存储数据的数组，BR是积分上下限，size是数组大小，TYPE是画包络的上限还是画包络的下限
void Multipath_bpsk(int m, double * BUFF, double Br, int size, double *TM, double ALPHA, double DELTA_TC);

void Multipath_boc(int a, int b, double * BUFF, double Br, int size, double * TM, double ALPHA, double DELTA_TC);

void Print_Multipath(double * BUFF, double * TM, int size);


#endif