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


class Pen
{
public:
	Pen()
	{
		std::cout << "Pen\n";
	}
	~Pen()
	{
		std::cout << "~Pen\n";
	}
	
};
int main()
{
	vector<Pen*> pens;
	pens.push_back(new Pen{});
	pens.push_back(new Pen{});
	pens.push_back(new Pen{});

	delete *pens.begin();
	pens.erase(pens.begin());


	//std::cout << group[0] << std::endl;
	std::cout << std::endl << R"(END)" << std::endl;


	return 0;
}