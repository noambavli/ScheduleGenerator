#pragma once
#include "pch.h"

template<typename T> void print(const T& toPrint) {

	std::cout << toPrint;
}

template<typename T, typename ...Args> void print(const T& first,const  Args&... rest)
{
	std::cout << first ;
	print(rest...);

}


char yesOrNoDialog() {
	char answer;
	std::cin >> answer;
	bool keepGoing = false;
	while (true)
	{
		if (answer != 'n' && answer != 'y') {
			print("invalid answer, try again ");
			std::cin >> answer;
		}
		else {
			return answer;
		}
	}
}