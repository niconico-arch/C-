#include "Calendar.h"
#include <cstring>
#include <cstdlib>
#include <sstream>

#define maxLineSize	80

bool
Calendar::roll(date& dt)
/*-------------------------------------------------------------------
** DESCRIPTION:	Roll a date forward until business day.
**
** RETURNS:		SUCCESS.
**-----------------------------------------------------------------*/
{
	while (!(isBusDay(dt)))
	{
		dt++;
	}
	return SUCCESS;
}

bool
Calendar::isBusDay(const date& dt)
/*-------------------------------------------------------------------
** DESCRIPTION:	Check whether a date is a business day.
**
** RETURNS:		TRUE if the date is a business day, else FALSE.
**-----------------------------------------------------------------*/
{
	/* check whether the date falls into weekend */
	if (dt.isWeekDay() == false)
		return false;
	/* check whether the date falls into any holiday according to the holiday set in the calendar */
	if (this->holidays.count(dt))
		return false;
	return true;
}

bool
Calendar::addBusDays(date& dt, int count)
/*-------------------------------------------------------------------
** DESCRIPTION:	add count number of days and roll to a business day.
**
** RETURNS:		SUCCESS after adding and rolling else FAILURE
**-----------------------------------------------------------------*/
{
	dt += count;
	roll(dt);
	return SUCCESS;
}

bool
Calendar::addMonths(date& dt, int count)
/*-------------------------------------------------------------------
** DESCRIPTION:	add count number of months and roll to a business day.
**
** RETURNS:		SUCCESS after adding and rolling else FAILURE
**-----------------------------------------------------------------*/
{
	dt.addMonths(count);
	roll(dt);
	return SUCCESS;
}

bool
Calendar::addYears(date& dt, int count)
/*-------------------------------------------------------------------
** DESCRIPTION:	add count number of years and roll to a business day.
**
** RETURNS:		SUCCESS after adding and rolling else FAILURE
**-----------------------------------------------------------------*/
{
	dt.addYears(count);
	roll(dt);
	return SUCCESS;
}

bool
Calendar::addHoliday(date& dt)
/*-------------------------------------------------------------------
** DESCRIPTION:	add a date into the calendar holiday set.
**
** RETURNS:		SUCCESS if added successfully else FAILURE.
**-----------------------------------------------------------------*/
{
	if (!isBusDay(dt))
	{
		return FAILURE;
	}
	this->holidays.insert(dt);
	return SUCCESS;
}

bool
Calendar::removeHoliday(date& dt)
/*-------------------------------------------------------------------
** DESCRIPTION:	Remove a date from the calendar holiday set.
**
** RETURNS:		SUCCESS if removed successfully else FAILURE.
**-----------------------------------------------------------------*/
{
	if (isBusDay(dt))
	{
		return FAILURE;
	}
	this->holidays.erase(dt);
	return SUCCESS;
}

MMCalendar::MMCalendar(string filename, string mkt) :market(mkt) 
/*-------------------------------------------------------------------
** DESCRIPTION:	Constructor for a Money Market calendar by loading 
**				holidays from a text file.
**-----------------------------------------------------------------*/ 
{
	ifstream holidayFile; // file stream holding the holidays
	string s; // string to store each line read from the holiday file
	holidayFile.open(filename);

	/* read the file line by line and add corresponding market holidays into the calendar*/
	while (getline(holidayFile, s))
	{
		/* for each line in the file delimited by comma, read the first item as date and second item as market */
		string dt, inputMarket;
		std::stringstream ss(s);
		getline(ss, dt, ',');
		getline(ss, inputMarket, ',');
		/* check if market of the line matches the calendar market, add the date into the calendar holiday*/
		if (inputMarket == mkt)
		{
			int year, month, day;
			year = stoi(dt.substr(0, 4));
			month = stoi(dt.substr(4, 2));
			day = stoi(dt.substr(6, 2));
			date inputHoliday(year, month, day);
			this->holidays.insert(inputHoliday);
		}
	}
	
}

bool
MMCalendar::roll(date& dt)
/*--------------------------------------------------------------------------
** DESCRIPTION:	Roll a date to business day in modified following manner.
**              roll the date forward to a business day if it's not month-end,
**				otherwise roll it backward to a business day.
**
** RETURNS:		SUCCESS after rolled the date successfully
**-----------------------------------------------------------------*/
{
	date orgDt = dt; //variable to store the value of the original date
	/* roll the date forward until a business day */
	while (!this->isBusDay(dt))
	{
		++dt;
	}
	/* if the rolled date ended in a different month, i.e. crossed a month-end
	** set the date back to the original date and roll backward till a business day*/
	if (orgDt.month()!=dt.month())
	{
		dt = orgDt;
		do 
		{
			--dt;
		} while (!this->isBusDay(dt));
	}
	return SUCCESS;
}