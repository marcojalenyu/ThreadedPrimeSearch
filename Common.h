#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

using String = std::string;

class Common
{
public:
	static String getTimestamp();
	static void printTimestamp();
};

#endif // COMMON_H