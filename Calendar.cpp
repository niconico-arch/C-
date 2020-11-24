#include "Calendar.h"
#include <cstring>
#include <cstdlib>

#define maxLineSize	80

bool
Calendar::roll(date& dt)
{
	// TODO
	while (!(isBusDay(dt)))
	{
		dt++;
	}
	return SUCCESS;
}

bool
Calendar::isBusDay(const date& dt)
{
	// TODO
	if (dt.isWeekDay() == false)
		return false;
	if (this->holidays.count(dt))
		return false;
	return true;
}

bool
Calendar::addBusDays(date& dt, int count)
{
	// TODO
	return SUCCESS;
}

bool
Calendar::addMonths(date& dt, int count)
{
	// TODO
	dt.addMonths(count);
	roll(dt);
	return SUCCESS;
}

bool
Calendar::addYears(date& dt, int count)
{
	// TODO
	dt.addYears(count);
	roll(dt);
	return SUCCESS;
}

bool
Calendar::addHoliday(date& dt)
{
	// TODO
	if (isBusDay(dt))
	{
		this->holidays.insert(dt);
	}
	return SUCCESS;
}

bool
Calendar::removeHoliday(date& dt)
{
	// TODO
	this->holidays.erase(dt);
	return SUCCESS;
}

MMCalendar::MMCalendar(string filename, string mkt) :market(mkt) {
	// TODO
	string s;
	const char* split = ",";
	ifstream holidayFile;
	holidayFile.open(filename);
	while (getline(holidayFile, s))
	{
		char* s_input = (char*)s.c_str();
		char* p = strtok(s_input, split);
		date& inputHoliday = date();
		string inputMarket = ;
		if (inputMarket == mkt)
		{
			this->holidays.insert(inputHoliday);
		}
	}
	
}

bool
MMCalendar::roll(date& dt)
{
	// TODO
	date& targetDt = dt;
	while (!isBusDay(dt))
	{
		++dt;
	}
	if (targetDt.month()!=dt.month())
	{
		dt = targetDt;
		do 
		{
			--dt;
		} while (!isBusDay(dt));
	}
	return SUCCESS;
}