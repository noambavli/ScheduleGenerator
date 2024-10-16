#include "pch.h"
#include "scheduleGenerator.h"
#include "common.h"
#include <iomanip>  // For std::setw




void ScheduleGenerator::generateSchedule() {
	schedule = new int** [hoursPerDay];

	// Initialize schedule array
	for (int hour = 0; hour < hoursPerDay; hour++) {
		schedule[hour] = new int* [daysInSchedual];
		for (int day = 0; day < daysInSchedual; day++) {
			schedule[hour][day] = new int[numOfClasses]();
			for (int classIndex = 0; classIndex < numOfClasses; classIndex++)
			{
				schedule[hour][day][classIndex] = -1;
			}
		}
	}

	int chunkAndTeacher, teacher, preferredChunk, hourIndex, hourIdentifier;
	int day = 0, hour = 0, classIndex = 0, triedTeachers = 0, hourCount = 0;

	// Scheduling logic #1


	/*


	for (int day = 0; day < daysInSchedual; day++) {
		for (int hour = 0; hour < hoursPerDay; hour++) {
			for (int classIndex = 0; classIndex < numOfClasses; classIndex++) {


				bool tryingNextTeacher = true;
				int triedTeachers = 0;
				int numOfTeacherOptions = classes[classIndex].size();
				int chunkAndTeacher = nextTeacherRequierment(classIndex, triedTeachers);
				int lastTeacher = chunkAndTeacher % 100;

				while (tryingNextTeacher) {
					triedTeachers++;
					int teacher = chunkAndTeacher % 100;

					if (lastTeacher != teacher) {
						lastTeacher = teacher;  // Update last teacher
					}

					if (chunkAndTeacher != -1) {
						int hourIdentifier = day * 24 + hour;

						// Check teacher availability and general blocked hours
						if (isTeacherAvailable(teacher, hourIdentifier) &&
							!isBlockedTimeForClass(classIndex, hourIdentifier) &&
							!isHourBlockedForAll(hourIdentifier) &&
							!isTeacherTeachingInThisHour(teacher, day, hour)) {

							// Assign teacher and update schedule
							schedule[hour][day][classIndex] = chunkAndTeacher;
							print("new schedule: ", "class:", classIndex, " day:", day, " hour:", hour, " teacher:", teacher, "\n");

							// Remove hour from teacher's requirements
							deleteHourFromTeacherInRequierments(classIndex, teacher, 0, false);
							tryingNextTeacher = false; // Successful assignment
						}
						else if (triedTeachers < numOfTeacherOptions) {
							// If the current teacher is unavailable, try the next teacher
							chunkAndTeacher = nextTeacherRequierment(classIndex, triedTeachers);
						}
						else {
							// If all teachers were tried and none are available
							schedule[hour][day][classIndex] = -1; // Mark as unscheduled
							tryingNextTeacher = false; // Stop trying
						}
					}
					else {
						// If no valid teacher was found
						schedule[hour][day][classIndex] = -1; // Mark as unscheduled
						tryingNextTeacher = false; // Stop trying
					}
				}
			}
		}
	}
}
*/


// Scheduling logic #2


	while (true) {
		if (numOfClasses == 0) {
			break;
		}

		while (classIndex < numOfClasses) {

			if (hour >= hoursPerDay) {
				hour = 0;
				day++;
			}

			if (day >= daysInSchedual) {
				break;
			}

			while (nextTeacherRequierment(classIndex, triedTeachers) != -1) {

				if (hour >= hoursPerDay) {
					hour = 0;
					day++;
				}
				if (day >= daysInSchedual) {
					break;
				}

				chunkAndTeacher = nextTeacherRequierment(classIndex, triedTeachers);
				teacher = chunkAndTeacher % 100;
				preferredChunk = chunkAndTeacher / 100;

				hourCount = 0;

				while (hourCount < preferredChunk) {
					hourIdentifier = day * 24 + hour;

					if (!isTeacherAvailable(teacher, hourIdentifier) ||
						isBlockedTimeForClass(classIndex, hourIdentifier) ||
						isHourBlockedForAll(hourIdentifier) ||
						isTeacherTeachingInThisHour(teacher, day, hour)) {

						//triedTeachers++;
						break;
					}

					if (hourCount == preferredChunk - 1) {
						scheduleTeacherClassHours(day, hour, classIndex, teacher, preferredChunk, chunkAndTeacher);
						hour += preferredChunk;
						classIndex++;
						triedTeachers = 0;
						break;
					}

					hourCount++;
				}

			}
			if (classIndex < numOfClasses) {
				break;
			}

			if (day >= daysInSchedual) {
				break;
			}
		}
	}
}

