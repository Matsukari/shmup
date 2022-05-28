#pragma once
#define resource_manager_h

#include <map>
#include <memory>
#include <string>
#include "logger.h"



template<class ResourceType>
class ResourceManager
{
public:
	ResourceManager()=default;
	~ResourceManager() 
	{
		logger("<ResourceManager> created ", res.size(), " shared objects before destroying!");
		for (auto i : res)
		{
			logger(i.first, " has ", UseCount(i.first), " alive shared objects!");
		}
	}

	using Res_ptr = std::shared_ptr<ResourceType>;
	using Res_map = std::map<std::string, Res_ptr>;
	template<class ID>
	Res_ptr ConstructVoid(ID p_id)
	{
		if (res.find(p_id) != res.end())
		{
			return Get(p_id);
		}
		Res_ptr resource = std::make_shared<ResourceType>();
		res[p_id] = resource;

		return resource;
	}
	template<class ID, class... Ts>
	Res_ptr Construct(ID p_id, Ts... args)
	{
		if (res.find(p_id) != res.end())
		{
			return Get(p_id);
		}
		Res_ptr resource = (std::make_shared<ResourceType>(args), ...); // fold expression
		res[p_id] = resource;

		return resource;
	}
	template<class ID, class FuncT, class... Ts>
	void Call(ID p_id, FuncT func, Ts... args)
	{
		// only if constructed
		if (res.find(p_id) != res.end())
		{
			(func(args), ...);
		}
	}

	Res_ptr Get(std::string p_id)
	{ 
		return res[p_id];
	}

	long UseCount(std::string p_id) const
	{
		return res.at(p_id).use_count();
	}
	const Res_map& GetResources() const noexcept { return res; }


protected:
	Res_map res;
	
};
