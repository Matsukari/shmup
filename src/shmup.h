#pragma once
#define shmup_h


#include "settings.h"
#include "visual_object.h"
#include "actor.h"
#include "bullet.h"
#include "gun.h"
#include "ship.h"
#include "animatedship.h"
#include "lean_animatedship.h"
#include "bg.h"
//#include "enemy_ship.h"
//#include "bg.h"
#include "../lib/include/resource_manager.h"
#include "../lib/json.hpp"

#include "graphics_manager.h"


namespace Shmup
{
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
		Rect screen;

		Timer timer;

		Manager::Graphics* graph;
	
	};



	ShmupGame::ShmupGame(Window* p_window, const Rect& p_rect) : 
		window(p_window),
		screen(p_rect)
	{
		logger("Initializing <Shmup>...");

		Settings::Init();

		graph = new Manager::Graphics(p_window, p_rect);

	}
	ShmupGame::~ShmupGame()
	{
		logger("Destructing <Shmup>...");
		window = nullptr;

		delete graph;
		graph = nullptr;

	}

	

	void ShmupGame::HandleEvents(const Event* p_event)
	{
		//player_ship->HandleEvents(p_event);
		graph->player->Set_Vel(FVec2{0.0f, 0.0f});
		
		const Uint8* state = SDL_GetKeyboardState(NULL);

		if 		(state[SDL_SCANCODE_A]) graph->player->Move_Left(Settings::config["speed"]["player_move"]);
		else if (state[SDL_SCANCODE_D]) graph->player->Move_Right(Settings::config["speed"]["player_move"]);
		if 		(state[SDL_SCANCODE_W]) graph->player->Move_Top(Settings::config["speed"]["player_move"]);
		else if (state[SDL_SCANCODE_S]) graph->player->Move_Bottom(Settings::config["speed"]["player_move"]);

		if (state[SDL_SCANCODE_J] && graph->player->Get_Gun()->Is_Recharged())
		{
			graph->player->Get_Gun()->Fire(FVec2{0, -fabs((float)Settings::config["speed"]["normal_bullet"])}, 
				Settings::config["damage"]["player"]);

			auto OnBulletCollide = [&](BulletList::iterator& p_bullet) -> void
			{
				Actor* who = (*p_bullet)->Get_WhoCollided();
				if (who)
				{
					logger("\n\nCollided, so, boom!\n\n");
					(*p_bullet)->Set_DeathReaction(graph->NewAnimReact_Explosion(&who->Get_Rect()) );
				}
				who = nullptr;
			};
			graph->player->Get_Gun()->Set_CallOnDeadBullet(OnBulletCollide);

		}
	
		
		if (Event::GetEvent().type == SDL_KEYUP)
		{
			if (Event::GetEvent().key.keysym.sym == SDLK_a || Event::GetEvent().key.keysym.sym == SDLK_d)
			{
				graph->player->DoneMoving();
			}
		}


	}
	void ShmupGame::Update(float p_dt)
	{
		/*if (timer.SinceLastPeek() >= 500 && !enemies.empty() && enemies[0]->Get_Gun()->Is_Recharged())
		{
			timer.Peek();
			toplayeranims.push_back(std::shared_ptr<Animation>{nullptr});
			enemies[0]->Get_Gun()->Fire(FVec2{0, fabs((float)Settings::config["speed"]["slow_bullet"])}, 
				Settings::config["damage"]["enemy_weak"], newreact(toplayeranims.back(), &player->Get_Rect()));
		}*/
		/*bg->Update(p_dt);
		player_ship->Update(p_dt);
		for(auto& enemy : enemy_ships)
		{
			enemy->Update(p_dt);
		}*/

		graph->Update(p_dt);


	}
	void ShmupGame::Render()
	{
	
		graph->Render();
	}
}

