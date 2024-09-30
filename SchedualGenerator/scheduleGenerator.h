#pragma once
#include "pch.h"

class ScheduleGenerator {

private:
	int daysInSchedual;
	int hoursPerDay;
	int numOfTeachers;
	int numOfClasses;
private:

	int*** schedual;
	
private:
	//names for the classes&teachers cause we only reffer to them as index in our program for efficiency
	std::vector<std::string>classesNames;
	std::vector<std::string>teachersNames;
	//vector of classes , every class by index(!) in vector, has list of the learning pairs (teacher,class) it needs
	std::vector < std::list<std::pair<int, int>>> classes;
	/*map of int by index(!) int as key,the map of the blocked hours and reason
	 (unique identifier of day and hour,reason)
	 the unique identifier is day*24+hour */
	std::unordered_map<int,std::unordered_map<int, std::string>> teachers;
	std::vector < std::list<std::pair<int, int>>> classesPreferredChunks;

	
private:
	// int unique identifier of day and hour (day*24+hour) and reason 
	std::unordered_map<int, std::string> generalBlockedHoursForEveryone;
	//todo: func for whole days tho you basically can just block all the hours / input less days 
	// and move restricstions but still

public:
	ScheduleGenerator(){}
	void initilizeDeatails();
	void generateSchedule();
public:
	void printAllteachers();
	void printTeachersNames();
	void printHoursRequierementsForClass(int classIndex);
	void printHoursRequirementsForAllClasses();
	void printSchedule();
private:
	void initilizeGeneralBlockedHours();
	void initilizeTeachers();
	void initilizeClasses();
	void addTeacherHoursForClass(int classesIndex, int teacherIndex, int hoursNum, int priority, int chunks);
	int nextTeacherRequierment(int classIndex, int triedTeachers);
	void deleteHourFromTeacherInRequierments(int classIndex, int teacher);
	bool isTeacherAvailable(int teacher, int hourIdentifier);
	bool isHourBlockedForAll(int hourIdentifier);
	bool isTeacherTeachingInThisHour(int teacher, int day, int hour);
	int preferredChunkOfTeacherHoursForClass(int teacher, int classIndex);
private:
	void improveSchedule();
	bool fixNotOptimizedChunk(int classIndex, int identifier, int currentChunkCount, int preferredChunk, int teacher, int nextHourTeacher, int prevHourTeacher);
	bool replaceTeachers(int classIndex, int teacher, int nextOrPrevTeacher, std::string& direction, int currentChunkIdentifier, int currentChunkCount, int completingChunkIdentifier, int completingChunkSize);
	int sizeOfNextChunk(int classIndex, int identifier);
	int sizeOfPrevChunk(int classIndex, int identifier, int currentChunkCount);





};