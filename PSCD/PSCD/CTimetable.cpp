#include "CTimetable.h"

int CTimetable::getIndexForDate(time_t date)
{
	struct tm time1;
	localtime_s(&time1, &date);

	int index = 0;
	if (time1.tm_mon == 1)
		index += ian;
	else if (time1.tm_mon == 2)
		index += ian + feb;
	else if (time1.tm_mon == 3)
		index += ian + feb + mar;
	else if (time1.tm_mon == 4)
		index += ian + feb + mar + apr;
	else if (time1.tm_mon == 5)
		index += ian + feb + mar + apr + may;
	else if (time1.tm_mon == 6)
		index += ian + feb + mar + apr + may + iun;
	else if (time1.tm_mon == 7)
		index += ian + feb + mar + apr + may + iun + iul;
	else if (time1.tm_mon == 8)
		index += ian + feb + mar + apr + may + iun + iul + aug;
	else if (time1.tm_mon == 9)
		index += ian + feb + mar + apr + may + iun + iul + aug + sep;
	else if (time1.tm_mon == 10)
		index += ian + feb + mar + apr + may + iun + iul + aug + sep + oct;
	else
		index += ian + feb + mar + apr + may + iun + iul + aug + sep + oct + nov;

	index += time1.tm_mday;

	return index;
}

CTimetable::CTimetable(time_t startDateOfCalendar)
{
    struct tm time;
    localtime_s(&time, &startDateOfCalendar);

	ian = 31;
	mar = 31;
	apr = 30;
	may = 31;
	iun = 30;
	iul = 31;
	aug = 31;
	sep = 30;
	oct = 31;
	nov = 30;
	dec = 31;

	//an bisect or not
	if (time.tm_year % 400 == 0 || (time.tm_year % 4 == 0 && time.tm_year % 100 != 0))
		feb = 29;
	else
		feb = 28;

	year[1] = -1;//1 ian
	year[2] = -1;//2 ian
	year[6] = -1;//6 ian
	year[7] = -1;//7 ian
	year[24] = -1;
	year[ian + feb + 1] = -1;//1 mai

	//TO DO == PASTE
	year[ian + feb + 3] = -1;//3 mai ==VINEREA PASTE
	year[ian + feb + 5] = -1;//5 mai ==PRIMA ZI DE PASTE ORTODOX
	year[ian + feb + 6] = -1;//6 mai ==A DOUA ZI DE PASTE ORTODOX

	year[ian + feb + mar + apr + may + 1] = -1;//1 IUNIE

	//TO DO == Rusalii
	year[ian + feb + mar + apr + may + 23] = -1;//23 IUNIE
	year[ian + feb + mar + apr + may + 24] = -1;//24 IUNIE


	year[ian + feb + mar + apr + may + iun + iul + 15] = -1;//15 aug
	year[ian + feb + mar + apr + may + iun + iul + aug + sep + oct + 30] = -1;//30 noiembrie

	year[ian + feb + mar + apr + may + iun + iul + aug + sep + oct + nov + 1] = -1;//1 decembrie
	year[ian + feb + mar + apr + may + iun + iul + aug + sep + oct + nov + 25] = -1;//25 decembrie
	year[ian + feb + mar + apr + may + iun + iul + aug + sep + oct + nov + 26] = -1;//26 decembrie

	//WEEKEND-URI
	// 01.01.2024 (monday)
	int contor = 1;
	for (int i = 2024; i < 1900 + time.tm_year; i++)
	{
		if (i % 400 == 0 || (i % 4 == 0 && i % 100 != 0))
			contor += 2;
		else
			contor++;

		contor %= 8;
	}

	for (int i = 1; i <= ian + feb + mar + apr + may + iun + iul + aug + sep + oct + nov + dec; i++)
	{
		if (contor <= 5)//till friday
			year[i] = 0;
		else
			year[i] = -1;//weekend
		contor = (contor + 1) % 8;
	}
}

void CTimetable::setOcupied(time_t startDate, time_t endDate)
{
	int startIndex = getIndexForDate(startDate);
	int n = CUtils::getDaysBetween(startDate, endDate);

	for (int i = startIndex; i <= startIndex + n; i++)
	{
		if (year[i] != -1)
			year[i]++;
	}
}

void CTimetable::print()
{
	for (int i = 1; i <= ian + feb + mar + apr + may + iun + iul + aug + sep + oct + nov + dec; i++)
	{
		cout << year[i] << " ";
	}
}

int CTimetable::getNrOfJobsBetween(time_t startDate, time_t endDate)
{
	int max = 0;
	int startIndex = getIndexForDate(startDate);
	int n = CUtils::getDaysBetween(startDate, endDate);

	for (int i = startIndex; i <= startIndex + n; i++)
		if (max < year[i])
			max = year[i];
	return max;
}

int CTimetable::verifyInterval(time_t& startDate, time_t& endDate)
{
	int startIndex = getIndexForDate(startDate);
	int n = CUtils::getDaysBetween(startDate, endDate);

	for (int i = startIndex; i <= startIndex + n; i++)
	{
		if (year[i] == -1)
		{
			CUtils::addDays(endDate, 1);
			n++;
		}
	}
	return n;
}

CTimetable::~CTimetable()
{
}
