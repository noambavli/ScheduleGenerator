#include "pch.h"
#include "scheduleGenerator.h"


int main()

{
	ScheduleGenerator* schedule = new ScheduleGenerator();
	schedule->initilizeDeatails();
	schedule->printAllteachers();
	schedule->printHoursRequirementsForAllClasses();
	schedule->generateSchedule();
	schedule->printSchedule();
	schedule->improveSchedule();
	schedule->printSchedule();
	schedule->printUnscheduledHours();
	schedule->printUnscheduledHoursDueTeachersBlockedTimes();

}