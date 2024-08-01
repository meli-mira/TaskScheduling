#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <iostream>
#include <ctime>
#include <chrono>
#include "CResource.h"
#include <vector>
#include "CUtils.h"
using namespace std;

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

	bool isFixed;
	bool isIntervalBased;
	bool hasIssues;
	bool hasBeenPlanned;

	vector<CResource*> usedResources;
public:
	CTask(int priority, string name, string description, time_t deadline, int duration);
	CTask(int priority, string name, string description, time_t startPoint, time_t deadline, int duration);
	CTask(int priority, string name, string description, time_t startDate, time_t endDate);

	string getName() const;
	time_t getStartDate() const;
	time_t getEndDate() const;
	time_t getDeadline() const;
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

	void scheduleTask(time_t startDate, time_t endDate);

	void print();

	~CTask();
};

