#include "Common.h"
#include "Configs.h"
#include "PrimeSearch.h"

int main()
{
	Configs* configs = Configs::getInstance();
	PrimeSearch primeSearch(configs->getNumThreads(), configs->getPrimeSearchLimit());
	primeSearch.start();
	std::cout << "Prime Search Completed. Enter to exit: ";
	std::cin.get();
	Configs::destroy();
	return 0;
}