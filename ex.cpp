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


class Energy
{
public:
	virtual ~Energy() = 0;

	virtual void Accum() = 0;
	
};
	Energy::~Energy() { std::cout << "~Energy\n";}


class Move
{
public:
	virtual ~Move();

	
};
	Move::~Move() { std::cout << "~Move\n";}


class Atom : public Energy, public Move
{
public:
	Atom() {}
	virtual ~Atom() override { std::cout << "~Atom\n";}
	
	virtual void Accum() override = 0; //{ std::cout << "ATOM ACCUM\n";}
};

class Mol : public Atom
{
public:
	Mol() {}
	virtual ~Mol() override {}
	
	virtual void Accum() override { std::cout << "MOl ACCUM\n";}
};

class Bacteria : public Mol
{
public:
	Bacteria() {}
	virtual ~Bacteria() override {}
	
};
int main()
{
	Bacteria bacteria;
	Energy &energy = bacteria;
	energy.Accum();

	std::cout << std::endl << R"(END)" << std::endl;


	return 0;
}