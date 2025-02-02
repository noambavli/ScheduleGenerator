#pragma once
#include "pch.h"

template<typename T>
void print(const T& toPrint) {
    std::cout << toPrint;
}

template<typename T, typename... Args>
void print(const T& first, const Args&... rest) {
    std::cout << first;
    if constexpr (sizeof...(rest) > 0) {
        std::cout << " ";
        print(rest...);
    }
}

char yesOrNoDialog() {
    char answer;
    while (true) {
        std::cin >> answer;
        if (answer == 'y' || answer == 'n') {
            return answer;
        }
        std::cout << "Invalid answer, try again: ";
    }
}
