#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <iostream>
#include <ctime>
#include <chrono>
#include "CResource.h"
#include <vector>
#include "CUtils.h"
using namespace std;
typedef enum TaskType
{
	FIXED, INTERVAL_BASED
};
class CTask
{
private:
	static int id;
	string task_id;

	int priority;
	string name;
	string description;
	time_t startDate;
	time_t endDate;

	int duration;//nr zile
	time_t deadline;
	time_t startNoEarlierThan;

	bool hasIssues;
	bool hasBeenPlanned;

	vector<CResource*> usedResources;
	TaskType taskType;

	int verifyInterval(CTimetable* nodeTimetable, int nodeCapacity, time_t& startTime, time_t& stopTime);
public:
	CTask(int priority, string name, string description, time_t startPoint, time_t endPoint, int duration, TaskType type);
	CTask(int priority, string name, string description, time_t startPoint, time_t endPoint, int duration, TaskType type, string resourceFile);

	string getID() const;
	string getName() const;
	time_t getStartDate() const;
	time_t getEndDate() const;
	time_t getDeadline() const;
	time_t getStartNoEarlierThan() const;

	int getPriority() const;
	int getDuration() const;
	bool getHasBeenPlanned() const;
	bool getIsFixed() const;
	bool getIsIntervalBased() const;
	vector<CResource*> getResources() const;

	void setStartDate(time_t date);
	void setEndDate(time_t date);
	void setHasIssues();
	void setHasBeenPlanned();
	void unsetHasBeenPlanned();

	void addResource(CResource* r);

	int scheduleTask(CTimetable* nodeTimetable, int nodeCapacity);
	int unscheduleTask(CTimetable* nodeTimetable);

	void print();

	~CTask();
};

