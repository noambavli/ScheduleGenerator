


#include "pch.h"
#include "schedualGenerator.h"
#include "common.h"
void SchedualGenerator::generateSchedule() {
	schedual = new int** [hoursPerDay];

	// Initialize schedule array
	for (int hour = 0; hour < hoursPerDay; hour++) {
		schedual[hour] = new int* [daysInSchedual];
		for (int day = 0; day < daysInSchedual; day++) {
			schedual[hour][day] = new int[numOfClasses]();
			for (int classIndex = 0; classIndex < numOfClasses; classIndex++)
			{
				schedual[hour][day][classIndex] = -1;
			}
		}
	}

	// Scheduling logic
	for (int hour = 0; hour < hoursPerDay; hour++) {
		for (int day = 0; day <= daysInSchedual; day++) {
			for (int classIndex = 0; classIndex < numOfClasses; classIndex++) {
				bool tryingNextTeacher = true;
				int triedTeachers = 0;
				int numOfTeacherOptions = classes[classIndex].size();

				while (tryingNextTeacher) {
					int chunkAndTeacher = nextTeacherRequierment(classIndex, triedTeachers);
					int teacher = chunkAndTeacher % 100;

					if (chunkAndTeacher != -1) {
						int hourIdentifier = day * 24 + hour;

						// Check teacher availability and general blocked hours
						if (isTeacherAvailable(teacher, hourIdentifier) &&
							!isHourBlockedForAll(hourIdentifier) 
							&& !teacherIsTeachingInThisHour(teacher,day ,hour)) {
							
							schedual[hour][day][classIndex] = chunkAndTeacher;
							deleteHourFromTeacherInRequierments(classIndex, teacher);
							tryingNextTeacher = false; // Successful assignment
						}
						else {
							triedTeachers++;
						}
					}
					else if (triedTeachers >= numOfTeacherOptions) {
						schedual[hour][day][classIndex] = -1; // Mark as unscheduled
						tryingNextTeacher = false; // Stop trying teachers
					}
				}
			}
		}
	}
}


bool SchedualGenerator::teacherIsTeachingInThisHour(int teacher, int day,int hour)
{
	for (int classIndex = 0; classIndex < numOfClasses; classIndex++)
	{
		try
		{
			if (schedual[hour][day][classIndex] == teacher)
			{
				return true;
			}
		}
		catch (const std::exception& e)
		{
			print("Exception occurred: ", e.what(), "\n");
		}
	

	}
	return false;

}


bool SchedualGenerator::isTeacherAvailable(int teacher, int hourIdentifier) {
	return teachers[teacher].empty() || teachers[teacher].find(hourIdentifier) == teachers[teacher].end();
}

bool SchedualGenerator::isHourBlockedForAll(int hourIdentifier) {
	return generalBlockedHoursForEveryone.find(hourIdentifier) != generalBlockedHoursForEveryone.end();
}

void SchedualGenerator::deleteHourFromTeacherInRequierments(int classIndex, int teacher) {
	for (auto& pair : classes[classIndex]) {
		if (pair.first == teacher) {
			pair.second -= 1; // Reduce available hours for this teacher
			break; 
		}
	}
}

int SchedualGenerator::nextTeacherRequierment(int classIndex, int triedTeachers) {
	int triedAndBlocked = 0;

	


	for (const auto& pair : classes[classIndex]) {
		if (triedAndBlocked >= triedTeachers && pair.second > 0) {
			// Return the teacher and chunk combination
			return pair.second * 100 + preferredChunkOfTeacherHoursForClass(pair.second,classIndex);
		}
		triedAndBlocked++;

	}

	return -1; // No more teachers available
}
void SchedualGenerator::initilizeDeatails() {

	print("WELCOME!\n");
	//todo:check input 
	print("how many days in the schedual? ");
	daysInSchedual = 6;
	//std::cin >> this->daysInSchedual;
	print("how many hours per day? ");
	//std::cin >> this->hoursPerDay;
	hoursPerDay = 9;
	initilizeGeneralBlockedHours();
	initilizeTeachers();
	initilizeClasses();

}


