#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

#include <memory>
//#include "lib/json.hpp"


//using json = nlohmann::json;

//std::shared_ptr<int> share()



#include <iostream>
#include <string>
#include <fstream>



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
Logger logger("logger.txt");


int main()
{
	int i = 12;
	logger("<Texture><", i, "> init", " ok");
	logger("<Texture><", i, "> destruct");


	std::cout << R"(END)" << std::endl;


	return 0;
}