bool ScheduleGenerator::scheduleTeacherClassHours(int day, int hour, int classIndex, int teacher, int preferredChunk, int chunkAndTeacher)
{
	
	for (int hourIndex = hour; hourIndex < hour + preferredChunk; hourIndex++)
	{
		int hourIdentifier = day * 24 + hourIndex;

		if (isTeacherAvailable(teacher, hourIdentifier) &&
			!isBlockedTimeForClass(classIndex, hourIdentifier) &&
			!isHourBlockedForAll(hourIdentifier) &&
			!isTeacherTeachingInThisHour(teacher, day, hourIndex)) {

			schedule[hourIndex][day][classIndex] = chunkAndTeacher;

			print("new schedule: ", "class:", classIndex, " day:", day, " hour:", hourIndex, " teacher:", teacher, "\n");
		}
		else {
			return false;
		}
	}

	deleteHourFromTeacherInRequierments(classIndex, teacher, preferredChunk, true);
	return true;
}



void ScheduleGenerator::improveSchedule() {
	// Initialize variables
	for (int classIndex = 0; classIndex < numOfClasses; classIndex++) {
		for (int day = 0; day < daysInSchedual; day++) {
			int hourIndex = 0;

			while (hourIndex < hoursPerDay) {
				int cell = schedule[hourIndex][day][classIndex];
				if (cell == -1) {
					hourIndex++;
					continue; // Skip unscheduled hours
				}

				int teacher = cell % 100;
				int preferredChunk = (cell - cell % 100) / 100;
				int currentChunkCount = 0;
				int nextHourTeacher = teacher;
				

				// Count consecutive hours for the same teacher
				while (teacher == nextHourTeacher) {
					int identifier = day * 24 + hourIndex;
					if (hourIndex + 1 < hoursPerDay) {
						nextHourTeacher = schedule[hourIndex + 1][day][classIndex] % 100;
					}
					else {
						nextHourTeacher = -1; // End of sequence
					}

					currentChunkCount++;

					// Check for chunk optimization
					if (nextHourTeacher != teacher && currentChunkCount % preferredChunk != 0) {
						int prevHourTeacher = (hourIndex - currentChunkCount >= 0) ? schedule[hourIndex - currentChunkCount][day][classIndex] % 100 : -1;
						bool isPatched = fixNotOptimizedChunk(classIndex, identifier, currentChunkCount, preferredChunk, teacher, nextHourTeacher, prevHourTeacher);

						if (isPatched) {
							print("\nPatched bad: ", "day: ", day, " hour: ", hourIndex, " class: ", classIndex);
						}
						else {
							print("\nDidn't patch bad: ", "day: ", day, " hour: ", hourIndex, " class: ", classIndex);
						}
					}

					hourIndex++;
				}
			}
		}
	}
}

bool ScheduleGenerator::fixNotOptimizedChunk(int classIndex, int currentChunkIdentifier, int currentChunkCount,
	int preferredChunk, int teacher, int nextHourTeacher, int prevHourTeacher) {
	bool result = false;

	auto processChunk = [&](bool isNextChunk) {
		std::pair<int, bool> surroundingChunkInfo = isNextChunk
			? nextChunkInfo(classIndex, currentChunkIdentifier)
			: prevChunkInfo(classIndex, currentChunkIdentifier, currentChunkCount);

		int completingChunkSize = preferredChunk - currentChunkCount;

		// Check if chunk can and needs to be fixed
		if (surroundingChunkInfo.first <= preferredChunk &&
			surroundingChunkInfo.second && 
			completingChunkSize == surroundingChunkInfo.first) {
			int surroundingTeacher = isNextChunk ? nextHourTeacher : prevHourTeacher;

			// Only attempt to replace if the surrounding teacher is different
			if (surroundingTeacher != teacher) {
				result = findCompletingChunkAndReplace(classIndex, teacher, surroundingTeacher, isNextChunk, currentChunkIdentifier, completingChunkSize);
				return true; // Return true if chunk was processed successfully
			}
		}

		return false; // Return false if chunk was not processed
		};

	if (processChunk(true)) {
		return true;  // Early return if next chunk was successful
	}

	// Process previous chunk
	return processChunk(false);  // Return result of previous chunk processing
}


