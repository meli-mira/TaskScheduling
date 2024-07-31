#include "CNode.h"
#include "CUtils.h"
int CNode::id = 0;

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

void CNode::readTaskFromFile(string filename)
{
	ifstream f(filename);
	string taskName, taskDescription, taskDeadline;
	int taskDuration, taskPriority;

	while (f >> taskName >> taskDescription >> taskDeadline >> taskDuration >> taskPriority)
	{
		CTask* t = new CTask(taskPriority, taskName, taskDescription, CUtils::parseDateTime(taskDeadline.c_str(), "%Y-%m-%d"), taskDuration);
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

CNode::~CNode()
{
}
