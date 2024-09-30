#include "pch.h"
#include "scheduleGenerator.h"
#include "common.h"
#include <iomanip>  // For std::setw




void ScheduleGenerator::generateSchedule() {
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
	for (int day = 0; day < daysInSchedual; day++) {
		for  (int hour = 0; hour < hoursPerDay; hour++) {
			for (int classIndex = 0; classIndex < numOfClasses; classIndex++) {
				bool tryingNextTeacher = true;
				int triedTeachers = 0;
				int numOfTeacherOptions = classes[classIndex].size();

				while (tryingNextTeacher) {
					triedTeachers=hour*2;
					triedTeachers++;
					int chunkAndTeacher = nextTeacherRequierment(classIndex, triedTeachers);
					
					int teacher = chunkAndTeacher % 100;

					if (chunkAndTeacher != -1) {
						int hourIdentifier = day * 24 + hour;

						// Check teacher availability and general blocked hours
						if (isTeacherAvailable(teacher, hourIdentifier) &&
							!isHourBlockedForAll(hourIdentifier) 
							&& !isTeacherTeachingInThisHour(teacher,day ,hour)) {
							
							schedual[hour][day][classIndex] = chunkAndTeacher;
							print("new schedule: ", "class:", classIndex, " day:", day, " hour", hour," teacher:", teacher, "\n");
							
							deleteHourFromTeacherInRequierments(classIndex, teacher);
							tryingNextTeacher = false; // Successful assignment
						}
						else {
							schedual[hour][day][classIndex] = -1; // Mark as unscheduled
							tryingNextTeacher = false; // Stop trying teachers
							
						}
					}
					else {
						schedual[hour][day][classIndex] = -1; // Mark as unscheduled
						tryingNextTeacher = false; // Stop trying teachers
					}
					
				}
			}
		}
	}
}






bool ScheduleGenerator::fixNotOptimizedChunk(int classIndex,int currentChunkIdentifier, int currentChunkCount, int preferredChunk, int teacher, int nextHourTeacher, int prevHourTeacher) {
	bool result = false;

	
	auto processChunk = [&](bool isNextChunk) {
		int chunkSize = isNextChunk ? sizeOfNextChunk(classIndex, currentChunkIdentifier) : sizeOfPrevChunk(classIndex, currentChunkIdentifier,currentChunkCount);
		int completingChunkSize = preferredChunk - currentChunkCount;
		int completingChunkIdentifier = -1;

		// Check chunk validity
		if (chunkSize > preferredChunk || (isNextChunk ? !isNextTeacherChunkIsPreferredChunk() : !isPrevTeacherChunkIsPreferredChunk()) || completingChunkSize != chunkSize) {
			return false;
		}

		int triedChunks = 0;
		while ((completingChunkIdentifier = (isNextChunk ? nextChunkOfteacher(teacher, size, triedChunks) : prevChunkOfteacher(teacher, size, triedChunks))) != -1) {
			if (completingChunkIdentifier != -1) {
				
				std::string direction = isNextChunk ? "next" : "prev";
				int nextOrPrevTeacher = isNextChunk ? nextHourTeacher : prevHourTeacher;
				result = replaceTeachers( classIndex,teacher, nextOrPrevTeacher, direction, currentChunkIdentifier, completingChunkIdentifier, completingChunkSize);

				if (result) {
					return true;  // Successfully replaced teachers
				}
			}
			triedChunks++;
		}
		return false;  
		};

	// Process next chunk
	if (processChunk(true)) {
		return true;  // Early return if next chunk was successful
	}

	// Process previous chunk
	return processChunk(false);  // Return result of previous chunk processing
}


bool 


