#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <list>
#include <iterator>

#include <memory>
//#include "lib/json.hpp"


//using json = nlohmann::json;

//std::shared_ptr<int> share()


template<class T>
using vector = std::vector<T>;
#include <cassert>

#include <functional>

#include <memory>
#include <ctime>
#include <sys/time.h>
#include <cmath>

void WaitTime(float seconds) noexcept
{
	clock_t start_time      = clock();
	float   seconds_to_wait = seconds * CLOCKS_PER_SEC;

	// Wait until current time passes seconds given;
	while (clock() < start_time + seconds_to_wait);
}

float Get_Random(float min, float max)
{
	timeval t;

	WaitTime(0.0001);

	gettimeofday(&t, NULL);
	srand(t.tv_usec * t.tv_sec);

	return min + static_cast<float>(rand()) / ( static_cast<float>(RAND_MAX/(max-min)));
}



int main()
{
	for (int i = 0; i < 10; ++i)
	{
		std::cout << Get_Random(0.0, 10.0) << std::endl;
	}



	//std::cout << group[0] << std::endl;
	std::cout << std::endl << R"(END)" << std::endl;


	return 0;
}