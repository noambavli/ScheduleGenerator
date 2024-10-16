#pragma once
#include "pch.h"

class ScheduleGenerator {

private:
	int daysInSchedual;
	int hoursPerDay;
	int numOfTeachers;
	int numOfClasses;
private:

	int*** schedule;
	
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
	std::vector <std::pair<int, int>> classesBlockedTimes;

	
private:
	// int unique identifier of day and hour (day*24+hour) and reason 
	std::unordered_map<int, std::string> generalBlockedHoursForEveryone;
	//todo: func for whole days tho you basically can just block all the hours / input less days 
	// and move restricstions but still

public:
	ScheduleGenerator(){}
	void initilizeDeatails();
	void generateSchedule();
	void improveSchedule();
public:
	void printAllteachers();
	void printTeachersNames();
	void printHoursRequierementsForClass(int classIndex);
	void printHoursRequirementsForAllClasses();
	void printSchedule();
	void printUnscheduledHoursDueTeachersBlockedTimes();
	void printUnscheduledHours();

private:
	void initilizeGeneralBlockedHours();
	void initilizeTeachers();
	void initilizeClasses();
	bool scheduleTeacherClassHours(int day, int hour, int classIndex, int teacher, int preferredChunk , int chunkAndTeacher);
	void addTeacherHoursForClass(int classesIndex, int teacherIndex, int hoursNum, int priority, int chunks);
	int nextTeacherRequierment(int classIndex, int triedTeachers);
	void deleteHourFromTeacherInRequierments(int classIndex, int teacher, int chunkSize, bool checkChunkSize);
	bool isTeacherAvailable(int teacher, int hourIdentifier);
	bool isHourBlockedForAll(int hourIdentifier);
	bool isBlockedTimeForClass(int classIndex, int hourIdentifier);
	bool isTeacherTeachingInThisHour(int teacher, int day, int hour);
	bool unscheduledTeacherAndClassHours(int classIndex, int teacher);
	int preferredChunkOfTeacherHoursForClass(int teacher, int classIndex);


private:
	bool fixNotOptimizedChunk(int classIndex, int identifier, int currentChunkCount, int preferredChunk, int teacher,
		int nextHourTeacher, int prevHourTeacher);
	bool replaceTeachers(int classIndex, int teacher, int nextOrPrevTeacher, bool direction, int currentChunkIdentifier,
		int currentChunkCount, int completingChunkIdentifier, int completingChunkSize);
	bool findCompletingChunkAndReplace(int classIndex, int teacher, int surroundingTeacher, bool isNextDirection,
		int currentChunkIdentifier, int completingChunkSize);
	std::pair<int, bool> nextChunkInfo(int classIndex, int identifier);
	std::pair<int, bool> prevChunkInfo(int classIndex, int identifier, int currentChunkCount);





};