bool ScheduleGenerator::findCompletingChunkAndReplace(int classIndex, int teacher, int surroundingTeacher, bool isNextDirection,
	int currentChunkIdentifier, int completingChunkSize) {
	bool result = false;
	for (int day = 0; day < daysInSchedual; ++day) {
		int targetTeacherChunkSize = 0;
		for (int hour = 0; hour < hoursPerDay; ++hour) {


			int cell = schedule[hour][day][classIndex];
			if (cell == -1) {
				continue;
			}
			int currentTeacher = cell % 100;
			if (currentTeacher == teacher){
				targetTeacherChunkSize++;
			} else {
				targetTeacherChunkSize = 0;
			}
			if (targetTeacherChunkSize == completingChunkSize)
			{
				result = replaceTeachers(classIndex, teacher, surroundingTeacher, isNextDirection, currentChunkIdentifier,
					targetTeacherChunkSize, cell, completingChunkSize);
				if (result) {
					return true;
				}

			}


		}
	}
	return false;
	





}



bool ScheduleGenerator::replaceTeachers(int classIndex ,int teacher, int nextOrPrevTeacher, bool next,
	int currentChunkIdentifier, int currentChunkCount, int completingChunkIdentifier, int completingChunkSize) {
	int currentChunkHour = currentChunkIdentifier % 24;
	int currentChunkDay = currentChunkIdentifier / 24;

	int completingChunkHour = completingChunkIdentifier % 24;
	int completingChunkDay = completingChunkIdentifier  / 24;

	int targetChunkFirstHour;

	if (next)
	{
		targetChunkFirstHour = currentChunkHour + 1;
	}

	if (!next) //prev
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


	for (int hourIndex = targetChunkFirstHour-1; hourIndex < completingChunkSize; hourIndex++) {

		//int cell = schedual[hourIndex][currentChunkDay][classIndex];
		schedule[hourIndex][currentChunkDay][classIndex] =  completingChunkSize * 100 + nextOrPrevTeacher;
	}

	for (int hourIndex = completingChunkHour; hourIndex < completingChunkSize; hourIndex++) {

		schedule[hourIndex][completingChunkDay][classIndex] = (currentChunkCount+completingChunkSize) * 100 + teacher;
	}
	


	return true;
	
}

std::pair<int, bool> ScheduleGenerator::nextChunkInfo(int classIndex ,int identifier) {

	int chunkSizeCounter = 0;
	int hour = identifier % 24;
	int day = identifier / 24;
	int hourIndex = hour+1;

	if (hourIndex >= hoursPerDay)
	{
		return std::make_pair(chunkSizeCounter, false);

	}

	int cell = schedule[hourIndex][day][classIndex];
	int teacher = cell % 100;
	int preferredChunk = (cell - cell % 100) / 100;

	if (hourIndex > hoursPerDay || cell == -1) {
		return std::make_pair(0, false);
	}

	int nextTeacher = teacher;
	while (cell != -1 && teacher == nextTeacher && hourIndex < hoursPerDay-1)
	{
		hourIndex++;
		chunkSizeCounter++;
		if (schedule[hourIndex][day][classIndex] != -1  )
		{
			nextTeacher = schedule[hourIndex][day][classIndex] % 100;
		}
		

	}

	bool isChunkSizePreferredChunk = (chunkSizeCounter == preferredChunk);
	return std::make_pair(chunkSizeCounter, isChunkSizePreferredChunk);;

}

std::pair<int, bool> ScheduleGenerator::prevChunkInfo(int classIndex, int identifier, int currentChunkCount) {

	int chunkSizeCounter = 0;
	int hour = identifier % 24;
	int day = identifier/ 24;
	int hourIndex = hour + 1 - currentChunkCount;
	

	if (hourIndex <= 0)
	{
		return std::make_pair(chunkSizeCounter, false);

	}
	int cell = schedule[hourIndex - 1][day][classIndex];
	int teacher = cell % 100;
	int preferredChunk = (cell - cell % 100) / 100;

	int prevTeacher = teacher;

	if (hourIndex > 0 || cell == -1) {
		{
			return std::make_pair(0, false);


			while (schedule[hourIndex][day][classIndex] != -1 && teacher == prevTeacher && hourIndex >= 0)
			{
				hourIndex--;
				chunkSizeCounter++;
				if (schedule[hourIndex][day][classIndex] != -1 && hourIndex >= 0)
				{
					prevTeacher = schedule[hourIndex][day][classIndex] % 100;
				}


			}

			bool isChunkSizePreferredChunk = (chunkSizeCounter == preferredChunk);
			return std::make_pair(chunkSizeCounter, isChunkSizePreferredChunk);;
		}

	}

}




