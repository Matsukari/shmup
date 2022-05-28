#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

#include <memory>
//#include "lib/json.hpp"


//using json = nlohmann::json;

//std::shared_ptr<int> share()

class Texture
{
public:
	Texture(int i) { val = 784; }
	~Texture() {}

	int val;
	
};

template<class... Args>
std::shared_ptr<Texture> share(Args... args)
{
	std::shared_ptr<Texture> texture = (std::make_shared<Texture>(args), ...);
	return texture;
}

int main()
{
	std::shared_ptr<Texture> texture = share(12);
	std::cout << texture->val << std::endl;
	
	std::cout << R"(END)" << std::endl;


	return 0;
}