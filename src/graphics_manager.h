#pragma once

#include "lean_animatedship.h"
#include "animation.h"
#include "reaction.h"

namespace Shmup
{
	namespace Manager
	{
		class Graphics : public Object
		{
		public:
			Graphics(Window* p_window, const Rect& p_rect);
			~Graphics() override;

			void Update(float p_dt) override;
			void Render() override;

			void NewMob_Weak();
			void NewMob_Elite();
			void NewMob_Boss();

			void DrawHPBar(const Actor* actor, SDL_FRect hprect, const Color& color);
			

		public:

			// save to frame or framemap members
			void ParseSpriteSheet();

			// creates animation; modify by to back(), and returns the actual reation you can polumorph to reacton
			std::unique_ptr<AnimReaction> NewAnimReact_Explosion(const FRect* p_spawnp);

			

		public:
			ResourceManager<Texture> texturemgr;
			Window* window;
			Rect screen;

			// actors
			LeanAnimatedShip* player;
			std::vector<AnimatedShip*> enemies;

			// backgroud
			BGSprite* bgsprite;

			// actor components

			// frames and framemap
			FrameMap pl_framemap;
			FrameMap en_framemap;
			RectArray pl_bull_frames;
			RectArray en_bull_frames;


			// effects
			std::vector<std::shared_ptr<Animation>> animations;

			// targets ,unallocated pointers
			ActorArray pl_targets; // player tragets; the enemy
			ActorArray en_targets; // enemy targets; player


			std::vector<Object*> renderable;


			nlohmann::json& crop = Settings::image_crop;
			nlohmann::json& config = Settings::config;

			
		};

	
		Graphics::Graphics(Window* p_window, const Rect& p_rect) :
			window(p_window), screen(p_rect)
		{

			ParseSpriteSheet();

			logger("Creating player");
			player = new LeanAnimatedShip(
				VisualObject(
					&screen, 
					texturemgr.Construct("player", window, Settings::IMAGE_PATH + str(config["use_texture"]["player"])),
					FRect(200, 200, config["size"]["player"][0], config["size"]["player"][1])
				),
				new Gun(
					VisualObject(
						&screen, 
						texturemgr.Construct("player_bullet", window, Settings::IMAGE_PATH + str(config["use_texture"]["player_bullet"])),
						FRect(0, 0, config["size"]["player_bullet"][0], config["size"]["player_bullet"][1])),
					15,		// ammo
					nullptr, // spawn
					nullptr,// targets
					&pl_bull_frames,// frames
					crop["player_bullet"]["fspeed"]),
				&pl_framemap
			);
			player->Get_Gun()->Set_SpawnPoint(&player->Get_Rect(), RectMan::CenterToCenter);
			player->Get_Gun()->Set_Targets(&pl_targets);
			player->Get_Gun()->Set_RechargeTime((Uint32)config["speed"]["player_recharge"]);
			player->Get_Gun()->Set_Ammo((Uint32)config["ammo"]["player"]);

			en_targets.push_back(player);


			NewMob_Weak();
			NewMob_Weak();
			enemies.back()->Set_Rect(FRect(160, 30, config["size"]["enemy_weak"][0], config["size"]["enemy_weak"][1]));
			NewMob_Weak();
			enemies.back()->Set_Rect(FRect(70, 10, config["size"]["enemy_weak"][0], config["size"]["enemy_weak"][1]));
			NewMob_Weak();
			enemies.back()->Set_Rect(FRect(500, 50, config["size"]["enemy_weak"][0], config["size"]["enemy_weak"][1]));



			bgsprite = new BGSprite(VisualObject(
				&screen,
				texturemgr.Construct("bgimage", window, Settings::IMAGE_PATH + str(config["use_texture"]["bg"])),
				screen),
				texturemgr.Construct("bgcloud", window, Settings::IMAGE_PATH + str(config["use_texture"]["clouds"])),
				(float)config["speed"]["bg_travel"]
			);


		}
		Graphics::~Graphics()
		{
			window = nullptr;

			delete player;
			player = nullptr;
			for(auto& en : enemies)
			{
				delete en;
				en = nullptr;
			}
			delete bgsprite;

			bgsprite = nullptr;

		}

		void Graphics::Update(float p_dt)
		{
			bgsprite->Update(p_dt);

			auto enemy_ptr = pl_targets.begin();
			for (auto enemy = enemies.begin(); enemy != enemies.end(); )
			{
				(*enemy)->Update(p_dt);
				if ( ! (*enemy)->Is_Alive())
				{
					delete *enemy;
					enemy = enemies.erase(enemy);
					enemy_ptr = pl_targets.erase(enemy_ptr);
				}
				else
				{
					enemy++;
					enemy_ptr++;
				}
			}
			player->Update(p_dt);


			for (auto& anim : animations)
			{
				anim->Update(p_dt);
			}
		}
		void Graphics::Render()
		{
			/*for(auto& obj : renderable)
			{
				obj->Render();
			}*/

			bgsprite->Render();
			for(auto& enemy : enemies) enemy->Render();
			player->Render();
			for(auto& anim : animations) anim->Render();


			if (player)
			{
				DrawHPBar(player, FRect(10.0, 10.0, 200.0, 10.0), GREEN);
			}
			if (!enemies.empty())
			{
				DrawHPBar(enemies[0], FRect(600.0, 10.0, 200.0, 10.0), RED);
			}
		}