bool ScheduleGenerator::isTeacherTeachingInThisHour(int teacher, int day,int hour)
{
	for (int classIndex = 0; classIndex < numOfClasses; classIndex++)
	{
		try
		{
			if (schedule[hour][day][classIndex] % 100 == teacher)
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

bool ScheduleGenerator::isBlockedTimeForClass(int classIndex, int hourIdentifier)
{
	auto it = std::find_if(classesBlockedTimes.begin(), classesBlockedTimes.end(),
		[hourIdentifier,classIndex](const std::pair<int, int>& p) {
			return p.second == hourIdentifier && p.first == classIndex;
		});

	return it != classesBlockedTimes.end(); // Returns true if found, false otherwise

}



void ScheduleGenerator::deleteHourFromTeacherInRequierments(int classIndex, int teacher, int chunkSize,bool checkChunkSize) {
	for (auto& pair : classes[classIndex]) {
		if (pair.first == teacher && pair.second > 0) {
			
			if (checkChunkSize)
			{
				if (pair.second == chunkSize)
				{
					pair.second = 0;
					break;
				}
			}
			else {
				pair.second = pair.second - 1;; // Reduce available hours for this teacher
				break;
			}
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

void ScheduleGenerator::initilizeTeachers()
{

	// *** This is the UI for inserting teachers. While developing, use the predefined data (in initilizeClasses func ). 
// For production, just comment out the predefined data and uncomment this section. ***

	/*
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
	*/
}

void ScheduleGenerator::initilizeClasses()
{
	// *** This is the UI for inserting teachers. While developing, use the predefined data (below). 
	// For production, just comment out the predefined data and uncomment this section. ***
	
	int classIndex = 0;
	/*
		print("\n\n\n************ started with classes settings *************\n");
		print("for every class, for every teacher the class will learn with,\n");
		print("you now want to provide the teacher number, how many hours in every \n ");
		print("schdual(gerenally week), the priority of the teacher (subject) \n");
		print("and the preferred chunks of hours (1-hour lessons,2-hours lessons,4-hours lessons etc)\n");
		print("schedule (generally week), the priority of the teacher (subject) \n");
		print("and the preferred chunks of hours (1-hour lessons, 2-hours lessons, 4-hours lessons, etc)\n");
		print("here are the teachers numbers and names: \n");
		print("_________________________________\n");
		printTeachersNames();
		print("_________________________________\n");
		// Predefined classes and teacher assignments
		// Class 0
		
		
		while (true) {

			print("do you want to add a class? y/n ");
			char answer = yesOrNoDialog();
			if (answer == 'n')
			{
				return;
			}
			else {

				while (true) {

					print("do you want to add a blocked times? y/n ");
					char answer = yesOrNoDialog();
					if (answer == 'n')
					{
						break;
					}
					else {
						int day;
						int hour;
						// no validate day/hour cause it will just not use if not in the schedule
						print("what day? ");
						std::cin >> day;
						print("what hour? ");
						std::cin >> hour;

						int identifier = day * 24 + hour;

						classesBlockedTimes.push_back(std::make_pair(classIndex, identifier));
					}

				
				classes.resize(classIndex + 1);
				std::string name = name;
				print("whats the name of the class? ");
				std::cin >> name;
				print("the identifier od the class is: ", classIndex, "\n");
				classesNames.push_back(name);
				while (true) {
					print("add a subject/teacher hours?");
					answer = yesOrNoDialog();
					if (answer == 'n')
					{
						break;
						classes.resize(classIndex + 1);
						classesNames.push_back("Class 0");

					}
					else {
						// todo: valiation
						int teacherIndex = 0;
						int hoursNum = 0;
						int priority = 0;
						int chunks = 0;
						print("teacher number? ");
						std::cin >> teacherIndex;
						print("how many hours? ");
						std::cin >> hoursNum;
						print("priority? low-1 regular-2 high-3 ");
						std::cin >> priority;
						print("preferred chunks? ");
						std::cin >> chunks;
						addTeacherHoursForClass(classIndex, teacherIndex, hoursNum, priority, chunks);
					}

				}
				classIndex++;
				this->numOfClasses = classIndex;

			}

		
		*/
			
	
			// Class 0

			//teachers[0][1] = "reason"; // teacher 0 unavailable in day 0 hour 1
			//classesBlockedTimes.push_back(std::make_pair(0, 0)); // class 0 unavailable in day 0 hour 0
		
			classes.resize(classIndex + 1);
			classesNames.push_back("Class 0");

			addTeacherHoursForClass(classIndex, 0, 3, 1, 3);
			addTeacherHoursForClass(classIndex, 5, 1, 1, 1); // Teacher, hours, priority, chunks size

			


			
	
			classIndex++;

			classes.resize(classIndex + 1);
			classesNames.push_back("Class 1");

			addTeacherHoursForClass(classIndex, 5, 8, 1, 2);
			addTeacherHoursForClass(classIndex, 4, 2, 3, 1);
			/*
			addTeacherHoursForClass(classIndex, 3, 3, 3, 1);
			addTeacherHoursForClass(classIndex, 3, 4, 3, 1);
			addTeacherHoursForClass(classIndex, 32, 4, 3, 2);
			addTeacherHoursForClass(classIndex, 8, 4, 3, 3);


			classes.resize(classIndex + 1);
			classesNames.push_back("Class 2");


			addTeacherHoursForClass(classIndex, 5, 1, 1, 1); // Teacher, hours, priority, chunks size
			addTeacherHoursForClass(classIndex, 4, 1, 3, 1);
			addTeacherHoursForClass(classIndex, 7, 3, 3, 2);
			addTeacherHoursForClass(classIndex, 12, 4, 3, 2);
			addTeacherHoursForClass(classIndex, 6, 4, 3, 3);



			classIndex++;


			*/
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

		if (hour.second != 0)
		{
			print("teacher: ", hour.first, " hours: ", hour.second, "\n");
			
		}

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

				print("day:", (blockedTime.first / 24, " hour:", blockedTime.first % 24, " reason:", blockedTime.second, "\n"));

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

void ScheduleGenerator::printSchedule() {
	const int columnWidth = 15;  

	std::cout << std::setw(columnWidth) << " ";  // Empty space for the hour labels
	for (int day = 0; day < daysInSchedual; day++) {
		std::cout << std::setw(columnWidth) << "Day " + std::to_string(day);
	}
	std::cout << std::endl;

	for (int hour = 0; hour < hoursPerDay; hour++) {

		std::cout << std::setw(columnWidth) << "Hour " + std::to_string(hour);

		for (int day = 0; day < daysInSchedual; day++) {
			std::string cellContent;
			bool hasClass = false;
			print("; ");
			for (int classIndex = 0; classIndex < numOfClasses; classIndex++) {
				int cell = schedule[hour][day][classIndex];
				if (cell != -1) {
					hasClass = true;
					int teacher = cell % 100;
					int preferredChunk = (cell - cell % 100) / 100;

					cellContent += " C" + std::to_string(classIndex) + " T" + std::to_string(teacher) + " P" + std::to_string(preferredChunk) + "|";
					
				}
			}

			if (!hasClass) {
				cellContent = "Free";  // If no class is scheduled
			}

		
			std::cout << std::setw(columnWidth) << cellContent;
		}
		std::cout << std::endl;  
	}
}


void ScheduleGenerator::printUnscheduledHoursDueTeachersBlockedTimes() {

	print("\n------------------------------------------------------------------------\n");

	bool unscheduledHoursExist = false;
	for (int teacher = 0; teacher < teachers.size(); ++teacher)
	{
		if (!teachers[teacher].empty()) {
			//print("identifier: ", i, " name: ", teachersNames[i]);
			//print(" blocked times: ");
			for (const auto& blockedTime : teachers[teacher])
			{
				for (int classIndex = 0; classIndex <= numOfClasses; classIndex++)
				{
					if (schedule[blockedTime.first % 24][blockedTime.first / 24][classIndex] == -1 && unscheduledTeacherAndClassHours(classIndex, teacher))
					{
						unscheduledHoursExist = true;
						print("\nday:", blockedTime.first / 24, " hour:", blockedTime.first % 24, " class: ", classIndex, " is unscheduled with teacher probably due to teacher:", teacher,
							" limitation: ", blockedTime.second);
					}
				}
				}
			}

		}
	print("\n------------------------------------------------------------------------\n");

	if (!unscheduledHoursExist)
	{
		print("All unscheduled hours are not related to any of the teachers' limitations.\n");
	}


}

void ScheduleGenerator::printUnscheduledHours() {

	print("\nThese are all the teacher-class hours that have not been scheduled:\n");

	for (int classIndex = 0; classIndex < numOfClasses; classIndex++)
	{
		printHoursRequierementsForClass(classIndex);

	}
}

bool ScheduleGenerator::unscheduledTeacherAndClassHours(int classIndex, int teacher) {

	for (const auto& classTeacher : classes[classIndex])
	{
		if (classTeacher.first == teacher)
		{
			return true;
		}

	}
	return false;
}

