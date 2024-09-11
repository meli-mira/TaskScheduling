#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
#include "CScheduler.h"
#include "CTimetable.h"
int N;

typedef struct process {
	char name;
	int duration, start, end, priority, deadline;
	int isOk;
}process;
process P[10];

void put(int N) {
	printf("PROCESS\tSTART-TIME\tEND-TIME\n");
	for (int i = 0; i < N; i++) {
		if (P[i].isOk == 0) printf("---------Process  %c misses----------\n", P[i].name);
		else printf("%c\t%d\t\t%d\n", P[i].name, P[i].start, P[i].end);
	}
}
void sortByDeadline()
{
	for (int i = 0; i < N - 1; i++)
	{
		for (int j = i + 1; j < N; j++)
			if (P[i].deadline > P[j].deadline || (P[i].deadline == P[j].deadline && P[i].priority < P[j].priority))
			{
				process aux = P[i];
				P[i] = P[j];
				P[j] = aux;
			}
	}
}
void schedule()
{
	sortByDeadline();

	int startTime = 0;
	for (int i = 0; i < N; i++)
	{
		if (startTime + P[i].duration <= P[i].deadline)
		{
			P[i].start = startTime;
			P[i].end = startTime + P[i].duration;
			startTime += P[i].duration;
			P[i].isOk = 1;
		}
		else
		{
			int j = i-1;
			//go back to search if there are less prioritar task already scheduled
			//1. find out the minim start point of the task
			int minStartPoint = P[i].deadline - P[i].duration;

			//2. go back until the curent task's priority is higher and there is not enough space for it
			while (P[j].priority > P[i].priority && P[j].start < minStartPoint && j>=0)
			{
				startTime -= P[j].duration;
				P[j].isOk = 0;
				j--;
			}

			//3. test if can be done or else leave the precedent schedule
			j++;
			if (startTime <= minStartPoint)
			{
				P[i].start = P[j].start;
				P[i].end = P[i].start + P[i].duration;
				P[i].isOk = 1;
				startTime += P[i].duration;

				// Tasks beetwen j and i-1 are not scheduled
				// Give then another shot
				process aux = P[i];
				for (int t = i; t > j; t--)
					P[t] = P[t - 1];
				P[j] = aux;

			}
			else
			{
				// The task i can't be scheduled
				P[i].isOk = 0;

				// Let the precedent schedule and move on
				for (int t = j; t <= i - 1; t++) {
					P[t].isOk = 1;
					startTime += P[t].duration;
				}
			}
		}
	}
}
void computeOutFileForPloting()
{
	FILE* g = fopen("out.txt","w");

	for (int i = 0; i < N; i++)
	{
		if (P[i].isOk == 1)
		{
			int j = 1;
			while (j <= P[i].duration)
			{
				fprintf(g, "t%c\n", P[i].name);
				j++;
			}
		}
	}


	fclose(g);
}
int main() {
	
	CNode* n1 = new CNode("NOD1", 11, 20, NULL,2 , 1);

	n1->readTaskFromFile("tasks1.txt");
	 
	CScheduler::getInstance()->scheduleTasksForNode(n1);
	CScheduler::getInstance()->printScheduledTasks(n1);

	CScheduler::getInstance()->computeOutFileForPloting(n1);

	////CScheduler::getInstance()->DFS(n1);
	//CTimetable t(CUtils::parseDateTime("2024.02.05", "%Y.%m.%d"));
	//
	//
	//time_t start = CUtils::parseDateTime("2024.04.15", "%Y.%m.%d"), stop = CUtils::parseDateTime("2024.05.15", "%Y.%m.%d");
	////t.print();
	////cout << t.getNrOfJobsBetween(CUtils::parseDateTime("2024.04.11", "%Y.%m.%d"), CUtils::parseDateTime("2024.06.15", "%Y.%m.%d"));
	////cout << t.verifyInterval(start, stop) << endl;
	//cout << nodeCapacity << " " << CUtils::dateToString(stop, "%Y.%m.%d");
	return 0;
}
