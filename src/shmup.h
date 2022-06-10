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
		RectArray frames2;
		Timer timer;

		LeanAnimatedShip* player;
		FrameMap framemap;
		FrameMap enemyframe;

		std::vector<std::shared_ptr<Animation>> anims;
		std::vector<std::shared_ptr<Animation>> toplayeranims;

		Gun* enemygun;
		std::vector<AnimatedShip*> enemies; // allocated
		ActorArray player_targets;
		ActorArray enemy_targets; // unallocated pointers


		std::shared_ptr<Animation> explosion;
		std::unique_ptr<Reaction> react;

		// actors
		/*Actor* player_ship;
		ActorArray enemy_ships;
		ActorArray enemy_targets; // the target of the enemy
		BG* bg;*/

		std::shared_ptr<Animation> explo(const FRect* p_spawnp)
		{
			/*logger("Creating General Explosion...");
			std::cout << screen.x << "\n";
			std::cout << "texture manager..." << "\n";
			texturemgr.Construct("explo", window, Settings::IMAGE_PATH + str(Settings::config["use_texture"]["bullet_hit"]));
			std::cout << "conversion..." << "\n";
			auto xxxx = jsonex::Arr2ToRect2(Settings::image_crop["bullet_hit"]["frames"]);
			std::cout << "image crop..." << "\n";
			auto xxxxxx = Settings::image_crop["bullet_hit"]["fspeed"];
			std::cout << "enemy 0..." << "\n";
			auto xxxxxxxxx = enemies[0]->Get_Rect();

			std::cout << "make this **ck share..." << "\n";*/

			return std::make_shared<Animation>(

				VisualObject(
					&screen,
					texturemgr.Construct("explo", window, Settings::IMAGE_PATH + str(Settings::config["use_texture"]["bullet_hit"])),
				FRect(0, 0, Settings::config["size"]["bullet_hit"][0], Settings::config["size"]["bullet_hit"][1])),
				FrameList(
					jsonex::Arr2ToRect2(Settings::image_crop["bullet_hit"]["frames"]), 
					Settings::image_crop["bullet_hit"]["fspeed"]),
				p_spawnp,
				RectMan::RandomInsideOffMid
			);
		}
		std::unique_ptr<AnimReaction> newreact(std::shared_ptr<Animation>& share, const FRect* p_spawnp)
		{
			//logger("Selected shared");
			share = explo(p_spawnp);
			//logger("Passed shared");
			return std::make_unique<AnimReaction>(share);
			
		}
	};



	ShmupGame::ShmupGame(Window* p_window, const Rect& p_rect) : 
		window(p_window),
		screen(p_rect)
	{
		logger("Initializing <Shmup>...");
		Settings::Init();

		logger("Converting json vec2 to rect2...");
		frames = jsonex::Arr2ToRect2(Settings::image_crop["player_bullet"]["frames"]);

		logger("Creating Gun...");
		gun = new Gun(VisualObject(
			&screen, 
			texturemgr.Construct("1", window, Settings::IMAGE_PATH + str(Settings::config["use_texture"]["player_bullet"])),
			FRect(0, 0, Settings::config["size"]["player_bullet"][0], Settings::config["size"]["player_bullet"][1])),
			15,
			&owner,
			nullptr,
			&frames,
			Settings::image_crop["player_bullet"]["fspeed"]
		);
		//gun->Get_Texture()->Set_Flip(SDL_FLIP_VERTICAL);

		logger("Creating framemap...");
		framemap["idle"]      = FrameList(jsonex::Arr2ToRect2(Settings::image_crop["player"]["frames"]["idle"]), Settings::image_crop["player"]["idle_fspeed"]);
		framemap["move_left"] = FrameList(jsonex::Arr2ToRect2(Settings::image_crop["player"]["frames"]["move_left"]), Settings::image_crop["player"]["move_left_fspeed"]);
		framemap["move_right"]= FrameList(jsonex::Arr2ToRect2(Settings::image_crop["player"]["frames"]["move_right"]), Settings::image_crop["player"]["move_right_fspeed"]);

		logger("Creating player");
		player = new LeanAnimatedShip(VisualObject(
			&screen, 
			texturemgr.Construct("player", window, Settings::IMAGE_PATH + str(Settings::config["use_texture"]["player"])),
			FRect(200, 200, Settings::config["size"]["player"][0], Settings::config["size"]["player"][1])),
			gun,
			&framemap
		);


		logger("Converting json vec2 to rect2...");
		frames2 = jsonex::Arr2ToRect2(Settings::image_crop["enemy_bullet"]["frames"]);

		logger("Creating Enemy Gun...");
		enemygun = new Gun(VisualObject(
			&screen, 
			texturemgr.Construct("2", window, Settings::IMAGE_PATH + str(Settings::config["use_texture"]["enemy_bullet"])),
			FRect(0, 0, Settings::config["size"]["enemy_bullet"][0], Settings::config["size"]["enemy_bullet"][1])),
			10,
			&owner,
			nullptr,
			&frames2,
			Settings::image_crop["player_bullet"]["fspeed"]
		);

		enemyframe["idle"] = FrameList(jsonex::Arr2ToRect2(Settings::image_crop["enemy_weak"]["frames"]["idle"]), Settings::image_crop["enemy_weak"]["idle_fspeed"]);
		enemies.push_back(new AnimatedShip(VisualObject(
			&screen, 
			texturemgr.Construct("enemy", window, Settings::IMAGE_PATH + str(Settings::config["use_texture"]["enemy_weak"])),
			FRect(300, 0, Settings::config["size"]["enemy_weak"][0], Settings::config["size"]["enemy_weak"][1])),
			enemygun,
			&enemyframe
		));
		gun->Set_SpawnPoint(&player->Get_Rect(), RectMan::CenterToCenter);
		gun->Set_Targets(&player_targets);
		gun->Set_RechargeTime((Uint32)Settings::config["speed"]["player_recharge"]);
		gun->Set_Ammo((Uint32)Settings::config["ammo"]["player"]);


		enemygun->Set_SpawnPoint(&enemies[0]->Get_Rect(), RectMan::CenterToCenter);
		enemygun->Set_Targets(&enemy_targets);
		enemygun->Set_RechargeTime((Uint32)Settings::config["speed"]["enemy_recharge"]);
		enemygun->Set_Ammo((Uint32)Settings::config["ammo"]["enemy_weak"]);


		enemy_targets.push_back(player);
		player_targets.push_back(enemies[0]);

		//explosion = explo();

		react = newreact(explosion, &enemies[0]->Get_Rect());
		explosion->Set_Rect(FRect{0, 0, 
			Settings::config["size"]["ship_explosion_elite"][0], 
			Settings::config["size"]["ship_explosion_elite"][1]});
		explosion->Set_SpawnPointManip(RectMan::CenterToCenter);

		//gun->Set_DeathReaction(newreact);
		enemies.back()->Set_DeathReaction(std::move(react));

		timer.Peek();


	}
	ShmupGame::~ShmupGame()
	{
		logger("Destructing <Shmup>...");
		window = nullptr;
		//delete gun;
		delete player;

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
		player->Set_Vel(FVec2{0.0f, 0.0f});
		
		const Uint8* state = SDL_GetKeyboardState(NULL);

			
		if 		(state[SDL_SCANCODE_A]) player->Move_Left(Settings::config["speed"]["player_move"]);
		else if (state[SDL_SCANCODE_D]) player->Move_Right(Settings::config["speed"]["player_move"]);
		if 		(state[SDL_SCANCODE_W]) player->Move_Top(Settings::config["speed"]["player_move"]);
		else if (state[SDL_SCANCODE_S]) player->Move_Bottom(Settings::config["speed"]["player_move"]);

		if (state[SDL_SCANCODE_J] && player->Get_Gun()->Is_Recharged())
		{
			anims.push_back(std::shared_ptr<Animation>{nullptr});
			player->Get_Gun()->Fire(FVec2{0, -fabs((float)Settings::config["speed"]["normal_bullet"])}, 
				Settings::config["damage"]["player"], newreact(anims.back(), &enemies[0]->Get_Rect()));
		}
	
		
		if (Event::GetEvent().type == SDL_KEYUP)
		{
			if (Event::GetEvent().key.keysym.sym == SDLK_a || Event::GetEvent().key.keysym.sym == SDLK_d)
			{
				player->DoneMoving();
			}
		}


	}
	void ShmupGame::Update(float p_dt)
	{
		if (timer.SinceLastPeek() >= 500 && !enemies.empty() && enemies[0]->Get_Gun()->Is_Recharged())
		{
			timer.Peek();
			toplayeranims.push_back(std::shared_ptr<Animation>{nullptr});
			enemies[0]->Get_Gun()->Fire(FVec2{0, fabs((float)Settings::config["speed"]["slow_bullet"])}, 
				Settings::config["damage"]["enemy_weak"], newreact(toplayeranims.back(), &player->Get_Rect()));
		}
		/*bg->Update(p_dt);
		player_ship->Update(p_dt);
		for(auto& enemy : enemy_ships)
		{
			enemy->Update(p_dt);
		}*/

		
		for (auto enemy = enemies.begin(); enemy != enemies.end(); )
		{
			(*enemy)->Update(p_dt);
			if ( ! (*enemy)->Is_Alive())
			{
				delete *enemy;
				enemy = enemies.erase(enemy);
			}
			else
				enemy++;
		}

		player->Update(p_dt);
		explosion->Update(p_dt);

		for (auto& bullanim : anims)
		{
			bullanim->Update(p_dt);
		}
		for (auto& bullanim : toplayeranims)
		{
			bullanim->Update(p_dt);
		}


	}
	void ShmupGame::Render()
	{
		/*bg->Render();
		player_ship->Render();
		for(auto enemy : enemy_ships)
		{
			enemy->Render();
		}*/
		for(auto i : enemies)
		{
			i->Render();
		}
		player->Render();
		explosion->Render();

		for (auto bullanim : anims)
		{
			bullanim->Render();
		}
		for (auto& bullanim : toplayeranims)
		{
			bullanim->Render();
		}


	}
}

