#include "CNode.h"
#include "CUtils.h"
int CNode::id = 1;

CNode::CNode(string name, int minPriority, int maxPriority, CNode* parentNode, int capacity, int level)
{
	this->name = name;
	this->minPriority = minPriority;
	this->maxPriority = maxPriority;
	this->parentNode = parentNode;
	this->capacity = capacity;
	this->level = level;

	if (parentNode != NULL)
		parentNode->addChildrenNode(this);

	this->node_id = to_string(id);
	id++;

	timetable = new CTimetable(time(0));
}

void CNode::addChildrenNode(CNode* node)
{
	childNodes.push_back(node);
}

void CNode::addTask(CTask* t)
{
	tasks.push_back(t);
}

void CNode::addNotification(CNotification* n)
{
	notifications.push_back(n);
}

int CNode::getLevel() const
{
	return this->level;
}

int CNode::getCapacity() const
{
	return this->capacity;
}

string CNode::getID() const
{
	return node_id;
}

string CNode::getName() const
{
	return name;
}

vector<CNode*> CNode::getChildren() const
{
	return childNodes;
}

CNode* CNode::getParentNode() const
{
	return parentNode;
}

void CNode::readTasksFromFile(string filename)
{
	ifstream f(filename);
	string taskName, taskDescription, taskStartNoEarlienThan, taskDeadline, type, resourceFile;
	int taskDuration, taskPriority;
	CTask* t;

	while (f >> taskName >> taskPriority >> taskDescription >> type >> taskStartNoEarlienThan >> taskDeadline >> taskDuration >> resourceFile)
	{
		if (type == "I")
			t = new CTask(taskPriority, taskName, taskDescription, CUtils::parseDateTime(taskStartNoEarlienThan.c_str(), "%Y-%m-%d"), CUtils::parseDateTime(taskDeadline.c_str(), "%Y-%m-%d"), taskDuration, INTERVAL_BASED, resourceFile);
		else
			t = new CTask(taskPriority, taskName, taskDescription, CUtils::parseDateTime(taskStartNoEarlienThan.c_str(), "%Y-%m-%d"), CUtils::parseDateTime(taskDeadline.c_str(), "%Y-%m-%d"), taskDuration, FIXED, resourceFile);
		tasks.push_back(t);
	}

	f.close();
}

vector<CTask*> CNode::getTasks() const
{
	return tasks;
}

void CNode::sortTasksByDeadline()
{
	for (int i = 0; i < tasks.size() - 1; i++) {
		for (int j = i + 1; j < tasks.size(); j++) {
			if ((tasks[i]->getDeadline() > tasks[j]->getDeadline())
				|| (tasks[i]->getDeadline() == tasks[j]->getDeadline() && tasks[i]->getPriority() > tasks[j]->getPriority())) {
				CTask* aux = tasks[i];
				tasks[i] = tasks[j];
				tasks[j] = aux;
			}
		}
	}
}

void CNode::scheduleTasks()
{
	time_t startDate = time(0), endDate;
	sortTasksByDeadline();

	for (int i = 0; i < tasks.size(); i++)
		tasks[i]->scheduleTask(timetable, capacity);
}

CNode::~CNode()
{
}
