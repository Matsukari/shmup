#pragma once
#define logger_h

#include <iostream>
#include <string>
#include <fstream>


#ifndef LOGGER_FILENAME
 #define LOGGER_FILENAME "logger.txt"
#endif


class Logger
{
public:
	Logger(std::string filen)
	{
		data.open(filen.c_str());
	}
	~Logger()
	{
		data.close();
	}

	// functionate
	template<typename T>
	void print(T arg)
	{
		if (data.is_open())
		{
			data << arg;
		}
	}
	template<typename T, typename... Ts>
	void operator()(T arg, Ts... args)
	{
		print(arg);
		(print(args), ...);
		print('\n');
	}

	
private:
	std::ofstream data;
	
};
extern Logger logger;