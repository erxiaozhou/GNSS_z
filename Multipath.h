#pragma once
#ifndef MULTIPATH
#define MULTIPATH

//��BPSK�Ķྶ���磬m�Ǵ洢���ݵ����飬BR�ǻ��������ޣ�size�������С��TYPE�ǻ���������޻��ǻ����������
void Multipath_bpsk(int m, double * BUFF, double Br, int size, double *TM, double ALPHA, double DELTA_TC);

void Multipath_boc(int a, int b, double * BUFF, double Br, int size, double * TM, double ALPHA, double DELTA_TC);

void Print_Multipath(double * BUFF, double * TM, int size);


#endif