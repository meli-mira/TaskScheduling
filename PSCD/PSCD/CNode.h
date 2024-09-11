#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include "CTask.h"
#include "CNotification.h"
#include "CTimetable.h"

using namespace std;
class CNode
{
private:
	static int id;
	string node_id;

	string name;
	int capacity;
	int minPriority;
	int maxPriority;

	CNode* parentNode;
	vector<CNode*> childNodes;
	vector<CTask*> tasks;
	vector<CNotification*> notifications;

	int level;

	CTimetable* timetable;
public:
	CNode(string name, int minPriority, int maxPriority, CNode* parentNode, int capacity, int level);

	void addChildrenNode(CNode* node);

	void addTask(CTask* t);
	void addNotification(CNotification* n);

	int getLevel() const;
	int getCapacity() const;
	string getID() const;
	string getName() const;
	vector<CNode*> getChildren() const;
	CNode* getParentNode() const;

	void readTaskFromFile(string filename);

	vector<CTask*> getTasks() const;

	void sortTasksByDeadline();

	void scheduleTasks();

	~CNode();
};

