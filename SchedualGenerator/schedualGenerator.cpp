#include "pch.h"
#include "schedualGenerator.h"
#include "common.h"

void SchedualGenerator::initilizeDeatails() {

	print("WELCOME!\n");
	//todo:check input 
	print("how many days in the schedual? ");
	std::cin >> this->days;
	print("how many hours per day? ");
	std::cin >> this->hoursPerDay;
	initilizeGeneralBlockedHours();
	initilizeTeachers();
	initilizeClasses();

}

//todo: divide into initilize all teachers and to initilize teacher 
void SchedualGenerator::initilizeTeachers()
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
			print("this teacher has the identifier ", indexOfTeacher ,"\n");
			indexOfTeacher = indexOfTeacher+1;
			

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

void SchedualGenerator::initilizeClasses() {
	print("\n\n\n************ started with classes settings *************\n");
	print("for every class, for every teacher the class will learn with,\n");
	print("you now want to provide the teacher number, how many hours in every \n ");
	print("schdual(gerenally week), the priority of the teacher (subject) \n");
	print("and the preferred chunks of hours (1-hour lessons,2-hours lessons,4-hours lessons etc)\n");
	print("here are the teachers numbers and names: \n");
	print("_________________________________\n");
	printTeachersNames();
	print("_________________________________\n");
	int classesIndex = 0;
	while (true) {
		
		print("do you want to add a class? y/n ");
		char answer = yesOrNoDialog();
		if (answer == 'n')
		{
			return;
		}
		else {
			classes.resize(classesIndex + 1);
			std::string name = name;
			print("whats the name of the class? ");
			std::cin >> name;
			print("the identifier od the class is: ", classesIndex, "\n");
			classesNames.push_back(name);

			while (true) {
				print("add a subject/teacher hours?");
				answer = yesOrNoDialog();
				if (answer == 'n')
				{
					break;

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
					addTeacherHoursForClass(classesIndex, teacherIndex, hoursNum, priority, chunks);
				}
				
			}
			classesIndex++;
			this->numOfClasses = classesIndex;
		
		}	

	}
	



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
	else{

		
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


void SchedualGenerator::printHoursRequierementsForClass(int classIndex){

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
			if (answer == 'n'){
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
		print( i, ".", teachersNames[i], "\n");
	}

}