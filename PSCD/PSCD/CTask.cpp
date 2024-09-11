#include "CTask.h"
int CTask::id = 0;

int CTask::verifyInterval(CTimetable* nodeTimetable, int nodeCapacity, time_t& startDate, time_t& endDate)
{
	int startIndex = nodeTimetable->getIndexForDate(startDate);
	int n = duration;
	int nr = 0, ok = 0, resourceAlocated;

	for (int i = startIndex; nr < n && i < nodeTimetable->getNrOfDays(); i++)
	{
		if (nodeTimetable->at(i) == -1) {
			if (nr == 0) //the first date from interval
				startDate = CUtils::addDays(startDate, 1);
			endDate = CUtils::addDays(endDate, 1);
		}
		else if (nodeTimetable->at(i) < nodeCapacity) {
			//resources are free?
			resourceAlocated = 0;
			for (int j = 0; j < usedResources.size(); j++)
			{
				if (usedResources[j]->isTheResourceAllocated(i) == true)
				{
					resourceAlocated = 1;
					break;
				}
			}
			if (resourceAlocated == 0) {
				nr++; //all good
			}
			else {
				//one of the resource is alocated in the day with index i -->try to bypass
				startDate = nodeTimetable->getDateFromIndex(i + 1);
				endDate = CUtils::addDays(startDate, duration -	1);
				nr = 0;
			}
		}
		else {
			/* capacitatea maxima depasita -> resetarea intervalului */
			startDate = nodeTimetable->getDateFromIndex(i + 1);
			endDate = CUtils::addDays(startDate, duration - 1);
			//cout << CUtils::dateToString(startDate, "%Y.%m.%d") << " " << CUtils::dateToString(endDate, "%Y.%m.%d") << endl;
			nr = 0;
		}

		if (CUtils::compareDates(endDate, deadline) == false)
			return -1;
	}
	return nr;
}

CTask::CTask(int priority, string name, string description, time_t deadline, int duration)
{
	this->priority = priority;
	this->name = name;
	this->description = description;
	this->duration = duration;

	this->deadline = deadline;

	this->task_id = to_string(id);
	id++;
	this->hasBeenPlanned = false;
	this->hasIssues = false;
}

CTask::CTask(int priority, string name, string description, time_t startPoint, time_t endPoint, int duration, TaskType type)
{
	this->priority = priority;
	this->name = name;
	this->description = description;
	this->duration = duration;

	this->hasBeenPlanned = false;
	this->hasIssues = false;
	this->taskType = type;

	this->task_id = to_string(id);
	id++;

	this->startNoEarlierThan = startPoint;
	this->deadline = endPoint;
}

string CTask::getName() const
{
	return name;
}

time_t CTask::getStartDate() const
{
	return this->startDate;
}

time_t CTask::getEndDate() const
{
	return this->endDate;
}

time_t CTask::getDeadline() const
{
	return deadline;
}

time_t CTask::getStartNoEarlierThan() const
{
	return startNoEarlierThan;
}

int CTask::getPriority() const
{
	return priority;
}

int CTask::getDuration() const
{
	return duration;
}

bool CTask::getHasBeenPlanned() const
{
	return hasBeenPlanned;
}

bool CTask::getIsIntervalBased() const
{
	if (taskType == INTERVAL_BASED)
		return true;
	else
		return false;
}

vector<CResource*> CTask::getResources() const
{
	return this->usedResources;
}

bool CTask::getIsFixed() const
{
	if (taskType == FIXED)
		return true;
	else
		return false;
}

void CTask::setStartDate(time_t date)
{
	this->startDate = date;
}

void CTask::setEndDate(time_t date)
{
	this->endDate = date;
}

void CTask::setHasIssues()
{
	this->hasIssues = true;
}

void CTask::setHasBeenPlanned()
{
	this->hasBeenPlanned = true;
}

void CTask::unsetHasBeenPlanned()
{
	this->hasBeenPlanned = false;
}

void CTask::addResource(CResource* r)
{
	this->usedResources.push_back(r);
}

int CTask::scheduleTask(CTimetable* nodeTimetable, int nodeCapacity)
{
	time_t startDate, endDate;
	startDate = this->startNoEarlierThan;
	endDate = CUtils::addDays(startDate, this->duration - 1);

	int n = verifyInterval(nodeTimetable, nodeCapacity, startDate, endDate);
	
	if (n != -1) {
		this->startDate = startDate;
		this->endDate = endDate;
		this->hasBeenPlanned = true;
		nodeTimetable->setOcupied(startDate, endDate);

		//success
		//set the resources of the planned task ocupied
	}
	else {
		//todo notification  + mai prioritar?
		cout << "Not possible to schedule the task. there are no " << endl;
		this->hasIssues = true;
		this->hasBeenPlanned = false;
		return -1;
	}

	return 0;
}

void CTask::print()
{
	cout << name << "\t" << CUtils::dateToString(startDate, "%Y-%m-%d") << "\t" << CUtils::dateToString(endDate, "%Y-%m-%d") << endl;
}

CTask::~CTask()
{
}
