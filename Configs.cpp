#include "Configs.h"

Configs* Configs::getInstance()
{
	Configs::initialize();
	return sharedInstance;
}

void Configs::initialize()
{
	if (sharedInstance == nullptr)
	{
		sharedInstance = new Configs();
		sharedInstance->configure();
	}
}

void Configs::destroy()
{
	if (sharedInstance != nullptr)
	{
		delete sharedInstance;
		sharedInstance = nullptr;
	}
}

// Get values from config.txt and set them
void Configs::configure()
{
	// Only change values if they are valid
	setDefaultValues();
	std::ifstream file("config.txt");
	if (file.is_open())
	{
		String line;
		while (std::getline(file, line))
		{
			// Each line is [config] [value]
			String config = line.substr(0, line.find(' '));
			String value = line.substr(line.find(' ') + 1);
			setConfig(config, value);
		}
	}
	else
	{
		std::cout << "Error opening file. Will set default." << std::endl;
	}
}

void Configs::setDefaultValues()
{
	sharedInstance->threadPrintVariation = IMMEDIATE;
	sharedInstance->threadTaskDivision = RANGE;
	sharedInstance->numThreads = 4;
	sharedInstance->primeSearchLimit = 100;
}

// Set from default to value specified (if valid)
void Configs::setConfig(String config, String value)
{
	if (config == "threadPrintVariation")
	{
		if (value == "IMMEDIATE")
		{
			sharedInstance->threadPrintVariation = IMMEDIATE;
		}
		else if (value == "WAIT_ALL")
		{
			sharedInstance->threadPrintVariation = WAIT_ALL;
		}
		else
		{
			std::cout << "Invalid value for threadPrintVariation. Defaulting to IMMEDIATE." << std::endl;
		}
	}
	else if (config == "threadTaskDivision")
	{
		if (value == "RANGE")
		{
			sharedInstance->threadTaskDivision = RANGE;
		}
		else if (value == "LINEAR")
		{
			sharedInstance->threadTaskDivision = LINEAR;
		}
		else
		{
			std::cout << "Invalid value for threadTaskDivision. Defaulting to RANGE." << std::endl;
		}
	}
	else if (config == "x")
	{
		try
		{
			int x = std::stoi(value);
			if (x > 0)
			{
				sharedInstance->numThreads = x;
			}
			else
			{
				std::cout << "Invalid value for x. Defaulting to 4." << std::endl;
			}
		}
		catch (std::invalid_argument)
		{
			std::cout << "Invalid value for x. Defaulting to 4." << std::endl;
		}
		catch (std::out_of_range)
		{
			std::cout << "x is out of range. Defaulting to 4." << std::endl;
		}
	}
	else if (config == "y")
	{
		try
		{
			int y = std::stoi(value);
			if (y > 0)
			{
				sharedInstance->primeSearchLimit = y;
			}
			else
			{
				std::cout << "Invalid value for y. Defaulting to 100." << std::endl;
			}
		}
		catch (std::invalid_argument)
		{
			std::cout << "Invalid value for y. Defaulting to 100." << std::endl;
		}
		catch (std::out_of_range)
		{
			std::cout << "y is out of range. Defaulting to 100." << std::endl;
		}
	}
	else
	{
		std::cout << "Invalid config." << std::endl;
	}
}