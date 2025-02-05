#include "PrimeSearch.h"

PrimeSearch::PrimeSearch(unsigned int numThreads, unsigned int primeSearchLimit)
{
	this->numThreads = numThreads;
	this->primeSearchLimit = primeSearchLimit;
}

void PrimeSearch::start()
{
	ThreadPrintVariation threadPrintVariation = Configs::getThreadPrintVariation();
	ThreadTaskDivision threadTaskDivision = Configs::getThreadTaskDivision();
	Common::printTimestamp();

	// Different task division schemes
	if (threadTaskDivision == RANGE)
	{
		startRangeSearch();
	}
	else if (threadTaskDivision == LINEAR)
		startLinearSearch();

	if (threadPrintVariation == WAIT_ALL)
	{
		this->printPrimeNumbers();
	}

	Common::printTimestamp();
}

// Check if num is prime from divisor range [start, end]
bool PrimeSearch::isPrimeInRange(unsigned int num, unsigned int start, unsigned int end)
{
	// 4 is the smallest composite number
	if (num <= 3) return true;
	for (unsigned int divisor = start; divisor <= end; divisor++)
	{
		if (num % divisor == 0) return false;
	}
	return true;
}

void PrimeSearch::startRangeSearch()
{
	// To create an equal range for each thread
	std::vector<std::thread> threads;
	unsigned int range = this->primeSearchLimit / this->numThreads;
	for (unsigned int threadID = 0; threadID < this->numThreads; threadID++)
	{
		unsigned int start = threadID * range + 1;
		unsigned int end = (threadID + 1) * range;

		// To include the remaining numbers (if any) to the last thread
		if (threadID == this->numThreads - 1) end = this->primeSearchLimit;

		// To only make a thread if range is less than primeSearchLimit
		if (start <= this->primeSearchLimit) 
		{
			threads.push_back(std::thread(&PrimeSearch::rangeSearch, this, threadID, start, end));
		}
	}

	for (std::thread& thread : threads)
	{
		thread.join();
	}
}

// Thread function to search for prime numbers in range [start, end]
void PrimeSearch::rangeSearch(unsigned threadID, unsigned int start, unsigned int end)
{
	for (unsigned int num = start; num <= end; num++)
	{
		// Check only up to square root of num for optimization
		if (isPrimeInRange(num, 2, sqrt(num)))
		{
			if (Configs::getThreadPrintVariation() == IMMEDIATE)
			{
				std::lock_guard<std::mutex> guard(mutexLock);
				std::cout << Common::getTimestamp() << " - Thread " << threadID << " found prime : " << num << std::endl;
			}
			else if (Configs::getThreadPrintVariation() == WAIT_ALL)
			{
				primeNumbers[num] = threadID;
			}
		}
	}
}

void PrimeSearch::startLinearSearch()
{
	for (unsigned int num = 1; num <= this->primeSearchLimit; num++)
	{
		// Store results of divisibility check in a shared vector
		std::vector<bool> isPrime(this->numThreads, true);
		this->linearSearch(num, isPrime);

		if (std::find(isPrime.begin(), isPrime.end(), false) == isPrime.end())
		{
			if (Configs::getThreadPrintVariation() == IMMEDIATE)
			{
				std::cout << Common::getTimestamp() << " - Prime Number found : " << num << std::endl;
			}
			else if (Configs::getThreadPrintVariation() == WAIT_ALL)
			{
				primeNumbers[num] = 0;
			}
		}
	}
}

// Each thread checks divisibility of a single number
void PrimeSearch::linearSearch(unsigned int num, std::vector<bool>& isPrime)
{
	// To create an equal range of divisors for each thread
	std::vector<std::thread> threads;
	unsigned int sqrtNum = sqrt(num);
	unsigned int range = (sqrtNum + numThreads - 1) / this->numThreads;

	for (unsigned int threadID = 0; threadID < this->numThreads; threadID++)
	{
		unsigned int start = threadID * range + 2;
		unsigned int end = (threadID + 2) * range;
		// To include the remaining divisors (if any) to the last thread
		if (end > sqrtNum) end = sqrtNum;

		// Only make a thread if range is less than sqrtNum
		if (start <= sqrtNum) {
			threads.push_back(std::thread([this, threadID, num, start, end, &isPrime]()
				{
					isPrime[threadID] = isPrimeInRange(num, start, end);
				}
			));
		}
	}

	for (std::thread& thread : threads)
	{
		thread.join();
	}

	threads.clear();
}

void PrimeSearch::printPrimeNumbers()
{
	for (auto const& prime : primeNumbers)
	{
		if (Configs::getThreadTaskDivision() == LINEAR)
		{
			std::cout << "Prime Number " << prime.first << " found" << std::endl;
		}
		else if (Configs::getThreadTaskDivision() == RANGE)
		{
			std::cout << "Prime Number " << prime.first << " found by Thread " << prime.second << std::endl;
		}
	}
}