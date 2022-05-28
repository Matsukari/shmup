#pragma once
#define shmup_h

#include "entity.h"
#include "actor.h"
#include "ship.h"
#include "bullet.h"
#include "effect.h"
#include "../lib/include/resource_manager.h"
#include "../lib/json.hpp"

namespace Shmup
{
	std::string assetfold = "assets/";
	std::string imgfold = assetfold + "images/";
	std::string fontfold = assetfold + "fonts/";


	using ShipArray = std::vector<Ship*>;
	class ShmupGame
	{
	public:
		ShmupGame(SDL_Renderer* p_renderer);
		~ShmupGame();

		void HandleEvents(const SDL_Event& p_event);
		void Update(float p_dt);
		void Render();

	private:
		ResourceManager<Texture> txtmgr;
		SDL_Renderer* renderer;
		ShipArray ships;

		nlohmann::json imgcrop;
	
		FRect spawnstart;
	};

	template<class RectT, class ValT>
	RectT json_ArrayToRect(const std::vector<ValT>& array)
	{
		return RectT{
			array.at(0),
			array.at(1),
			array.at(2),
			array.at(3)
		};
	}

	ShmupGame::ShmupGame(SDL_Renderer* p_renderer) : 
		renderer(p_renderer)
	{
		logger("Initializing <Shmup>...");

		logger("Parsing img crop");
		std::ifstream cropfile(imgfold + "crop.json");
		cropfile >> imgcrop;
		cropfile.close();

		spawnstart = FRect{
			100,
			100,
			16 * 3,
			24 * 3
		};


		logger("Converting json array to rects");
		RectArray idleframes;
		for(auto i : imgcrop["ships"]["red_ship"]["frames"]["idle"])
		{
			idleframes.push_back(json_ArrayToRect<Rect, int>(i));
		}

		logger("Creating player properties");

		EntityProp playerprop
		{
			.texture = txtmgr.ConstructVoid("red_ship"),
			.rect = spawnstart,
			.frames = idleframes,
			.fps = imgcrop["ships"]["red_ship"]["fps"]
		};

		//txtmgr.Call("red_ship", playerprop.texture->Load, renderer, imgcrop["ships"]["red_ship"]["filename"]);
	}
	ShmupGame::~ShmupGame()
	{
		logger("Destructing <Shmup>...");
		renderer = nullptr;

	}

	void ShmupGame::HandleEvents(const SDL_Event& p_event)
	{
	}
	void ShmupGame::Update(float p_dt)
	{
	}
	void ShmupGame::Render()
	{
	}
}

