#pragma once
#include "CUtils.h"
#define MAX_CAPACITY 3
class CTimetable
{
private:
	int year[369];
	int nrDays;
	time_t startDateOfCalendar;
	int ian, feb, mar, apr, may, iun, iul, aug, sep, oct, nov, dec;

public:
	CTimetable(time_t startDateOfCalendar);

	void setOcupied(time_t startDate, time_t endDate);
	void print();

	int getNrOfJobsBetween(time_t startDate, time_t endDate);
	int getIndexForDate(time_t date);
	time_t getDateFromIndex(int index);
	int getNrOfDays();

	int at(int i);
	
	~CTimetable();
};