bool ScheduleGenerator::replaceTeachers(int classIndex ,int teacher, int nextOrPrevTeacher, std::string& direction, int currentChunkIdentifier, int currentChunkCount, int completingChunkIdentifier, int completingChunkSize) {
	int currentChunkHour = currentChunkIdentifier % 10;
	int currentChunkDay = (currentChunkIdentifier - currentChunkHour) / 24;

	int completingChunkHour = completingChunkIdentifier % 10;
	int completingChunkDay = (completingChunkIdentifier - currentChunkHour) / 24;

	int targetChunkFirstHour;

	if (direction == "next")
	{
		targetChunkFirstHour = currentChunkHour + 1;
	}

	if (direction == "prev")
	{
		targetChunkFirstHour = currentChunkHour - currentChunkCount + 1 - completingChunkSize + 1;
	}

	for (int hourIndex = targetChunkFirstHour; hourIndex < completingChunkSize; hourIndex++) {

		if (isTeacherTeachingInThisHour(teacher,currentChunkDay, hourIndex))
		{
			return false;
		}
	}
	
	for (int hourIndex = completingChunkHour; hourIndex < completingChunkSize; hourIndex++) {

		if (isTeacherTeachingInThisHour(nextOrPrevTeacher, completingChunkDay, hourIndex))
		{
			return false;
		}
	}

	//replace


	for (int hourIndex = targetChunkFirstHour; hourIndex < completingChunkSize; hourIndex++) {

		int cell = schedual[hourIndex][currentChunkDay][classIndex];
		schedual[hourIndex][currentChunkDay][classIndex] = (cell - cell % 100) + teacher;
	}

	for (int hourIndex = completingChunkHour; hourIndex < completingChunkSize; hourIndex++) {

		int cell = schedual[hourIndex][completingChunkDay][classIndex];
		schedual[hourIndex][completingChunkDay][classIndex] = (cell - cell % 100) + nextOrPrevTeacher;
	}



	return true;
	
}


int ScheduleGenerator::sizeOfNextChunk(int classIndex ,int identifier) {

	int chunkSizeCounter = 0;
	int hour = identifier % 10;
	int day = (identifier - identifier) / 24;
	int hourIndex = hour+1;
	if (hourIndex >= hoursPerDay || schedual[hourIndex][day][classIndex] == -1) {
		return 0;
	}
	int teacher = schedual[hourIndex][day][classIndex] % 100;
	int nextTeacher = teacher;
	while (schedual[hourIndex][day][classIndex] != -1 && teacher == nextTeacher)
	{
		hourIndex++;
		chunkSizeCounter++;
		if (schedual[hourIndex][day][classIndex] != -1)
		{
			nextTeacher = schedual[hourIndex][day][classIndex] % 100;
		}
		

	}
	return chunkSizeCounter;

}
int ScheduleGenerator::sizeOfPrevChunk(int classIndex, int identifier, int currentChunkCount) {

	int chunkSizeCounter = 0;
	int hour = identifier % 10;
	int day = (identifier - identifier) / 24;
	int hourIndex = hour + 1 - currentChunkCount;
	int teacher = schedual[hourIndex-1][day][classIndex] % 100;
	int prevTeacher = teacher;
	while (schedual[hourIndex][day][classIndex] != -1 && teacher == prevTeacher)

	
	
	

	if (hourIndex <= 0 || schedual[hourIndex-1][day][classIndex] == -1) {
		return 0;
	}
	
	while (schedual[hourIndex][day][classIndex] != -1 && teacher == prevTeacher && hourIndex >= 0)
	{
		hourIndex--;
		chunkSizeCounter++;
		if (schedual[hourIndex][day][classIndex] != -1 && hourIndex >= 0)
		{
			prevTeacher = schedual[hourIndex][day][classIndex] % 100;
		}


	}
	return chunkSizeCounter;





}



