#pragma once
#include "Common.h"
#include "Configs.h"
#include <thread>
#include <vector>

class PrimeSearch
{
public:
	PrimeSearch(unsigned int numThreads, unsigned int primeSearchLimit);

	void start();
	void rangeSearch(unsigned threadID, unsigned int start, unsigned int end); // Straight division of search range (e.g. 1-100, 101-200, 201-300)
	void linearSearch(); // Each thread is for divisibility check of a single number
	bool isPrime(unsigned int num);

private:
	unsigned int numThreads = 1;
	unsigned int primeSearchLimit = 100;
};

