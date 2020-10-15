#include "CRR.h"
#include <iostream>
#include <math.h>

using namespace std;

double CPPayoffCalc(double stockPrice, double strike, CALL_PUT call_put)
{
	if (call_put == Call)
	{
		return fmax(stockPrice - strike, 0);
	}
	else if (call_put == Put)
	{
		return fmax(strike - stockPrice, 0);
	}
	else
	{
		return FAILURE;
	}
}

int CRRBinomialTree(
	CALL_PUT	callOrPut,				/* (I) put or call flag (use Call or Put) */
	AMER_EURO	amerOrEuro,				/* (I) option type (use European, American) */
	double		spotPrice,				/* (I) spot price of underlying */
	double		strike,					/* (I) strike price of option */
	double		maturity,				/* (I) maturity of option in years */
	double		vol,					/* (I) annual volatility of underlying */
	double		rate,					/* (I) annual continuous compounded discount rate */
	int			nStep,					/* (I) number of steps in tree */
	double		*value)					/* (O) option value */
/*-----------------------------------------------------------------------------
** FUNCTION:	CRRBinomialTree.
**
** DESCRIPTION:	Calculates the price of an option using the Cox, Ross and Rubinstein
**				binomial tree model.
**
** RETURNS:		SUCCESS and the premium, otherwise FAILURE.                                  
**
**---------------------------------------------------------------------------*/
{

	if (nStep <= 0)
	{
		cout << "CRRBinomialTree: Negative number of steps." << endl;
		return FAILURE;
	}

	if (spotPrice <= 0.0 || strike <= 0.0 || maturity < 0.0 || vol < 0.0 || rate < 0.0)
	{
		cout << "CRRBinomialTree: Invalid input detected." << endl;
		return FAILURE;
	}

	// TO-BE-COMPLETED

	*value = 0.0;

	double *deltaT = new double();
	double *discount = new double();
	double *u = new double();
	double *d = new double();
	double *p = new double();
	double *payoff = new double[nStep];
	double *stockPrice = new double[nStep*2];

	*deltaT = maturity / nStep;
	*discount = exp(-rate * *deltaT);
	*u = exp(vol * sqrt(*deltaT));
	*d = 1 / *u;
	*p = (exp(rate * *deltaT) - *d) / (*u - *d);

	for (int iter = 0; iter < nStep; iter++)
	{
		stockPrice[iter*2] = spotPrice * pow(*u, nStep - iter) * pow(*d, iter);
		stockPrice[iter * 2 + 1] = spotPrice * pow(*u, nStep - 1 - iter) * pow(*d, iter);
		payoff[iter] = CPPayoffCalc(stockPrice[iter*2], strike, callOrPut);
	}

	for (int step = nStep; step > 0; step--)
	{
		for (int iter = 0; iter < step; iter++)
		{
			payoff[iter] = *discount * (*p * payoff[iter] + (1 - *p) * payoff[iter + 1]);
			if (amerOrEuro == American)
			{
				if (callOrPut == Call)
				{
					payoff[iter] = fmax(payoff[iter], stockPrice[nStep - step + 1 + iter * 2] - spotPrice);
				}
				else if (callOrPut == Put)
				{
					payoff[iter] = fmax(payoff[iter], spotPrice - stockPrice[nStep - step + 1 + iter * 2]);
				}
			}
		}
	}
	*value = payoff[0];
	delete deltaT;
	delete discount;
	delete u;
	delete d;
	delete p;
	delete[] payoff;
	delete[] stockPrice;
	return SUCCESS;
}

int main()
{
	AMER_EURO A_E;
	CALL_PUT C_P;
	double spotPrice, strikePrice, maturity, vol, rate, crr;
	int nStep, err;
	A_E = European; C_P = Call;	spotPrice = 100.0; strikePrice = 100.0; maturity = 1; vol = 0.2; rate = 0.05; nStep = 10;
	CRRBinomialTree(C_P, A_E, spotPrice, strikePrice, maturity, vol, rate, nStep, &crr);
	return 0;
}