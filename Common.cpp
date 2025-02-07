#include "Common.h"

String Common::getTimestamp()
{
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()) % 1000000000;

    std::tm timeInfo;
	localtime_s(&timeInfo, &now_c); // Thread-safe version of localtime

    std::ostringstream oss;
	// Format: YYYY MM DD HH:MM:SS.mmmmmmmmm (nanoseconds)
	oss << std::put_time(&timeInfo, "%Y %m %d %H:%M:%S") 
        << '.' 
        << std::setfill('0') 
        << std::setw(9) 
        << ms.count();

    return oss.str();
}

void Common::printTimestamp()
{
	std::cout << Common::getTimestamp() << std::endl;
}