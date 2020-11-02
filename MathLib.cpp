#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <cassert>
#include "MathLib.h"

bool RandGen::seed = false;

RandGen::RandGen() {
	/* initialize uniform random number generator */
	if (!seed) {
		srand ( (unsigned int)time(NULL) );
		seed = true;
	}
}

PolarRej* PolarRej::_instance = 0;
SinCos* SinCos::_instance = 0;
Uniform* Uniform::_instance = 0;
Exponential* Exponential::_instance = 0;

PolarRej*
PolarRej::Instance() {
	if (_instance == 0) {
		_instance = new PolarRej;
	}
	return _instance;
}

SinCos*
SinCos::Instance() {
	if (_instance == 0) {
		_instance = new SinCos;
	}
	return _instance;
}

Uniform*
Uniform::Instance() {
	if (_instance == 0) {
		_instance = new Uniform;
	}
	return _instance;
}

Exponential*
Exponential::Instance() {
	if (_instance == 0) {
		_instance = new Exponential;
	}
	return _instance;
}

double PolarRej::generate()
/*-----------------------------------------------------------------------------
** DESCRIPTION:	Generates a (double) random number, a sequence of which are
**				distributed normally with zero mean and unit variance using
**				Box-Muller transformation (Polar rejection)

** NOTE:		Since each execution of Polar Rejection generates 2 independent normally 
**              distributed random numbers with zero mean, unit variance, for efficiency
**              one random number should be returned, the other one should be saved for
**              the next call.
**--------------------------------------------------------------------------*/
{
	// declare variables for calculation purpose. For variables to be saved, declare as static variable.
	double v1, v2, w, n1;
	static double n2;
	static int use_last = 0;

	// if the second random number is not used yet, set the return number to be the second number generated in the last run and set use_last as False
	if (use_last)
	{
		n1 = n2;
		use_last = 0;
	}
	// generate two random numbers following Polar rejection method for Box-Muller transformation
	else
	{
		do {
			v1 = 2.0 * uniform() - 1.0;
			v2 = 2.0 * uniform() - 1.0;
			w = v1 * v1 + v2 * v2;
		} while (w >= 1.0);

		w = sqrt((-2.0 * log(w)) / w);
		n1 = v1 * w;
		n2 = v2 * w;
		use_last = 1;
	}

	return(n1);
}

double SinCos::generate()
/*-----------------------------------------------------------------------------
** DESCRIPTION:	Generates a (double) random number, a sequence of which are
**				distributed normally with zero mean and unit variance using
**				Box-Muller transformation (sin/cos).

** NOTE:		Since each execution of SinCos generates 2 independent normally
**              distributed random numbers with zero mean, unit variance, for efficiency
**              one random number should be returned, the other one should be saved for
**              the next call.
**---------------------------------------------------------------------------*/
{
	// declare variables for calculation purpose. For variables to be saved, declare as static variable.
	double u1, u2, pi, theta, r, n1;
	static double n2;
	static int use_last = 0;

	// if the second random number is not used yet, set the return number to be the second number generated in the last run and set use_last as False
	if (use_last)
	{
		n1 = n2;
		use_last = 0;
	}
	// generate two random numbers following sin/cos method for Box-Muller transformation
	else
	{
		u1 = uniform();
		u2 = uniform();
		pi = 2 * acos(0.0);
		theta = 1.0 * pi * u1;
		r = sqrt(-2.0 * log(u2));
		n1 = r * cos(theta);
		n2 = r * sin(theta);
		use_last = 1;
	}

	return(n1);
}

double Uniform::generate()
{
	double rv;
	do {
		rv = 1.0*rand()/RAND_MAX;
	} while (rv <= 0.0 || rv >= 1.0);
	return rv;
}

double Exponential::generate()
{
	return -log(1.0*rand()/RAND_MAX);
}

double __stdcall uniform() { return Uniform::Instance()->generate(); }
double __stdcall exponential() { return Exponential::Instance()->generate(); }
double __stdcall normalPR() { return PolarRej::Instance()->generate(); }
double __stdcall normalSC()  { return SinCos::Instance()->generate(); }