int SchedualGenerator::preferredChunkOfTeacherHoursForClass(int teacher, int classIndex) {

	for (const auto& pair : classesPreferredChunks[classIndex])
	{
		if (pair.first == teacher)
		{
			return pair.second;
		}
	}
	return -1;
	
}


void SchedualGenerator::initilizeTeachers()
{
	print("************ starting with teachers settings *************\n");

	// Predefined teachers
	this->teachersNames.push_back("lili");  // Teacher 0
	this->teachersNames.push_back("mia");   // Teacher 1
	this->teachersNames.push_back("nill");  // Teacher 2

	// No restrictions for any teacher
	numOfTeachers = 3;

	print("************ finished with teachers settings *************\n\n\n");
}

void SchedualGenerator::initilizeClasses()
{
	print("\n\n\n************ started with classes settings *************\n");
	print("for every class, for every teacher the class will learn with,\n");
	print("you now want to provide the teacher number, how many hours in every \n ");
	print("schedule (generally week), the priority of the teacher (subject) \n");
	print("and the preferred chunks of hours (1-hour lessons, 2-hours lessons, 4-hours lessons, etc)\n");
	print("here are the teachers numbers and names: \n");
	print("_________________________________\n");
	printTeachersNames();
	print("_________________________________\n");

	// Predefined classes and teacher assignments
	// Class 0
	int classesIndex = 0;
	classes.resize(classesIndex + 1);
	classesNames.push_back("Class 0");

	addTeacherHoursForClass(classesIndex, 0, 4, 1, 2);  // Teacher 0: 4 hours, priority 1, chunks of 2 hours
	addTeacherHoursForClass(classesIndex, 1, 5, 3, 2);  // Teacher 1: 5 hours, priority 3, chunks of 2 hours

	// Class 1
	classesIndex++;
	classes.resize(classesIndex + 1);
	classesNames.push_back("Class 1");

	addTeacherHoursForClass(classesIndex, 2, 4, 1, 2);  // Teacher 2: 4 hours, priority 1, chunks of 2 hours

	numOfClasses = 2;
	classesPreferredChunks = classes;

	print("************ finished with classes settings *************\n\n\n");
}


void SchedualGenerator::addTeacherHoursForClass(int classesIndex, int teacherIndex, int hoursNum, int priority, int chunksSize)
{


	int numOfFilledChunks = (hoursNum - hoursNum % chunksSize) / chunksSize;
	int numOfHoursInUnfilledChunk = hoursNum % chunksSize;
	bool needUnfilled = false;
	if (numOfHoursInUnfilledChunk != 0) { needUnfilled = true; }

	std::pair<int, int> newPair = std::make_pair(0, 0);
	std::pair<int, int> newPairUnfilled = std::make_pair(0, 0);
	//diff logic in a way that will make more sense to duplicate code even tho bad practice
	//todo: maybe rethink of this spesific part in func algorithm
	if (hoursNum / 2 < chunksSize && hoursNum <= chunksSize) {

		newPair.first = teacherIndex;
		newPair.second = chunksSize;
		newPairUnfilled.first = teacherIndex;
		newPairUnfilled.second = hoursNum - chunksSize;

		if (priority == 1)
		{
			classes[classesIndex].push_back(newPair);
			if (needUnfilled) { classes[classesIndex].push_back(newPairUnfilled); }
		}

		if (priority == 2)
		{	//push to the middle

			int listSize = classes[classesIndex].size();
			auto iter = classes[classesIndex].begin();
			if (listSize % 2 == 0) { std::advance(iter, listSize / 2); }
			if (listSize % 2 != 0) { std::advance(iter, listSize - 1 / 2); }

			if (needUnfilled) { classes[classesIndex].insert(iter, newPairUnfilled); }
			classes[classesIndex].insert(iter, newPair);
		}
		if (priority == 3)
		{
			if (needUnfilled) { classes[classesIndex].push_front(newPairUnfilled); }
			classes[classesIndex].push_front(newPair);

		}
		//printHoursRequierementsForClass(classesIndex);

	}
	else {


		newPair.first = teacherIndex;
		newPair.second = chunksSize;
		newPairUnfilled.first = teacherIndex;
		newPairUnfilled.second = numOfHoursInUnfilledChunk;


		for (int i = 1; i <= numOfFilledChunks; i++)
		{

			if (priority == 1)
			{
				classes[classesIndex].push_back(newPair);
				if (i == numOfFilledChunks && needUnfilled) { classes[classesIndex].push_back(newPairUnfilled); }
			}

			if (priority == 2)
			{	//push to the middle

				int listSize = classes[classesIndex].size();
				auto iter = classes[classesIndex].begin();
				std::advance(iter, listSize / 2);

				if (i == 0 && needUnfilled) { classes[classesIndex].insert(iter, newPairUnfilled); }
				classes[classesIndex].insert(iter, newPair);
			}
			if (priority == 3)
			{
				if (i == 1 && needUnfilled) { classes[classesIndex].push_front(newPairUnfilled); }
				classes[classesIndex].push_front(newPair);

			}




		}

	}
}


