#include "CRR.h"
#include <iostream>
#include <math.h>

using namespace std;

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

	double* deltaT = new double();
	double* discount = new double();
	double* u = new double();
	double* d = new double();
	double* p = new double();
	int* iter = new int(0);  
	double* old_payoff = new double[nStep];

	deltaT = maturity/nStep;
	discount = exp(-rate*deltaT);
	u = exp(vol*pow(deltaT,2));
	d = 1/u;
	p = (exp(rate*deltaT)-d)/(u-d);

	for(iter = nStep; iter > 0; iter--)
	{
		double* new_payoff = new double[iter];
		new_payoff[iter] = discount*(p*old_payoff[iter]+(1-p)*old_payoff[iter+1])
		old_payoff = new_payoff;
    }
	return SUCCESS;
}