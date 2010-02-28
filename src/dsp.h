#pragma once

// From matlab
void amdemod(int np, float *x, float *y, float *z,
			 int Fc, int Fs,
			 float ini_phase, float carr_amp,
			 int ord, float *num, float *den);

/* 
filter.c 
An ANSI C implementation of MATLAB FILTER.M (built-in)
Written by Chen Yangquan <elecyq@nus.edu.sg>
1998-11-11
http://mechatronics.ece.usu.edu/yqchen/filter.c/
*/
void filter(int ord, float *a, float *b, int np, float *x, float *y);
void filtfilt(int ord, float *a, float *b, int np, float *x, float *y);