void ScheduleGenerator::improveSchedule() {
	// fix horrible coding
	int nextHourTeacher = -1;
	int recentTeacher = -1;
	int teacher = -1;
	int chunkHours = -1;
	int preferredChunk = -1;
	for (int classIndex = 0; classIndex < numOfClasses; classIndex++) {
		for (int day = 0; day < daysInSchedual; day++) {
			int hourIndex = 0;

			while (hourIndex < hoursPerDay)
			{
				int currentChunkCount = 0;
				int cell = schedual[hourIndex][day][classIndex];
				if (cell == 1)
				{
					continue;
				}
				int teacher = cell % 100;
				int preferredChunk = (cell - cell % 100) / 100;
				int nextHourTeacher = teacher;

				while (teacher == nextHourTeacher) 
				{
					if (hourIndex + 1 < hoursPerDay) // if it's not the last hour
					{
						nextHourTeacher = schedual[hourIndex + 1][day][classIndex] % 100;
					}
					else {
						nextHourTeacher = -1; //then nextHourTeacher != teacher is true for the next check

					}
					
					currentChunkCount++;
					
					if (nextHourTeacher != teacher && currentChunkCount % preferredChunk != 0)
					{

						int identifier = day * 24 + hourIndex;
						int prevHourTeacher =  schedual[hourIndex - currentChunkCount][day][classIndex] % 100;
						bool isPatched = fixNotOptimizedChunk(classIndex,identifier,currentChunkCount,preferredChunk, teacher, nextHourTeacher, prevHourTeacher);
						if (isPatched) { print("patched bad: ", "day: ", day, "hour: ", hourIndex, "class: ", classIndex); }
						else { print("didn't patched bad: ", "day: ", day, "hour: ", hourIndex, "class: ", classIndex); }
					}
					
				hourIndex++;
				}
			}
		}
	}
}

bool ScheduleGenerator::isTeacherTeachingInThisHour(int teacher, int day,int hour)
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
bool ScheduleGenerator::isTeacherAvailable(int teacher, int hourIdentifier) {
	return teachers[teacher].empty() || teachers[teacher].find(hourIdentifier) == teachers[teacher].end();
}
bool ScheduleGenerator::isHourBlockedForAll(int hourIdentifier) {
	return generalBlockedHoursForEveryone.find(hourIdentifier) != generalBlockedHoursForEveryone.end();
}

void ScheduleGenerator::deleteHourFromTeacherInRequierments(int classIndex, int teacher) {
	for (auto& pair : classes[classIndex]) {
		if (pair.first == teacher && pair.second > 0) {
			pair.second = pair.second - 1;; // Reduce available hours for this teacher
			break; 
		}
	}
}

int ScheduleGenerator::nextTeacherRequierment(int classIndex, int triedTeachers) {
	int triedAndBlocked = 0;

	


	for (const auto& pair : classes[classIndex]) {
		if (triedAndBlocked >= triedTeachers && pair.second > 0) {
			// Return the teacher and chunk combination
			return preferredChunkOfTeacherHoursForClass(pair.first, classIndex) * 100 + pair.first;
		}
		triedAndBlocked++;

	}

	return -1; // No more teachers available
}
int ScheduleGenerator::preferredChunkOfTeacherHoursForClass(int teacher, int classIndex) {

	for (const auto& pair : classesPreferredChunks[classIndex])
	{
		if (pair.first == teacher)
		{
			return pair.second;
		}
	}
	return -1;
	
}


