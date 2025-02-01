#include "Common.h"

String Common::getTimestamp()
{
	auto now = std::chrono::system_clock::now();
    std::time_t now_c;
    char buffer[26];
    now_c = std::chrono::system_clock::to_time_t(now);
    ctime_s(buffer, sizeof(buffer), &now_c);
	// Format: Day Mon dd hh:mm:ss yyyy (no newline)
	buffer[24] = '\0';
    return std::string(buffer);
}

void Common::printTimestamp()
{
	std::cout << Common::getTimestamp() << std::endl;
}