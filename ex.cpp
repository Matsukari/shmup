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


class OBJ
{
public:
	OBJ(int px, int ph) : x(px), h(ph) {}
	~OBJ() {}

	int x;
	int h;
	
};
class Actor : public OBJ
{
public:
	Actor(const OBJ& obj, int pp) : OBJ(obj), SetP(pp) { std::cout << "Actor initialized" << std::endl; }
	~Actor() { std::cout << "Actor destructed" << std::endl; }

	void SetP(int pp) { p = pp; }
	
	int p;
};
int main()
{
	const Actor actor(OBJ{12 , 99});

	std::cout << actor.p << std::endl;

	std::cout << std::endl << R"(END)" << std::endl;


	return 0;
}