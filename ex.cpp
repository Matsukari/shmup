#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

#include <memory>
//#include "lib/json.hpp"


//using json = nlohmann::json;

//std::shared_ptr<int> share()

class Head
{
public:
	Head() {}
	~Head() {}

	void Heavy() { weight = 12311; }

	int weight = 87;
	
};

void needclass(Head head)
{
	std::cout << head.weight << std::endl;
}


int main()
{
	needclass((Head{}.Heavy()));

	std::cout << R"(END)" << std::endl;


	return 0;
}