		void Graphics::ParseSpriteSheet()
		{

			logger("Converting json vec2 to rect2...");
			pl_bull_frames = jsonex::Arr2ToRect2(crop["player_bullet"]["frames"]);
			en_bull_frames = jsonex::Arr2ToRect2(crop["enemy_bullet"]["frames"]);

			logger("Creating framemap...");
			pl_framemap["idle"]      = FrameList(jsonex::Arr2ToRect2(crop["player"]["frames"]["idle"]), crop["player"]["idle_fspeed"]);
			pl_framemap["move_left"] = FrameList(jsonex::Arr2ToRect2(crop["player"]["frames"]["move_left"]), crop["player"]["move_left_fspeed"]);
			pl_framemap["move_right"]= FrameList(jsonex::Arr2ToRect2(crop["player"]["frames"]["move_right"]), crop["player"]["move_right_fspeed"]);
			
			en_framemap["idle"] = FrameList(jsonex::Arr2ToRect2(crop["enemy_weak"]["frames"]["idle"]), crop["enemy_weak"]["idle_fspeed"]);
		
		}


	
		std::unique_ptr<AnimReaction> Graphics::NewAnimReact_Explosion(const FRect* p_spawnp)
		{
			logger("\ncreating new react-animation explosion...\n");

			animations.push_back( std::shared_ptr<Animation>{ nullptr } );

			animations.back() = std::make_shared<Animation>(
				VisualObject(
					&screen,
					texturemgr.Construct("bullet_hit", window, Settings::IMAGE_PATH + str(config["use_texture"]["bullet_hit"])),
				FRect(0, 0, config["size"]["bullet_hit"][0], config["size"]["bullet_hit"][1])),
				FrameList(
					jsonex::Arr2ToRect2(crop["bullet_hit"]["frames"]), 
					crop["bullet_hit"]["fspeed"]),
				p_spawnp,
				RectMan::RandomInsideOffMid
			);

			return std::make_unique<AnimReaction>(animations.back());
			
		}

		void Graphics::NewMob_Weak()
		{
			logger("\ncreating mob weak...\n");
			enemies.push_back(new AnimatedShip(
				VisualObject(
					&screen, 
					texturemgr.Construct("enemy", window, Settings::IMAGE_PATH + str(config["use_texture"]["enemy_weak"])),
					FRect(300, 0, config["size"]["enemy_weak"][0], config["size"]["enemy_weak"][1])),
				new Gun(
					VisualObject(
						&screen, 
						texturemgr.Construct("2", window, Settings::IMAGE_PATH + str(config["use_texture"]["enemy_bullet"])),
						FRect(0, 0, config["size"]["enemy_bullet"][0], config["size"]["enemy_bullet"][1])),
					10,
					nullptr,
					nullptr,
					&en_bull_frames,
					crop["player_bullet"]["fspeed"]
				),
				&en_framemap
			));
			enemies.back()->Get_Gun()->Set_SpawnPoint(&enemies[0]->Get_Rect(), RectMan::CenterToCenter);
			enemies.back()->Get_Gun()->Set_Targets(&en_targets);
			enemies.back()->Get_Gun()->Set_RechargeTime((Uint32)config["speed"]["enemy_recharge"]);
			enemies.back()->Get_Gun()->Set_Ammo((Uint32)config["ammo"]["enemy_weak"]);
			enemies.back()->Set_DeathReaction( NewAnimReact_Explosion(&enemies.back()->Get_Rect()) );

			animations.back()->Set_Rect(FRect(0, 0, 
				config["size"]["ship_explosion_elite"][0], 
				config["size"]["ship_explosion_elite"][1]));
			animations.back()->Set_SpawnPointManip(RectMan::CenterToCenter);

			pl_targets.push_back(enemies.back());
		}
		void Graphics::NewMob_Elite()
		{
		}
		void Graphics::NewMob_Boss()
		{
		}

		void Graphics::DrawHPBar(const Actor* actor, SDL_FRect hprect, const Color& color)
		{
			hprect.w = (float)((float)actor->Get_Health() / (float)actor->Get_MaxHealth()) * hprect.w;

			SDL_SetRenderDrawColor(window->Get_Renderer(), color);
			SDL_RenderFillRectF(window->Get_Renderer(), &hprect);
			
		};


	}

}