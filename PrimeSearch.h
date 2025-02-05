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
	void linearSearch(unsigned int num, std::vector<bool>& isPrime); // Each thread is for divisibility check of a single number
	bool isPrimeInRange(unsigned int num, unsigned int start, unsigned int end);
	void printPrimeNumbers();

private:
	unsigned int numThreads = 1;
	unsigned int primeSearchLimit = 100;
	std::mutex mutexLock;
	// Key: prime number, Value: thread ID that found the prime, for WAIT_ALL print
	std::map<unsigned int, unsigned int> primeNumbers;
};

