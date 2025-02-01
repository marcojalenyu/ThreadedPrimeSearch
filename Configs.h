#pragma once
#include "Common.h"

enum ThreadPrintVariation {
	IMMEDIATE, // Print immediately after each thread finishes
	WAIT_ALL // Wait for all threads to finish before printing
};

enum ThreadTaskDivision {
	RANGE, // Straight division of search range (e.g. 1-100, 101-200, 201-300)
	LINEAR // Each thread is for divisibility check of a single number
};

class Configs
{
public:
	static Configs* getInstance();
	static void initialize();
	static void destroy();
	static void configure();
	static void setDefaultValues();
	static void setConfig(String config, String value);

	static ThreadPrintVariation getThreadPrintVariation() { return sharedInstance->threadPrintVariation; }
	static ThreadTaskDivision getThreadTaskDivision() { return sharedInstance->threadTaskDivision; }
	static unsigned int getPrimeSearchLimit() { return sharedInstance->primeSearchLimit; }
	static unsigned int getNumThreads() { return sharedInstance->numThreads; }

private:
	Configs() = default;
	Configs& operator=(Configs const&) {}; // To prevent copying of Configs

	inline static Configs* sharedInstance; // Make sharedInstance static
	ThreadPrintVariation threadPrintVariation;
	ThreadTaskDivision threadTaskDivision;
	unsigned int numThreads; // Number of threads to use
	unsigned int primeSearchLimit; // Upper limit of prime search

	friend class PrimeSearch;
};