#include "CResource.h"
#include "CTask.h"
int CResource::id = 0;

CResource::CResource(string name)
{
	this->name = name;
	this->resource_id = id++;

	timetable = new CTimetable(time(0));
}

CResource::~CResource()
{
}

string CResource::getID() const
{
	return resource_id;
}

string CResource::getName() const
{
	return name;
}

bool CResource::isTheResourceAllocated(time_t startTime, time_t endTime)
{
	if (timetable->getNrOfJobsBetween(startTime, endTime) >= 1)
		return false;
	else
		return true;
}

bool CResource::isTheResourceAllocated(int index)
{
	if (timetable->at(index) != 0)
		return true;
	else
		return false;
}

void CResource::setTheResourceOcupied(CTask* t, time_t startTime, time_t endTime)
{
	timetable->setOcupied(startTime, endTime);
	alocare.push_back(make_pair(t, make_pair(startTime, endTime)));
}
