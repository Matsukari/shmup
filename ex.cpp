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

int x = 991;
class Deduct
{
public:
	Deduct(){}
	~Deduct() {}


	int& v = x;
	
};

int main()
{
	Deduct d;
	x = 8;
	std::cout << d.v;

	//std::cout << group[0] << std::endl;
	std::cout << std::endl << R"(END)" << std::endl;


	return 0;
}