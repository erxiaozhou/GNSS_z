#pragma once

#define Kls_PSK_AS_GS_PSK_AS_GL 1
#define Kls_PSK_AS_GS_BOC_AS_GL 2
#define Kls_BOC_AS_GS_PSK_AS_GL 3
#define Kls_BOC_AS_GS_BOC_AS_GL 4

/*����ȷ��Ҫ����Kls������*/
typedef struct _KLS {

	int Kls_TYPE;					//����ȷ��Kls�ļ�������
	int GS_PSK_R_m;					//��PSK������ΪGs�ź�ʱ��mֵ
	int Gs_BOC_a, Gs_BOC_b;			//��BOC������ΪGs�ź�ʱ�Ħ� , �µ�ֵ
	int Gl_PSK_R_m;					//��PSK������ΪGl�ź�ʱ��mֵ
	int Gl_BOC_a, Gl_BOC_b;			//��BOC������ΪGl�ź�ʱ�Ħ� , �µ�ֵ

} Kls;


/*************************************************
��������Kls, Bt�Ƿ������ Br�ǽ��ܴ���
**************************************************/
double Kls_func(Kls K, double Bt, double Br);
