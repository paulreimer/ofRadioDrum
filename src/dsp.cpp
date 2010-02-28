#include "dsp.h"

#include <math.h>
#ifndef TWO_PI
#define TWO_PI   6.28318530717958647693
#endif

/* 
 filter.c 
 An ANSI C implementation of MATLAB FILTER.M (built-in)
 Written by Chen Yangquan <elecyq@nus.edu.sg>
 1998-11-11
*/

void filter(int ord, float *a, float *b, int np, float *x, float *y)
{
	int i,j;
	y[0] = b[0]*x[0];
	for (i=1; i<ord+1; i++)
	{
        y[i] = 0.;
        for (j=0; j<i+1; j++)
        	y[i] = y[i] + b[j]*x[i-j];
        for (j=0; j<i; j++)
        	y[i] = y[i] - a[j+1]*y[i-j-1];
	}
	/* end of initial part */
	for (i=ord+1; i<np+1; i++)
	{
		y[i] = 0.;
		for (j=0; j<ord+1; j++)
			y[i] = y[i] + b[j]*x[i-j];
		for (j=0; j<ord; j++)
			y[i] = y[i] - a[j+1]*y[i-j-1];
	}
}

void filtfilt(int ord, float *a, float *b, int np, float *x, float *y)
{
	int i;
	
	filter(ord,a,b,np,x,y);
	/* NOW y=filter(b,a,x);*/
	
	/* reverse the series for filtfilt */
	for (i=0; i<np; i++)
		x[i] = y[np-i-1];
	
	/* do filter again */
	filter(ord,a,b,np,x,y);
	
	/* reverse the series back */
	for (i=0; i<np; i++)
		x[i] = y[np-i-1];
	for (i=0; i<np; i++)
		y[i] = x[i];
}

// From matlab
void amdemod(int np, float *x, float *y, float *z,
			 int Fc, int Fs,
			 float ini_phase, float carr_amp,
			 int ord, float *num, float *den)
{
	// [num,den] = butter(5,Fc*2/Fs);

	int i;
	
	for (i=0; i<np; i++)
	{
		z[i] = x[i] * cosf(TWO_PI * Fc * (i / Fs) + ini_phase);
		filtfilt(ord, num, den, np, z, y);
		y[i] = y[i] * 2 - carr_amp;
	}
}
