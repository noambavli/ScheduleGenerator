#include "pch.h"
#include "schedualGenerator.h"


int main()

{
	SchedualGenerator* schedual = new SchedualGenerator();
	schedual->initilizeDeatails();
	schedual->printAllteachers();
	schedual->printHoursRequirementsForAllClasses();
	schedual->generateSchedule();
	schedual->printSchedule();
}