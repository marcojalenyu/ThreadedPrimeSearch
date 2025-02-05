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
		this->startRangeSearch();
	else if (threadTaskDivision == LINEAR)
		this->startLinearSearch();

	if (threadPrintVariation == WAIT_ALL)
		this->printPrimeNumbers();

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
	std::vector<std::thread> threads;
	// To ensure no extra threads are created (1 thread : 1+ numbers)
	unsigned int threadsUsed = this->primeSearchLimit < this->numThreads ? this->primeSearchLimit : this->numThreads;
	// To create an equal range for each thread
	unsigned int range = this->primeSearchLimit / threadsUsed;
	for (unsigned int threadID = 0; threadID < threadsUsed; threadID++)
	{
		unsigned int start = threadID * range + 1;
		unsigned int end = (threadID + 1) * range;
		// To include the remaining numbers (if any) to the last thread
		if (threadID == threadsUsed - 1) end = this->primeSearchLimit;
		threads.push_back(std::thread(&PrimeSearch::rangeSearch, this, threadID, start, end));
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
		if (isPrimeInRange(num, 2, (int) sqrt(num)))
		{
			if (Configs::getThreadPrintVariation() == IMMEDIATE)
			{
				std::lock_guard<std::mutex> guard(mutexLock);
				std::cout << Common::getTimestamp() << " - Thread " << threadID << " found prime : " << num << std::endl;
			}
			else if (Configs::getThreadPrintVariation() == WAIT_ALL)
			{
				primeNumbers[num] = std::make_pair(threadID, Common::getTimestamp());
			}
		}
	}
}

void PrimeSearch::startLinearSearch()
{
	for (unsigned int num = 1; num <= this->primeSearchLimit; num++)
	{
		std::atomic<unsigned int> lastThreadUsed(0);
		// Store results of divisibility check in a shared vector
		std::vector<bool> isPrime(this->numThreads, true);
		this->linearSearch(num, isPrime, lastThreadUsed);

		if (std::find(isPrime.begin(), isPrime.end(), false) == isPrime.end())
		{
			if (Configs::getThreadPrintVariation() == IMMEDIATE)
			{
				std::cout << Common::getTimestamp() << " - Thread " << lastThreadUsed << " found prime : " << num << std::endl;
			}
			else if (Configs::getThreadPrintVariation() == WAIT_ALL)
			{
				primeNumbers[num] = std::make_pair(lastThreadUsed.load(), Common::getTimestamp());
			}
		}
	}
}

// Each thread checks divisibility of a single number
void PrimeSearch::linearSearch(unsigned int num, std::vector<bool>& isPrime, std::atomic<unsigned int>& lastThreadUsed)
{
	std::vector<std::thread> threads;
	unsigned int sqrtNum = (int) sqrt(num);
	// To ensure no extra threads are created (1 thread : 1+ divisors)
	unsigned int threadsUsed = this->numThreads < sqrtNum ? this->numThreads : sqrtNum;
	// To create an equal range of divisors for each thread
	unsigned int range = (sqrtNum + threadsUsed - 1) / threadsUsed;

	for (unsigned int threadID = 0; threadID < threadsUsed; threadID++)
	{
		unsigned int start = threadID * range + 2;
		unsigned int end = (threadID + 2) * range;
		// To include the remaining divisors (if any) to the last thread
		if (end > sqrtNum) end = sqrtNum;

		// Only make a thread if range is less than sqrtNum
		if (start <= sqrtNum) {
			threads.push_back(std::thread([this, threadID, num, start, end, &isPrime, &lastThreadUsed]()
				{
					isPrime[threadID] = isPrimeInRange(num, start, end);
					lastThreadUsed = threadID;
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
		unsigned int primeNum = prime.first;
		unsigned int threadID = prime.second.first;
		String timestamp = prime.second.second;
		std::cout << timestamp << " - Thread " << threadID << " found prime : " << primeNum << std::endl;
	}
}