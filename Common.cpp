#include "Common.h"

String Common::getTimestamp()
{
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    std::tm timeInfo;
	localtime_s(&timeInfo, &now_c); // Thread-safe version of localtime

    std::ostringstream oss;
	// Format: YYYY MM DD HH:MM:SS.mmm
	oss << std::put_time(&timeInfo, "%Y %m %d %H:%M:%S") 
        << '.' 
        << std::setfill('0') 
        << std::setw(3) 
        << ms.count();

    return oss.str();
}

void Common::printTimestamp()
{
	std::cout << Common::getTimestamp() << std::endl;
}