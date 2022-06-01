#pragma once
#define shmup_h

#include "entity.h"
#include "actor.h"
#include "bullet.h"
#include "gun.h"
#include "effect.h"
#include "ship.h"
#include "shipex.h"
#include "../lib/include/resource_manager.h"
#include "../lib/json.hpp"




namespace Shmup
{
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
	template<class RectArrayT, class RectT, class ValT>
	RectArrayT json_2dArrayToRectArray(const std::vector<std::vector<ValT>>& array2d)
	{
		RectArrayT array;
		for (auto rect : array2d)
		{
			array.push_back(json_ArrayToRect<RectT, ValT>(rect)); // 1d vectors
		}


		return array; 

	}	
	std::string assetpath = "assets/";
	std::string imgpath = assetpath + "images/";
	std::string fontpath = assetpath + "fonts/";


	using ShipArray = std::vector<Ship*>;

	class ShmupGame
	{
	public:
		ShmupGame(SDL_Renderer* p_renderer, const FRect& p_rect);
		~ShmupGame();

		void HandleEvents(const SDL_Event& p_event);
		void Update(float p_dt);
		void Render();

	private:
		ResourceManager<Texture> txtmgr;
		SDL_Renderer* renderer;
		ActorArray ships;
		FRect rect;
		FRect spawnstart;

		Actor* bg;
		Actor* bg_buffer;
		Actor* clouds;


		nlohmann::json imgcrop;

	};


	ShmupGame::ShmupGame(SDL_Renderer* p_renderer, const FRect& p_rect) : 
		renderer(p_renderer),
		rect(p_rect)
	{
		logger("Initializing <Shmup>...");


		logger("Parsing img crop");
		std::ifstream cropfile(imgpath + "crop.json");
		cropfile >> imgcrop;
		cropfile.close();


		spawnstart = FRect{
			100,
			100,
			16 * 2,
			24 * 2
		};

		
		logger("Creating player properties");

		EntityProp playerprop
		{
			.texture = txtmgr.Construct("red_ship", renderer, imgpath + std::string{imgcrop["ships"]["red_ship"]["filename"]}),
			.rect = spawnstart,
			.frames = {}, // void
			.fspeed = imgcrop["ships"]["red_ship"]["fspeed"]
		};
		EntityProp bulletlong
		{
			.texture = txtmgr.Construct("laser_long", renderer, imgpath + std::string{imgcrop["effects"]["laser_long"]["filename"]}),
			.rect = {0,0, 10, 26},
			.frames = json_2dArrayToRectArray<RectArray, Rect, int>(imgcrop["effects"]["laser_long"]["frames"]),
			.fspeed = imgcrop["effects"]["laser_long"]["fspeed"]
		};
		FrameMap player_framemap;
		player_framemap["idle"] = json_2dArrayToRectArray<RectArray, Rect, int>(imgcrop["ships"]["red_ship"]["frames"]["idle"]);
		player_framemap["move_left"] = json_2dArrayToRectArray<RectArray, Rect, int>(imgcrop["ships"]["red_ship"]["frames"]["move_left"]);
		player_framemap["move_right"] = json_2dArrayToRectArray<RectArray, Rect, int>(imgcrop["ships"]["red_ship"]["frames"]["move_right"]);

		Gun* player_gun = new Gun{bulletlong};
		ships.push_back(new ShipEx{playerprop, player_gun, player_framemap});
		player_gun->SetSpawnPoint(&ships.front()->GetRect());
		player_gun->SetTargets(nullptr);

		bg = new Actor{
			EntityProp{
				.texture = txtmgr.Construct("desert", renderer, imgpath + std::string{imgcrop["bg"]["desert"]["filename"]}),
				.rect = rect,
				.frames = RectArray{rect},
				.fspeed = 0
			}
		};
		bg_buffer = new Actor{
			EntityProp{
				.texture = txtmgr.Get("desert"),
				.rect = {bg->rect.x, bg->rect.y - bg->rect.h, bg->rect.w, bg->rect.h},
				.frames = RectArray{bg->rect},
				.fspeed = 0
			}
		};
		bg->SetVel(Vecf2{0, 100});
		bg_buffer->SetVel(Vecf2{0, 100});
		//clouds_texture = new Actor{
		//	txtmgr.Construct("clouds", renderer, imgpath + std::string{imgcrop["bg"]["clouds_trans"]["filename"]})
		//};


	}
	ShmupGame::~ShmupGame()
	{
		logger("Destructing <Shmup>...");
		renderer = nullptr;

		for(auto& i : ships)
		{
			delete i;
			i = nullptr;
		}
		delete bg;
		bg = nullptr;
		delete bg_buffer;
		bg_buffer = nullptr;

		//clouds = nullptr;

	}

	void ShmupGame::HandleEvents(const SDL_Event& p_event)
	{
		ships.front()->HandleEvents(p_event); // first index: player
	}
	void ShmupGame::Update(float p_dt)
	{
		bg->Update(p_dt);
		bg_buffer->Update(p_dt);
		for(auto& ship : ships)
		{
			ship->Update(p_dt);
		}
	}
	void ShmupGame::Render()
	{
		bg->Render();
		bg_buffer->Render();
		for(auto ship : ships)
		{
			ship->Render();
		}
	}
}

