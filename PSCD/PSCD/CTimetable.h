#pragma once
#include "CUtils.h"

class CTimetable
{
private:
	int year[369];
	time_t startDateOfCalendar;
	int ian, feb, mar, apr, may, iun, iul, aug, sep, oct, nov, dec;

	int getIndexForDate(time_t date);
public:
	CTimetable(time_t startDateOfCalendar);

	void setOcupied(time_t startDate, time_t endDate);
	void print();
	int getNrOfJobsBetween(time_t startDate, time_t endDate);

	int verifyInterval(time_t& startDate, time_t& endDate);

	~CTimetable();
};

