#include<stdio.h>
#include "nrutil.h"
#include<time.h>
#include<math.h>

#define IA 16807
#define IM 2147483647
#define AM (1.0/IM)
#define IQ 127773
#define IR 2836
#define NTAB 32
#define NDIV (1+(IM-1)/NTAB)
#define EPS 1.2e-7
#define RNMX (1.0-EPS)

float ran1(long* idum)
{
	int j;
	long k;
	static long iy = 0;
	static long iv[NTAB];
	float temp;

	if (*idum <= 0 || !iy) {
		if (-(*idum) < 1) *idum = 1;
		else *idum = -(*idum);
		for (j = NTAB + 7; j >= 0; j--) {
			k = (*idum) / IQ;
			*idum = IA * (*idum - k * IQ) - IR * k;
			if (*idum < 0) *idum += IM;
			if (j < NTAB) iv[j] = *idum;
		}
		iy = iv[0];
	}
	k = (*idum) / IQ;
	*idum = IA * (*idum - k * IQ) - IR * k;
	if (*idum < 0) *idum += IM;
	j = iy / NDIV;
	iy = iv[j];
	iv[j] = *idum;
	if ((temp = AM * iy) > RNMX) return RNMX;
	else return temp;
}
#undef IA
#undef AM
#undef IQ
#undef IR
#undef NTAB
#undef NDIV
#undef EPS
#undef RNMX

float gasdev(long* idum)
{
	float ran1(long* idum);
	static int iset = 0;
	static float gset;
	float fac, rsq, v1, v2;

	if (*idum < 0) iset = 0;
	if (iset == 0) {
		do {
			v1 = 2.0 * ran1(idum) - 1.0;
			v2 = 2.0 * ran1(idum) - 1.0;
			rsq = v1 * v1 + v2 * v2;
		} while (rsq >= 1.0 || rsq == 0.0);
		fac = sqrt(-2.0 * log(rsq) / rsq);
		gset = v1 * fac;
		iset = 1;
		return v2 * fac;
	}
	else {
		iset = 0;
		return gset;
	}
}

#define N 20
#define NOVER2 (N/2)
#define NPTS 1000 //여기를 수정
#define ISCAL 400
#define LLEN 50

void main() {
	long idum = time(NULL);
	//uniform
	printf("------------------------- uniform (ran1) ----------------\n");
	printf("samples : %d\n", NPTS);
	for (int i = 1; i <= NPTS; i++) {
		printf("%.8f  ", 5 * ran1(&idum) - 3);
	}

	printf("\n---------------------------gaussian(gasdev)-----------------------------\n");
	printf("samples : %d\n", NPTS);
	for (int i = 1; i <= NPTS; i++) {
		printf("%.8f  ", 1.5 * gasdev(&idum) + 0.5);
	}

	printf("\n---------------------------gasdev graph-----------------------------\n");
	char words[LLEN + 1]; //51
	int i, j, k, klim, dist[N + 1];
	idum = time(NULL);
	float dd, x;

	for (j = 0; j <= N; j++) dist[j] = 0;
	for (i = 1; i <= NPTS; i++) {
		x = 1.5 * gasdev(&idum) + 0.5; //m and s
		//printf("%d : %.8f\n", i, x);
		j = (int)(x > 0 ? x + 0.5 : x - 0.5);
		if ((j >= -NOVER2) && (j <= NOVER2)) ++dist[j + NOVER2];
	}
	printf("Normally distributed deviate of %6d points\n", NPTS);
	printf("%5s %10s %9s\n", "x", "p(x)", "graph:");
	for (j = 0; j <= N; j++) {
		dd = (float)dist[j] / NPTS;
		for (k = 1; k <= LLEN; k++) words[k] = ' ';
		klim = (int)(ISCAL * dd);
		if (klim > LLEN)  klim = LLEN;
		for (k = 1; k <= klim; k++) words[k] = '*';
		printf("%8.4f %8.4f  ", j / (0.25 * N), dd);
		for (k = 1; k <= LLEN; k++) printf("%c", words[k]);
		printf("\n");
	}

	/*for (float i = -3; i <= 2; i += 0.05) {
		printf("%.2f\n", i);
	}*/
}
