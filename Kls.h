#pragma once

#define Kls_PSK_AS_GS_PSK_AS_GL 1
#define Kls_PSK_AS_GS_BOC_AS_GL 2
#define Kls_BOC_AS_GS_PSK_AS_GL 3
#define Kls_BOC_AS_GS_BOC_AS_GL 4

/*用来确定要计算Kls的类型*/
typedef struct _KLS {

	int Kls_TYPE;					//用来确定Kls的计算类型
	int GS_PSK_R_m;					//当PSK用来作为Gs信号时的m值
	int Gs_BOC_a, Gs_BOC_b;			//当BOC用来作为Gs信号时的α , β的值
	int Gl_PSK_R_m;					//当PSK用来作为Gl信号时的m值
	int Gl_BOC_a, Gl_BOC_b;			//当BOC用来作为Gl信号时的α , β的值

} Kls;


/*************************************************
用来计算Kls, Bt是发射带宽， Br是接受带宽
**************************************************/
double Kls_func(Kls K, double Bt, double Br);
