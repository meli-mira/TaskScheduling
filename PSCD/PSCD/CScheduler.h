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
	void setTasksResourcesOcupied(CTask* t, time_t startDate, time_t endDate);

public:
	static CScheduler* getInstance();
	static void destroyInstance();

	void scheduleAll();
	void scheduleTasksForNode(CNode* n);

	void printScheduledTasks(CNode* n);
	void computeOutFileForPloting(CNode* n);

	CNode* readNodeFromFile(string filename);
	void readNodesFromFile(string filaname);
	void readResourcesFromFile(string filename);

	CNode* searchNode(string id);
	CResource* searchResource(string id);
	
	void DFS(CNode* Node);
};

