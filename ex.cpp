#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

#include <memory>
//#include "lib/json.hpp"


//using json = nlohmann::json;

//std::shared_ptr<int> share()


template<class T>
using vector = std::vector<T>;


class Actor
{
public:
	Actor() { std::cout << "Actor initialized" << std::endl; }
	~Actor() { std::cout << "Actor destructed" << std::endl; }

	int x = 2;
	
};
int main()
{
	vector<int> points(4);
	std::cout << "Before" << std::endl;
	for(auto i = points.begin(); i !=)
	{
		std::cout << i << " ";
		i = 82;
	}
	std::cout << std::endl << "After" << std::endl;
	for(auto i : points)
	{
		std::cout << i << " ";
	}


	std::cout << std::endl << R"(END)" << std::endl;


	return 0;
}