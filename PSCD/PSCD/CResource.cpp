#include "CResource.h"
#include "CTask.h"
int CResource::id = 0;

CResource::CResource(string name)
{
	this->name = name;
	this->resource_id = id++;
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

void CResource::addTask(CTask* t)
{
	alocare.push_back(t);
}

bool CResource::isTheResourceAllocated(time_t startTime, time_t endTime)
{
	for (int i = 0; i < alocare.size(); i++)
	{
		if (!(CUtils::compareDates(startTime, alocare[i]->getEndDate()) == 0 || CUtils::compareDates(alocare[i]->getStartDate(), endTime) == 0))
			return true;
	}

	return false;
}
