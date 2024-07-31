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

void CScheduler::setTasksResourcesOcupied(CTask* t)
{
	vector<CResource*> resources = t->getResources();
	for (int i = 0; i < resources.size(); i++) {
		resources[i]->addTask(t);
	}
}

int CScheduler::scheduleTaskCase1(int& startTime, CTask* t)
{
	bool ok;
	if (CUtils::compareDates(CUtils::addDays(startTime, t->getDuration()), t->getDeadline()) == true) {

		ok = checkTaskDependencies(t, startTime, CUtils::addDays(startTime, t->getDuration()));

		if (ok == 1) {

			t->scheduleTask(startTime, CUtils::addDays(startTime, t->getDuration()));
			startTime = CUtils::addDays(startTime, t->getDuration());

			setTasksResourcesOcupied(t);
			return 1;
		}
		else {
			CLogger::logger("Task could not be scheduled - RESOURCE CONFLICT");
			return -1;
		}
	}
	return 0;
}

int CScheduler::scheduleTaskCase2(int& startTime, vector<CTask*>tasks, int i)
{
	int j = i - 1;
	// Go back to search if there are less prioritar task already scheduled
	//1. Find out the minimum start point of the task

	time_t minStartPoint = CUtils::substractDays(tasks[i]->getDeadline(), tasks[i]->getDuration());

	//2. Go back until the curent task's priority is higher and there is not enough space for it
	if (j >= 0) {
		while (tasks[j]->getPriority() > tasks[i]->getPriority() && tasks[j]->getStartDate() > minStartPoint && tasks[j]->getHasBeenPlanned() == 1)
		{
			startTime = CUtils::substractDays(startTime, tasks[j]->getDuration());
			tasks[j]->unsetHasBeenPlanned();
			j--;
			if (j < 0)
				break;
		}
	}

	//3. Test if the task can be done or else leave the precedent schedule
	j++;

	if (CUtils::compareDates(startTime, minStartPoint) == true) {
		tasks[i]->scheduleTask(startTime, CUtils::addDays(tasks[i]->getStartDate(), tasks[i]->getDuration()));
		startTime = CUtils::addDays(startTime, tasks[i]->getDuration());

		// Tasks beetwen j and i-1 are not scheduled
		// Give then another shot
		CTask* aux = tasks[i];
		tasks.erase(tasks.begin() + i);
		tasks.insert(tasks.begin() + j, aux);

		return 1;
	}
	else {
		// The task i can't be scheduled
		tasks[i]->unsetHasBeenPlanned();

		// Let the precedent schedule and move on
		for (int t = j; t <= i - 1; t++) {
			tasks[t]->setHasBeenPlanned();
			startTime = CUtils::addDays(startTime, tasks[t]->getDuration());
		}

		//n->addNotification(new CNotification("Activitatea nu a fost planificata", TASK_HIGHER_PRIORITY, tasks[i]));
	}
	return 0;
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

void CScheduler::scheduleTasksForNode(CNode* n)
{
	int startTime = time(0);

	// Sort tasks by deadline
	n->sortTasksByDeadline();

	vector<CTask*> tasks = n->getTasks();
	for (int i = 0; i < tasks.size(); i++)
	{
		CLogger::logger(string("Scheduling task: ") + tasks[i]->getName() + string(" with deadline ") + CUtils::dateToString(tasks[i]->getDeadline(), "%Y.%m.%d") + string(" and duration ") + to_string(tasks[i]->getDuration()));
		bool ok;

		ok = scheduleTaskCase1(startTime, tasks[i]);
		if (ok == 1)
			CLogger::logger(string("Task has been planned starting with the date: ") + CUtils::dateToString(tasks[i]->getStartDate(), "%Y.%m.%d"));
		else if (ok == -1) {
			CLogger::logger("Task could not be scheduled - RESOURCE CONFLICT");
		}
		else if (ok == 0) {
			ok = scheduleTaskCase2(startTime, tasks, i);
			if (ok == 1)
				CLogger::logger(string("Task has been planned starting with the date: ") + CUtils::dateToString(tasks[i]->getStartDate(), "%Y.%m.%d"));
			else
				CLogger::logger("Task has not been planned. There is no room for it. The previous timetable will not be changed!");
		}
	}
}

void CScheduler::scheduleAll()
{
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

void CScheduler::readNodesFromFile(string filename)
{
	ifstream f(filename);
	f.close();
}

void CScheduler::readResourcesFromFile(string filename)
{
	ifstream f(filename);
	string resourceName;

	CResource* r;

	while (f >> resourceName) {
		r = new CResource(resourceName);
		resources.push_back(r);
	}

	f.close();
}

void CScheduler::DFS(CNode* node)
{
	cout << node->getName() << endl;
	for (int i = 0; i < node->getChildren().size(); i++)
	{
		DFS(node->getChildren()[i]);
	}
}
