#pragma once
#define reaction_h

#include <memory>

namespace Shmup
{
	class Reaction
	{
	public:
		virtual ~Reaction()=0;
		virtual void Activate()=0;
		
	};
	Reaction::~Reaction(){}
	using Reaction_ptr = std::unique_ptr<Reaction>;

}