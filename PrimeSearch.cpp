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
		for (unsigned int num = 1; num < this->primeSearchLimit; num++)
		{
			// Store results of divisibility check in a shared vector
			std::vector<bool> isDivisible(this->numThreads, false);
			// To create an equal range of divisors for each thread
			unsigned int sqrtNum = sqrt(num);
			unsigned int range = sqrtNum / this->numThreads;
			for (unsigned int threadID = 0; threadID < this->numThreads; threadID++)
			{
				unsigned int start = threadID * range + 1;
				unsigned int end = (threadID + 1) * range;
				// To include the remaining divisors (if any) to the last thread
				if (end > sqrtNum) end = sqrtNum;
				
				
			}


		}
	}

	for (std::thread& thread : threads)
	{
		thread.join();
	}

	if (Configs::getThreadPrintVariation() == WAIT_ALL)
	{
		for (auto const& prime : primeNumbers)
		{
			std::cout << "Prime Number " << prime.first << " found by Thread " << prime.second << std::endl;
		}
	}

	Common::printTimestamp();
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

bool PrimeSearch::hasDivisorsInRange(unsigned int num, unsigned int start, unsigned int end)
{
	for (unsigned int i = start; i <= end; i++)
	{
		if (num % i == 0) return true;
	}
	return false;
}

void PrimeSearch::rangeSearch(unsigned threadID, unsigned int start, unsigned int end)
{
	for (unsigned int num = start; num <= end; num++)
	{
		if (isPrime(num))
		{
			if (Configs::getThreadPrintVariation() == IMMEDIATE)
			{
				std::lock_guard<std::mutex> guard(mutex_lock);
				std::cout << Common::getTimestamp() << " - Thread " << threadID << " found prime : " << num << std::endl;
			}
			else if (Configs::getThreadPrintVariation() == WAIT_ALL)
			{
				std::lock_guard<std::mutex> guard(mutex_lock);
				primeNumbers[num] = threadID;
			}
		}
	}
}

void PrimeSearch::linearSearch()
{

}