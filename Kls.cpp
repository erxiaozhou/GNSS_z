#include "Kls.h"
#include "BPSK_Fac.h"
#include "BOC_Fac.h"
#include "math.h"
#include "stdio.h"
#include "integration.h"

int Gs_m, Gl_m;
int Gs_a, Gs_b;
int Gl_a, Gl_b;

double BPSK_BPSK_func(double f) {

	return G_bpsk(f, Gs_m * 1023000.0) * G_bpsk(f, Gl_m * 1023000.0);
}

double BPSK_BOC_func(double f) {

	return G_bpsk(f, Gs_m * 1023000.0) * G_boc(f, Gl_a, Gl_b);
}

double BOC_BPSK_func(double f) {

	return G_bpsk(f, Gl_m * 1023000.0) * G_boc(f, Gs_a, Gs_b);
}

double BOC_BOC_func(double f) {

	return G_boc(f, Gl_a, Gl_b) * G_boc(f, Gs_a, Gs_b);
}

double Kls_func_1(double Bt, double Br) {

	double lamda_Gl;

	lamda_Gl = Lamda_bpsk(Bt, Gl_m);

	return Romberg(Br, BPSK_BPSK_func) / lamda_Gl;
}

double Kls_func_2(double Bt, double Br) {

	double lamda_Gl;
	double fs, fc;

	fs = Gl_a * 1023000.0;
	fc = Gl_b * 1023000.0;

	lamda_Gl = Lambda_boc(Bt, fs, fc);

	return Romberg(Br, BPSK_BOC_func) / lamda_Gl;
}

double Kls_func_3(double Bt, double Br) {

	double lamda_Gl;

	lamda_Gl = Lamda_bpsk(Bt, Gl_m);

    return Romberg(Br, BOC_BPSK_func) / lamda_Gl;
}

double Kls_func_4(double Bt, double Br) {

	double lamda_Gl;
	double fs, fc;

	fs = Gl_a * 1023000.0;
	fc = Gl_b * 1023000.0;

	lamda_Gl = Lambda_boc(Bt, fs, fc);

	return Romberg(Br, BOC_BOC_func) / lamda_Gl;
}

//º∆À„∏Ù¿Îœµ ˝
double Kls_func(Kls K, double Bt, double Br) {

	switch (K.Kls_TYPE) {
	case Kls_PSK_AS_GS_PSK_AS_GL: {
		Gs_m = K.GS_PSK_R_m;
		Gl_m = K.Gl_PSK_R_m;
		return -10 * log10(Kls_func_1(Bt, Br));
		break;
	}
	case Kls_PSK_AS_GS_BOC_AS_GL: {
		Gs_m = K.GS_PSK_R_m;
		Gl_a = K.Gl_BOC_a;
		Gl_b = K.Gl_BOC_b;
		return -10 * log10(Kls_func_2(Bt, Br));
		break;
	}
	case Kls_BOC_AS_GS_PSK_AS_GL: {
		Gs_a = K.Gs_BOC_a;
		Gs_b = K.Gs_BOC_b;
		Gl_m = K.Gl_PSK_R_m;
		return -10 * log10(Kls_func_3(Bt, Br));
	}
	case Kls_BOC_AS_GS_BOC_AS_GL: {
		Gs_a = K.Gs_BOC_a;
		Gs_b = K.Gs_BOC_b;
		Gl_a = K.Gl_BOC_a;
		Gl_b = K.Gl_BOC_b;
		return -10 * log10(Kls_func_4(Bt, Br));
		break;
	}
	}
	return 0;
}
