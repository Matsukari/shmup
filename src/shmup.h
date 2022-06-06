#pragma once
#define shmup_h


#include "settings.h"
#include "visual_object.h"
#include "actor.h"
#include "bullet.h"
#include "gun.h"
//#include "effect.h"
#include "ship.h"
#include "animatedship.h"
#include "lean_animatedship.h"
//#include "enemy_ship.h"
//#include "bg.h"
#include "../lib/include/resource_manager.h"
#include "../lib/json.hpp"




namespace Shmup
{

	//using ShipArray = std::vector<Ship*>;

	class ShmupGame
	{
	public:
		ShmupGame(Window* p_window, const Rect& p_rect);
		~ShmupGame();

		void HandleEvents(const Event* p_event);
		void Update(float p_dt);
		void Render();


	private:
		// manager
		ResourceManager<Texture> texturemgr;
		Window* window;
		Gun* gun;

		FRect owner;

		Rect screen;
		RectArray frames;

		Timer timer;

		// actors
		/*Actor* player_ship;
		ActorArray enemy_ships;
		ActorArray enemy_targets; // the target of the enemy
		BG* bg;*/


	};


	ShmupGame::ShmupGame(Window* p_window, const Rect& p_rect) : 
		window(p_window),
		screen(p_rect)
	{
		logger("Initializing <Shmup>...");
		Settings::Init();

		logger("Converting json vec2 to rect2...");
		frames = jsonex::Arr2ToRect2(Settings::image_crop["player_bullet"]["frames"]);

		logger("Creating bullet...");
		owner = FRect(100, 100, 5*3, 16*3);
		gun = new Gun(VisualObject(
			&screen, 
			texturemgr.Construct("1", window, Settings::IMAGE_PATH + str(Settings::config["use_texture"]["player_bullet"])),
			FRect(0, 0, 5*3, 16*3)),
			5,
			&owner,
			nullptr,
			&frames,
			Settings::image_crop["player_bullet"]["fspeed"]
		);
			gun->Get_Texture()->Set_Flip(SDL_FLIP_VERTICAL);

		timer.Peek();


	}
	ShmupGame::~ShmupGame()
	{
		logger("Destructing <Shmup>...");
		window = nullptr;
		delete gun;

		/*delete player_ship;
		player_ship = nullptr;
		for(auto& enemy : enemy_ships)
		{
			delete enemy;
			enemy = nullptr;
		}
		delete bg;
		bg = nullptr;*/

		//clouds = nullptr;

	}

	

	void ShmupGame::HandleEvents(const Event* p_event)
	{
		//player_ship->HandleEvents(p_event);

		if (timer.SinceLastPeek() >= 500)
		{
			timer.Peek();
			gun->Fire(FVec2{0, 100}, 10);
		}
	}
	void ShmupGame::Update(float p_dt)
	{
		/*bg->Update(p_dt);
		player_ship->Update(p_dt);
		for(auto& enemy : enemy_ships)
		{
			enemy->Update(p_dt);
		}*/
		gun->Update(p_dt);
	}
	void ShmupGame::Render()
	{
		/*bg->Render();
		player_ship->Render();
		for(auto enemy : enemy_ships)
		{
			enemy->Render();
		}*/

		gun->Render();
	}
}

