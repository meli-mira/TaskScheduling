#include "CTask.h"
int CTask::id = 0;

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
	this->isIntervalBased = false;
	this->isFixed = false;
}

CTask::CTask(int priority, string name, string description, time_t startPoint, time_t deadline, int duration)
{
	/* Constructor for a task that has a not before start date */

	this->priority = priority;
	this->name = name;
	this->description = description;
	this->duration = duration;

	this->startNoEarlierThan = startPoint;
	this->isIntervalBased = true;
	this->deadline = deadline;

	this->task_id = to_string(id);
	id++;

	this->hasBeenPlanned = false;
	this->hasIssues = false;
	this->isFixed = true;
}

CTask::CTask(int priority, string name, string description, time_t startDate, time_t endDate)
{
	/* Constructor for a task that has a fixed schedule */

	this->priority = priority;
	this->name = name;
	this->description = description;

	this->startDate = startDate;
	this->endDate = endDate;

	this->task_id = to_string(id);
	id++;

	this->hasBeenPlanned = true;
	this->hasIssues = false;
	this->isIntervalBased = false;
	this->isFixed = true;
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
	return isIntervalBased;
}

bool CTask::getIsFixed() const
{
	return isFixed;
}

vector<CResource*> CTask::getResources() const
{
	return usedResources;
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

void CTask::scheduleTask(time_t startDate, time_t endDate)
{
	this->startDate = startDate;
	this->endDate = endDate;
	this->hasBeenPlanned = true;
}

void CTask::print()
{
	cout << name << "\t" << CUtils::dateToString(startDate, "%Y-%m-%d") << "\t" << CUtils::dateToString(endDate, "%Y-%m-%d") << endl;
}

CTask::~CTask()
{
}
