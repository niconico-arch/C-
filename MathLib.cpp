#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <cassert>
#include "MathLib.h"

bool RandGen::seed = false;

RandGen::RandGen() {
	/* initialize uniform random number generator */
	if (!seed) {
		srand((unsigned int)time(NULL));
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
	double v1, v2, w, rv1;

	/* if it's the first time running the function (no random variable 2 was saved)
	** generate two random variables with sin/cos method for Box-Muller transformation
	** set first as false so that the random number 2 generated can be directly used next time */
	if (first)
	{
		do {
			v1 = 2.0 * uniform() - 1.0;
			v2 = 2.0 * uniform() - 1.0;
			w = v1 * v1 + v2 * v2;
		} while (w >= 1.0);

		w = sqrt((-2.0 * log(w)) / w);
		rv1 = v1 * w;
		rv2 = v2 * w;
		first = false; 
	}
	/* if it's not the first time running the function (random variable 2 was saved)
	** use randome number 2 generate in the previous run directly
	** set first as true to re-generate two new random number next time*/
	else
	{
		rv1 = rv2;
		first = true;
	}

	return(rv1);
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
	// declare variables for calculation purpose
	double u1, u2, pi, theta, r, rv1;

	/* if it's the first time running the function (no random variable 2 was saved)
	** generate two random variables with sin/cos method for Box-Muller transformation
	** set first as false so that the random number 2 generated can be directly used next time */
	if (first)
	{
		u1 = uniform();
		u2 = uniform();
		pi = 2 * acos(0.0);
		theta = 2.0 * pi * u1;
		r = sqrt(-2.0 * log(u2));
		rv1 = r * cos(theta);
		rv2 = r * sin(theta);
		first = false;
	}
	/* if it's not the first time running the function (random variable 2 was saved)
	** use randome number 2 generate in the previous run directly
	** set first as true to re-generate two new random number next time*/
	else
	{
		rv1 = rv2;
		first = true;
	}

	return(rv1);
}


double Uniform::generate()
{
	double rv;
	do {
		rv = 1.0 * rand() / RAND_MAX;
	} while (rv <= 0.0 || rv >= 1.0);
	return rv;
}

double Exponential::generate()
{
	return -log(1.0 * rand() / RAND_MAX);
}

double __stdcall uniform() { return Uniform::Instance()->generate(); }
double __stdcall exponential() { return Exponential::Instance()->generate(); }
double __stdcall normalPR() { return PolarRej::Instance()->generate(); }
double __stdcall normalSC() { return SinCos::Instance()->generate(); }
