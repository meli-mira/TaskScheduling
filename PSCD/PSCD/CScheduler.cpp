#include "CScheduler.h"
CScheduler* CScheduler::instance = NULL;
CScheduler::CScheduler()
{
}

CScheduler::~CScheduler()
{
}

bool CScheduler::checkTaskDependencies(CTask* t, time_t startDate, time_t endDate)
{
	vector<CResource*> resources = t->getResources();
	for (int i = 0; i < resources.size(); i++) {
		if (resources[i]->isTheResourceAllocated(startDate, endDate) == true) {
			CLogger::logger(string("Resource ") + resources[i]->getName() + string(" is ocupied between- ") + CUtils::dateToString(startDate, "%Y.%m.%d") + CUtils::dateToString(endDate, "%Y.%m.%d"));
			return false;
		}
	}
	return true;
}

void CScheduler::setTasksResourcesOcupied(CTask* t, time_t startDate, time_t endDate)
{
	vector<CResource*> resources = t->getResources();
	for (int i = 0; i < resources.size(); i++) {
		resources[i]->setTheResourceOcupied(t, startDate, endDate);
	}
}

CScheduler* CScheduler::getInstance()
{
	if (instance == NULL)
		instance = new CScheduler();
	return instance;
}

void CScheduler::destroyInstance()
{
	if (instance != NULL)
		delete instance;
	instance = NULL;
}

void CScheduler::scheduleAll()
{
}

void CScheduler::scheduleTasksForNode(CNode* n)
{
	n->scheduleTasks();
}

void CScheduler::printScheduledTasks(CNode* n)
{
	vector<CTask*> tasks = n->getTasks();

	cout << "PROCESS\t\tSTART-TIME\tEND-TIME\n";
	for (int i = 0; i < tasks.size(); i++)
	{
		if (tasks[i]->getHasBeenPlanned() == true)
			tasks[i]->print();
		else
			cout << "Task " << tasks[i]->getName() << " can't be planned!! Admin will be notified!" << endl;
	}
}

void CScheduler::computeOutFileForPloting(CNode* n)
{
	ofstream g("out.txt");

	vector<CTask*> tasks = n->getTasks();
	int ok = 0;
	for (int i = 0; i < tasks.size(); i++) {
		if (tasks[i]->getHasBeenPlanned() == true) {
			if (ok == 0) {
				g << CUtils::dateToString(tasks[i]->getStartDate(), "%Y-%m-%d") << endl;
				ok = 1;
			}

			int j = 1;
			while (j <= tasks[i]->getDuration()) {
				g << tasks[i]->getName() << endl;
				j++;
			}
		}
	}
	g.close();
}

CNode* CScheduler::readNodeFromFile(string filename)
{
	ifstream f(filename);

	string nodeName, nodeTasksFile, parentNodeID;
	int minPriority, maxPriority, capacity, level;

	CNode* n = NULL;
	while (!f.eof())
	{
		f >> nodeName >> minPriority >> maxPriority >> parentNodeID >> capacity >> level >> nodeTasksFile;

		n = new CNode(nodeName, minPriority, maxPriority, searchNode(parentNodeID), capacity, level);
		n->readTasksFromFile(nodeTasksFile);

		this->nodes.push_back(n);
	}

	f.close();
	return n;
}

void CScheduler::readNodesFromFile(string filename)
{
	ifstream f(filename);
	string nodeFile;

	while (f >> nodeFile)
		readNodeFromFile(nodeFile);
	
	f.close();
}

void CScheduler::readResourcesFromFile(string filename)
{
	ifstream f(filename);
	string resourceName;
	int resourceCapacity;

	CResource* r;

	while (f >> resourceName >> resourceCapacity) {
		r = new CResource(resourceName, resourceCapacity);
		resources.push_back(r);
	}

	f.close();
}

CNode* CScheduler::searchNode(string id)
{
	for (int i = 0; i < nodes.size(); i++)
	{
		if (nodes[i]->getID() == id)
			return nodes[i];
	}
	return NULL;
}

CResource* CScheduler::searchResource(string id)
{
	for (int i = 0; i < resources.size(); i++)
	{
		if (resources[i]->getID() == id)
			return resources[i];
	}
	return  NULL;
}

void CScheduler::DFS(CNode* node)
{
	cout << node->getName() << endl;
	for (int i = 0; i < node->getChildren().size(); i++)
	{
		DFS(node->getChildren()[i]);
	}
}
