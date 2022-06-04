#pragma once
#define settings_h

#include "../lib/json.hpp"
#include "../lib/include/shapes.h"
#include "../lib/include/logger.h"
#include "../lib/include/texture.h"
#include <memory>
#include <vector>
#include <string>
#include <fstream>

// gittoken accessme = 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
template<class T>
inline std::string ts(const T& n) noexcept
{
	return std::move(std::to_string(n));
}
using str = std::string;

namespace Shmup
{
	using Vec2 = SDL_Point;
	using FVec2 = SDL_FPoint;
	using Texture_ptr = std::shared_ptr<Texture>;
	using RectArray = std::vector<Rect>;

	namespace jsonex
	{
		// 1d array
		Rect ArrToRect(const std::vector<int>& array) noexcept
		{
			if (array.size() != 4)
			{
				logger("In ArrayToRect, array does not qualify to convert!");
				return Rect();
			}
			return Rect{
				array.at(0),
				array.at(1),
				array.at(2),
				array.at(3)
			};
		}
		// 2d array
		RectArray Arr2ToRect2(const std::vector<std::vector<int>>& array2d)
		{
			RectArray array;
			for (auto rect : array2d)
			{
				array.push_back(ArrToRect(rect));
			}
			return array; 
		}	
	}
	
	namespace Settings
	{

		const std::string ASSET_PATH = "assets/";
		const std::string IMAGE_PATH = ASSET_PATH + "images/";
		const std::string FONT_PATH = ASSET_PATH + "fonts/";

		nlohmann::json image_crop;
		nlohmann::json config;

		void Init()
		{
			logger("Parsing img crop json...");
			
			std::ifstream mfile(IMAGE_PATH + "crop.json");
			mfile >> image_crop;
			mfile.close(); 
			
			logger("Pasring config json....");
			
			std::ifstream file("config.json");
			file >> config;;
			file.close();
			

		}


	}
}