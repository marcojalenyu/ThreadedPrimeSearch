#include "Common.h"
#include "Configs.h"

int main()
{
	Common::printTimestamp();
	Configs::initialize();
	std::cin.get();
	Configs::destroy();
	return 0;
}