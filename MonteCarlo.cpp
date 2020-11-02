#include <iostream>
#include "MonteCarlo.h"

using namespace std;

#define SUCCESS 1
#define FAILURE 0

MonteCarlo1Asset* MonteCarlo1Asset::_instance = 0;

MonteCarlo1Asset*
MonteCarlo1Asset::Instance() {
	if (_instance == 0) {
		_instance = new MonteCarlo1Asset;
	}
	return _instance;
}

double MonteCarlo1Asset::simulate()
/*-----------------------------------------------------------------------------
** DESCRIPTION:	Simulate one step.
** RETURNS:		Next value.
**---------------------------------------------------------------------------*/
{
	double normalBM, deltaT;
	//spotT1 = 0.0;
	// random normal variable generated with Box Muller
	normalBM = normalPR();
	//spotT = getSpotVal();
	// calculate deltaT for given step
	deltaT = m_dt / 365;
	//spotT1 = spotT * exp(deltaT * (getIr() - 0.5 * getVol() * getVol()) +normalBM * getVol() * sqrt(deltaT));
	// calculate the next step value with monte carlo
	m_spotVal = m_spotVal * exp(deltaT * (m_ir - 0.5 * m_vol * m_vol) + normalBM * m_vol * sqrt(deltaT));
	return m_spotVal;
}

int __stdcall
MonteCarlo(double spotVal, double vol, double div, double ir, double dt)
{
	MonteCarlo1Asset *mC = MonteCarlo1Asset::Instance();
	mC->setSpotVal(spotVal);
	mC->setVol(vol);
	mC->setDiv(div);
	mC->setIr(ir);
	mC->setDt(dt);

	return SUCCESS;
}

double __stdcall
MCSimulate()
{
	MonteCarlo1Asset *mC = MonteCarlo1Asset::Instance();
	if (mC->getSpotVal() <= 0.0 || mC->getVol() < 0.0 || 
		mC->getDiv() <= -1.0 || mC->getIr() < 0.0 || 
		mC->getDt() <= 0.0)
	{
		return FAILURE;
	}
	return MonteCarlo1Asset::Instance()->simulate();
}