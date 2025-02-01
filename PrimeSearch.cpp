#include "PrimeSearch.h"

PrimeSearch::PrimeSearch(unsigned int numThreads, unsigned int primeSearchLimit)
{
	this->numThreads = numThreads;
	this->primeSearchLimit = primeSearchLimit;
}

void PrimeSearch::start()
{
	Configs* configs = Configs::getInstance();
	ThreadTaskDivision threadTaskDivision = configs->getThreadTaskDivision();
	std::vector<std::thread> threads;
	Common::printTimestamp();

	if (threadTaskDivision == RANGE)
	{
		// To create an equal range for each thread
		unsigned int range = this->primeSearchLimit / this->numThreads;
		for (unsigned int threadID = 0; threadID < this->numThreads; threadID++)
		{
			unsigned int start = threadID * range + 1;
			unsigned int end = (threadID + 1) * range;
			threads.push_back(std::thread(&PrimeSearch::rangeSearch, this, threadID, start, end));
		}		
	}
	else if (threadTaskDivision == LINEAR)
	{
		for (unsigned int threadID = 0; threadID < this->numThreads; threadID++)
		{
			threads.push_back(std::thread(&PrimeSearch::linearSearch, this));
		}
	}

	for (std::thread& thread : threads)
	{
		thread.join();
	}
}

bool PrimeSearch::isPrime(unsigned int num)
{
	if (num < 2) return false;
	// Check only up to square root of num for optimization
	for (int i = 2; i * i <= num; i++)
	{
		if (num % i == 0) return false;
	}
	return true;
}

void PrimeSearch::rangeSearch(unsigned threadID, unsigned int start, unsigned int end)
{
	for (unsigned int num = start; num <= end; num++)
	{
		if (isPrime(num))
		{
			std::cout << Common::getTimestamp() << " - Thread " << threadID << " found prime : " << num << std::endl;
		}
	}
}

void PrimeSearch::linearSearch()
{

}