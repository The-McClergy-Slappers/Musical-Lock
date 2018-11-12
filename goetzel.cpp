#include <iostream>
#include "include/armadillo"

using namespace std;
using namespace arma;

rowvec goertzel(rowvec signal, double targetfreq, int blocksize, int samplingrate) {
	int blocks = samplingrate/2 / blocksize;
	double k = floor(0.5 + blocksize * targetfreq / samplingrate);
	double w = k * 2 * 3.14159 / blocksize;
	double cosw = cos(w);
	//double sinw = sin(w);
	double coef = 2 * cosw;
	
	rowvec magnitudes = rowvec(blocks);
	for (int i = 0; i < blocks; i++) {		
		magnitudes[i] = goertzelstep(signal, coef, blocksize,i);
	}
	return magnitudes;	
}

double goertzelstep(rowvec signal, double coef, int blocksize, int i) {
	double stepm0 = 0;
	double stepm1 = 0;
	double stepm2 = 0;
	for (int j = 0; j < blocksize; j++) {
		stepm0 = coef * stepm1 - stepm2 + signal[blocksize*i+j];
		stepm2 = stepm1;
		stepm1 = stepm0;
	}
	return sqrt(pow(stepm1,2) + pow(stepm2,2) - stepm1 * stepm2 * coef);
}

