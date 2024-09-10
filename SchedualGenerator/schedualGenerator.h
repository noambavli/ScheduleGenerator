#pragma once
#include "pch.h"

class SchedualGenerator {

private:
	int days;
	int hoursPerDay;
	int numOfTeachers;
	int numOfClasses;
private:
	/*
	nested unordered map of : day, a map of hour - (map of(class - teacher))
	so schedualing goes like this schedule[day][hour][teacherNum] = classNum ; 
	*/
	std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, int>>> schedule;
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
	
private:
	// int unique identifier of day and hour (day*24+hour) and reason 
	std::unordered_map<int, std::string> generalBlockedHoursForEveryone;
	//todo: func for whole days tho you basically can just block all the hours / input less days 
	// and move restricstions but still

public:
	SchedualGenerator(){}
	void initilizeDeatails();
	void generateSchedual();
public:
	void printAllteachers();
	void printTeachersNames();
	void printHoursRequierementsForClass(int classIndex);
	void printHoursRequirementsForAllClasses();
private:
	void initilizeGeneralBlockedHours();
	void initilizeTeachers();
	void initilizeClasses();
	void addTeacherHoursForClass(int classesIndex, int teacherIndex, int hoursNum, int priority, int chunks);
	
	
	





};