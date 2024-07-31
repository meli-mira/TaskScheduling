
#pragma once
#include "CUtils.h"
class CResource
{
private:
	static int id;
	string resource_id;

	string name;

	vector<class CTask*> alocare;
public:
	CResource(string name);
	~CResource();

	string getID() const;
	string getName() const;

	void addTask(class CTask* t);

	bool isTheResourceAllocated(time_t startTime, time_t endTime);

};

