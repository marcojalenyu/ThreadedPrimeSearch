#pragma once
#include "Common.h"
#include "Configs.h"
#include <thread>
#include <vector>
#include <cmath>
#include <mutex>
#include <map>

class PrimeSearch
{
public:
	PrimeSearch(unsigned int numThreads, unsigned int primeSearchLimit);

	void start();
	void startRangeSearch();
	void rangeSearch(unsigned threadID, unsigned int start, unsigned int end); // Straight division of search range (e.g. 1-100, 101-200, 201-300)
	void startLinearSearch();
	void linearSearch(unsigned int num, std::vector<bool>& isPrime, std::atomic<unsigned int>& lastThreadUsed); // Each thread is for divisibility check of a single number
	bool isPrimeInRange(unsigned int num, unsigned int start, unsigned int end);
	void printPrimeNumbers();

private:
	unsigned int numThreads;
	unsigned int primeSearchLimit;
	std::mutex mutexLock;
	// Key: prime number, Value: thread ID of last thread that found the prime & timestamp
	std::map<unsigned int, std::pair<unsigned int, String>> primeNumbers;
};

