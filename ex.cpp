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

struct Body
{
	int size=  7712;
	int str = 123;
};


class Pen
{
public:
	Pen(const Body& body) : bod(body)
	{
		std::cout << "Pen\n";
	}
	~Pen()
	{
		std::cout << "~Pen\n";
	}
	

	Body bod;
};


int main()
{
	Pen* pen = nullptr;
	{
		Body body{};
		body.size = 6700;
		pen = new Pen(body);
		std::cout << "Before " << pen->bod.size << std::endl;

	}
	std::cout << "after " << pen->bod.size << std::endl;
	delete pen;


	//std::cout << group[0] << std::endl;
	std::cout << std::endl << R"(END)" << std::endl;


	return 0;
}