void SchedualGenerator::printHoursRequierementsForClass(int classIndex) {

	for (const auto& hour : classes[classIndex]) {

		print("teacher: ", hour.first, " hours: ", hour.second, "\n");

	}

}


void SchedualGenerator::printHoursRequirementsForAllClasses()
{
	print("============ all classes ============\n");
	for (int i = 0; i < numOfClasses; i++)
	{
		print("-------------------------------\n");
		print("class identifier: ", i, "\n");
		printHoursRequierementsForClass(i);
		print("-------------------------------\n");
	}

}


void SchedualGenerator::initilizeGeneralBlockedHours() {

	char answer;

	print("do you have any spesific hours in spesific days that are blocked for everyone? y/n ");
	answer = yesOrNoDialog();
	if (answer == 'n')
	{
		return;
	}
	if (answer == 'y')
	{
		while (true)
		{
			print("add an hour? y/n ");
			answer = yesOrNoDialog();
			if (answer == 'n') {
				return;
			}
			else {
				int day;
				int hour;
				std::string reason;
				// no need to validate day/hour because it will just not use if not vaild 
				print("what day? ");
				std::cin >> day;
				print("what hour? ");
				std::cin >> hour;
				print("what reason? ");
				std::cin >> reason;

				int identifier = day * 24 + hour;
				this->generalBlockedHoursForEveryone[identifier] = reason;

			}
		}
	}

}

void SchedualGenerator::printSchedual() {

	for (int hour = 0; hour < hoursPerDay; hour++) {

		for (int day = 0; day < daysInSchedual; day++) {
			if (schedual[hour][day] == NULL)
			{
				for (int i = 0; i < numOfClasses; i++)
				{
					print(0);
					continue;
				}
			}
			else {
				print("\n");
				for (int classIndex = 0; classIndex < numOfClasses; classIndex++) {

					print(" ", schedual[hour][day][classIndex], " ");
				}
			}
		}

	}



}

void SchedualGenerator::printAllteachers()

{
	print("============ all teachers ============\n");

	for (int i = 0; i < numOfTeachers; ++i)
	{

		if (!teachers[i].empty()) {
			print("identifier: ", i, " name: ", teachersNames[i]);
			print(" blocked times: ");
			for (const auto& blockedTime : teachers[i])
			{

				print("day:", ((blockedTime.first - blockedTime.first % 10) / 24), " hour:", blockedTime.first % 24, " reason:", blockedTime.second, "\n");

			}
		}
		else {
			print("identifier: ", i, " name: ", teachersNames[i], "\n");

		}

	}





}

void SchedualGenerator::printTeachersNames()
{
	for (int i = 0; i < numOfTeachers; i++)
	{
		print(i, ".", teachersNames[i], "\n");
	}

}