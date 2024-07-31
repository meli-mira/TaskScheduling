#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include "CTask.h"
#include "CResource.h"
#include "CUtils.h"
#include "CNode.h"
#include <unordered_map>
#include <map>
#include "CLogger.h"
#include "CNotification.h"
using namespace std;

class CScheduler
{
private:
	vector<CNode*> nodes;
	vector<CResource*> resources;

	static CScheduler* instance;
	CScheduler();
	~CScheduler();

	bool checkTaskDependencies(CTask* t, time_t startDate, time_t endDate);
	void setTasksResourcesOcupied(CTask* t);

	int scheduleTaskCase1(int& startTime, CTask* t);
	int scheduleTaskCase2(int& startTime, vector<CTask*>tasks, int i);
public:
	static CScheduler* getInstance();
	static void destroyInstance();

	void addPlannedTask(CTask* t, CNode* n);

	bool isTheResourceUsed(CResource* r, time_t startTime, time_t endTime);
	bool isTheNodeOcupied(CNode* n, time_t startTime, time_t endTime);

	void scheduleTasksForNode(CNode* n);
	void scheduleAll();

	void printScheduledTasks(CNode* n);
	void computeOutFileForPloting(CNode* n);

	void readNodesFromFile(string filaname);
	void readResourcesFromFile(string filename);

	void DFS(CNode* Node);
};