void ScheduleGenerator::initilizeDeatails() {

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
void ScheduleGenerator::initilizeGeneralBlockedHours() {

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
				// todo: add validation , for now no need to validate day/hour because it will just not use if not vaild 
				print("what day? ");
				std::cin >> day;
				print("what hour? ");
				std::cin >> hour;
				print("what reason? ");
				std::cin >> reason;

				int identifier = day * 24 + hour;
				generalBlockedHoursForEveryone[identifier] = reason;

			}
		}
	}

}
//todo: divide into initilize all teachers and to initilize teacher 
void ScheduleGenerator::initilizeTeachers()
{
	print("************ starting with teachers settings *************\n");
	int indexOfTeacher = 0;
	while (true)
	{
		print("add a teacher? y/n ");
		char answer = yesOrNoDialog();
		if (answer == 'n')
		{
			numOfTeachers = indexOfTeacher;
			return;
		}
		if (answer == 'y')
		{
			std::string name;
			print("what is the teacher's name? ");
			std::cin >> name;
			this->teachersNames.push_back(name);
			print("this teacher has the identifier ", indexOfTeacher, "\n");
			indexOfTeacher = indexOfTeacher + 1;


			print("does this teacher have restrictions? y/n ");
			char answer = yesOrNoDialog();
			if (answer == 'n')
			{
				continue;
			}
			else {

				while (true) {

					print("do you want to add a restriction? y/n ");
					char answer = yesOrNoDialog();
					if (answer == 'n')
					{
						break;
					}
					else {
						int day;
						int hour;
						std::string reason;
						// no validate day/hour cause it will just not use if not vaild 
						print("what day? ");
						std::cin >> day;
						print("what hour? ");
						std::cin >> hour;
						print("what reason? ");
						std::cin >> reason;

						int identifier = day * 24 + hour;

						teachers[indexOfTeacher][identifier] = reason;
					}

				}
			}
		}



	}
	print("************ finished with teachers settings *************\n\n\n");
	numOfTeachers = indexOfTeacher;
}
void ScheduleGenerator::initilizeClasses()
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

	addTeacherHoursForClass(classesIndex,0, 8, 1, 2); 
	addTeacherHoursForClass(classesIndex, 3, 8, 1, 2);
	addTeacherHoursForClass(classesIndex, 5, 4, 1, 2); // Teacher 0: 4 hours, priority 1, chunks of 2 hours
	addTeacherHoursForClass(classesIndex, 4, 2, 3, 1);  // Teacher 1: 5 hours, priority 3, chunks of 2 hours

	// Class 1
	classesIndex++;
	classes.resize(classesIndex + 1);
	classesNames.push_back("Class 1");

	addTeacherHoursForClass(classesIndex, 3, 2, 1, 1);
	addTeacherHoursForClass(classesIndex, 3, 8, 1, 2);
	addTeacherHoursForClass(classesIndex, 5, 4, 1, 2);// Teacher 2: 4 hours, priority 1, chunks of 2 hours

	numOfClasses = 2;
	classesPreferredChunks = classes;

	print("************ finished with classes settings *************\n\n\n");
}


void ScheduleGenerator::addTeacherHoursForClass(int classesIndex, int teacherIndex, int hoursNum, int priority, int chunksSize)
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



void ScheduleGenerator::printHoursRequierementsForClass(int classIndex) {

	for (const auto& hour : classes[classIndex]) {

		print("teacher: ", hour.first, " hours: ", hour.second, "\n");

	}

}
void ScheduleGenerator::printHoursRequirementsForAllClasses()
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
void ScheduleGenerator::printAllteachers()

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
void ScheduleGenerator::printTeachersNames()
{
	for (int i = 0; i < numOfTeachers; i++)
	{
		print(i, ".", teachersNames[i], "\n");
	}

}

//for most of that func, thanks for chat gpt lol
void ScheduleGenerator::printSchedule() {
	const int columnWidth = 15;  // Define uniform width for each column

	// Print the header (days)
	std::cout << std::setw(columnWidth) << " ";  // Empty space for the hour labels
	for (int day = 0; day < daysInSchedual; day++) {
		std::cout << std::setw(columnWidth) << "Day " + std::to_string(day);
	}
	std::cout << std::endl;

	// Iterate over each hour of the day
	for (int hour = 0; hour < hoursPerDay; hour++) {
		// Print the hour label
		std::cout << std::setw(columnWidth) << "Hour " + std::to_string(hour + 1);

		// Iterate over each day
		for (int day = 0; day < daysInSchedual; day++) {
			std::string cellContent;
			bool hasClass = false;
			print(" ; ");
			// Collect information for each class at this day and hour
			for (int classIndex = 0; classIndex < numOfClasses; classIndex++) {
				int cell = schedual[hour][day][classIndex];
				if (cell != -1) {
					hasClass = true;
					int teacher = cell % 100;
					int preferredChunk = (cell - cell % 100) / 100;

					cellContent += " C" + std::to_string(classIndex) + " T" + std::to_string(teacher) + " P" + std::to_string(preferredChunk) + " | ";
					
				}
			}

			if (!hasClass) {
				cellContent = "Free";  // If no class is scheduled
			}

			// Print the cell content with fixed width
			std::cout << std::setw(columnWidth) << cellContent;
		}
		std::cout << std::endl;  // Move to the next hour row
	}
}
