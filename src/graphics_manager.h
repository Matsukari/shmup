#pragma once

#include "lean_animatedship.h"
#include "animation.h"
#include "reaction.h"

namespace Shmup
{
	namespace Manager
	{
		// shouldnt have separated this shit to shmup, this beast basically manage everything
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

			void UpdateLastStand(float p_dt);
			void RenderLastStand();



			void DrawHPBar(const Actor* actor, SDL_FRect barrect, const Color& color);
			

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
			FrameMap en_weak_framemap;
			FrameMap en_elite_framemap;
			FrameMap en_boss_framemap;

			RectArray pl_bull_frames;
			RectArray en_bull_frames;

			Actor* pl_lasthitactor=nullptr;


			// effects
			std::vector<std::shared_ptr<Animation>> animations;

			// targets ,unallocated pointers
			ActorArray pl_targets; // player tragets; the enemy
			ActorArray en_targets; // enemy targets; player


			std::vector<Object*> renderable;


			nlohmann::json& crop = Settings::image_crop;
			nlohmann::json& config = Settings::config;

			bool is_everything_ok_after_gameover = false;

			
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
			logger("Setting max health from player");
			player->Set_MaxHealth(config["health"]["player"]);
			logger("Setting health from player");

			player->Set_Health(config["health"]["player"]);
			player->Get_Gun()->Set_SpawnPoint(&player->Get_Rect(), RectMan::CenterToCenter);
			player->Get_Gun()->Set_Targets(&pl_targets);
			player->Get_Gun()->Set_RechargeTime((Uint32)config["speed"]["player_recharge"]);
			logger("Setting ammo from player");

			player->Get_Gun()->Set_Ammo((Uint32)config["ammo"]["player"]);
			logger("Setting death react from player");

			player->Set_DeathReaction( NewAnimReact_Explosion(&player->Get_Rect()) );

			animations.back()->Set_Rect(FRect(0, 0, 
				config["size"]["world_explosion"][0], 
				config["size"]["world_explosion"][1]));
			animations.back()->Set_SpawnPointManip(RectMan::CenterToCenter);

			en_targets.push_back(player);


			NewMob_Weak();
			NewMob_Weak();
			enemies.back()->Set_Rect(FRect(160, 30+50, config["size"]["enemy_weak"][0], config["size"]["enemy_weak"][1]));
			NewMob_Weak();
			enemies.back()->Set_Rect(FRect(70, 10+50, config["size"]["enemy_weak"][0], config["size"]["enemy_weak"][1]));
			NewMob_Weak();
			enemies.back()->Set_Rect(FRect(500, 50+50, config["size"]["enemy_weak"][0], config["size"]["enemy_weak"][1]));

			NewMob_Elite();
			enemies.back()->Set_Rect(FRect(360, 100+50, config["size"]["enemy_elite"][0], config["size"]["enemy_elite"][1]));

			NewMob_Boss();
			enemies.back()->Set_Rect(FRect(300, 0, config["size"]["enemy_boss"][0], config["size"]["enemy_boss"][1]));


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
			pl_lasthitactor = nullptr;

			delete player;
			for(auto& en : enemies)
			{
				delete en;
			}
			delete bgsprite;


		}

		void Graphics::Update(float p_dt)
		{
			bgsprite->Update(p_dt);

			auto enemy_ptr = pl_targets.begin();
			for (auto enemy = enemies.begin(); enemy != enemies.end(); )
			{
				if ((*enemy)->Get_Gun()->Is_Recharged())
				{
					(*enemy)->Get_Gun()->Fire(FVec2{0, fabs((float)Settings::config["speed"]["slow_bullet"])}, 
						Settings::config["damage"]["enemy_weak"]);

					// triggered on deletion of p_bullet
					auto OnBulletCollide = [&](BulletList::iterator& p_bullet) -> void
					{
						Actor* who = (*p_bullet)->Get_WhoCollided();
						if (who)
						{
							logger("\n\nCollided, so, boom!\n\n");
							(*p_bullet)->Set_DeathReaction(NewAnimReact_Explosion(&who->Get_Rect()) );
						}
						who = nullptr;
					};
					(*enemy)->Get_Gun()->Set_CallOnDeadBullet(OnBulletCollide);
				}

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
				FRect frect{0, 0, 200.0, 7.0};
				frect.x = 5;
				frect.y = screen.Bottom() - frect.h;
				DrawHPBar(player, frect, GREEN);
			}
			if (pl_lasthitactor)
			{
				FRect frect{0, 0, 200.0, 7.0};
				frect.x = screen.Right() - frect.w - 5;
				frect.y = screen.Bottom() - frect.h;
				DrawHPBar(pl_lasthitactor, frect, RED);
			}
		}
		void Graphics::UpdateLastStand(float p_dt)
		{
			if (!bgsprite->SlowWhile(50.0f, 0.0f, p_dt))
			{
				is_everything_ok_after_gameover = true;
			}
			bgsprite->Update(p_dt);

			for (auto& anim : animations)
			{
				anim->Update(p_dt);
			}
		}
		void Graphics::RenderLastStand()
		{
			bgsprite->Render();

			for(auto& anim : animations) anim->Render();
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
			
			en_weak_framemap["idle"] = FrameList(jsonex::Arr2ToRect2(crop["enemy_weak"]["frames"]["idle"]), crop["enemy_weak"]["idle_fspeed"]);
			en_elite_framemap["idle"] = FrameList(jsonex::Arr2ToRect2(crop["enemy_elite"]["frames"]["idle"]), crop["enemy_elite"]["idle_fspeed"]);
			en_boss_framemap["idle"] = FrameList(jsonex::Arr2ToRect2(crop["enemy_boss"]["frames"]["idle"]), crop["enemy_boss"]["idle_fspeed"]);
		
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
					texturemgr.Construct("enemy_weak", window, Settings::IMAGE_PATH + str(config["use_texture"]["enemy_weak"])),
					FRect(300, 0, config["size"]["enemy_weak"][0], config["size"]["enemy_weak"][1])),
				new Gun(
					VisualObject(
						&screen, 
						texturemgr.Construct("enemy_bullet", window, Settings::IMAGE_PATH + str(config["use_texture"]["enemy_bullet"])),
						FRect(0, 0, config["size"]["enemy_bullet"][0], config["size"]["enemy_bullet"][1])),
					10,
					nullptr,
					nullptr,
					&en_bull_frames,
					crop["enemy_bullet"]["fspeed"]
				),
				&en_weak_framemap
			));
			enemies.back()->Set_MaxHealth(config["health"]["enemy_weak"]);
			enemies.back()->Set_Health(config["health"]["enemy_weak"]);
			enemies.back()->Get_Gun()->Set_SpawnPoint(&enemies.back()->Get_Rect(), RectMan::CenterToCenter);
			enemies.back()->Get_Gun()->Set_Targets(&en_targets);
			enemies.back()->Get_Gun()->Set_RechargeTime((Uint32)config["speed"]["enemy_weak_recharge"]);
			enemies.back()->Get_Gun()->Set_Ammo((Uint32)config["ammo"]["enemy_weak"]);
			enemies.back()->Set_DeathReaction( NewAnimReact_Explosion(&enemies.back()->Get_Rect()) );

			animations.back()->Set_Rect(FRect(0, 0, 
				config["size"]["ship_explosion_weak"][0], 
				config["size"]["ship_explosion_weak"][1]));
			animations.back()->Set_SpawnPointManip(RectMan::CenterToCenter);

			pl_targets.push_back(enemies.back());
		}
		void Graphics::NewMob_Elite()
		{
			logger("\ncreating mob elite...\n");
			enemies.push_back(new AnimatedShip(
				VisualObject(
					&screen, 
					texturemgr.Construct("enemy_elite", window, Settings::IMAGE_PATH + str(config["use_texture"]["enemy_elite"])),
					FRect(300, 0, config["size"]["enemy_elite"][0], config["size"]["enemy_elite"][1])),
				new Gun(
					VisualObject(
						&screen, 
						texturemgr.Construct("enemy_bullet", window, Settings::IMAGE_PATH + str(config["use_texture"]["enemy_bullet"])),
						FRect(0, 0, config["size"]["enemy_bullet"][0], config["size"]["enemy_bullet"][1])),
					10,
					nullptr,
					nullptr,
					&en_bull_frames,
					crop["enemy_bullet"]["fspeed"]
				),
				&en_elite_framemap
			));
			enemies.back()->Set_MaxHealth(config["health"]["enemy_elite"]);
			enemies.back()->Set_Health(config["health"]["enemy_elite"]);
			enemies.back()->Get_Gun()->Set_SpawnPoint(&enemies.back()->Get_Rect(), RectMan::CenterToCenter);
			enemies.back()->Get_Gun()->Set_Targets(&en_targets);
			enemies.back()->Get_Gun()->Set_RechargeTime((Uint32)config["speed"]["enemy_elite_recharge"]);
			enemies.back()->Get_Gun()->Set_Ammo((Uint32)config["ammo"]["enemy_elite"]);
			enemies.back()->Set_DeathReaction( NewAnimReact_Explosion(&enemies.back()->Get_Rect()) );

			animations.back()->Set_Rect(FRect(0, 0, 
				config["size"]["ship_explosion_elite"][0], 
				config["size"]["ship_explosion_elite"][1]));
			animations.back()->Set_SpawnPointManip(RectMan::CenterToCenter);

			pl_targets.push_back(enemies.back());

		}
		void Graphics::NewMob_Boss()
		{
			logger("\ncreating mob boss...\n");
			enemies.push_back(new AnimatedShip(
				VisualObject(
					&screen, 
					texturemgr.Construct("enemy_boss", window, Settings::IMAGE_PATH + str(config["use_texture"]["enemy_boss"])),
					FRect(300, 0, config["size"]["enemy_boss"][0], config["size"]["enemy_boss"][1])),
				new Gun(
					VisualObject(
						&screen, 
						texturemgr.Construct("enemy_bullet", window, Settings::IMAGE_PATH + str(config["use_texture"]["enemy_bullet"])),
						FRect(0, 0, config["size"]["enemy_bullet"][0], config["size"]["enemy_bullet"][1])),
					10,
					nullptr,
					nullptr,
					&en_bull_frames,
					crop["enemy_bullet"]["fspeed"]
				),
				&en_boss_framemap
			));
			enemies.back()->Set_MaxHealth(config["health"]["enemy_boss"]);
			enemies.back()->Set_Health(config["health"]["enemy_boss"]);
			enemies.back()->Get_Gun()->Set_SpawnPoint(&enemies.back()->Get_Rect(), RectMan::CenterToCenter);
			enemies.back()->Get_Gun()->Set_Targets(&en_targets);
			enemies.back()->Get_Gun()->Set_RechargeTime((Uint32)config["speed"]["enemy_boss_recharge"]);
			enemies.back()->Get_Gun()->Set_Ammo((Uint32)config["ammo"]["enemy_boss"]);
			enemies.back()->Set_DeathReaction( NewAnimReact_Explosion(&enemies.back()->Get_Rect()) );

			animations.back()->Set_Rect(FRect(0, 0, 
				config["size"]["ship_explosion_boss"][0], 
				config["size"]["ship_explosion_boss"][1]));
			animations.back()->Set_SpawnPointManip(RectMan::CenterToCenter);


			pl_targets.push_back(enemies.back());
		}

		void Graphics::DrawHPBar(const Actor* actor, SDL_FRect barrect, const Color& color)
		{
			SDL_FRect hprect=barrect; // barrect; outline, hprect; filled
			hprect.w = (float)((float)actor->Get_Health() / (float)actor->Get_MaxHealth()) * hprect.w;

			// fill
			SDL_SetRenderDrawColor(window->Get_Renderer(), color);
			SDL_RenderFillRectF(window->Get_Renderer(), &hprect);

			// outline
			SDL_SetRenderDrawColor(window->Get_Renderer(), BLACK);
			for (float col = barrect.x; col < barrect.x + barrect.w; col += (barrect.w * 0.1))
			{
				SDL_RenderDrawLineF(window->Get_Renderer(), col, barrect.y, col, barrect.y + barrect.h);

			}
			SDL_RenderDrawRectF(window->Get_Renderer(), &hprect);



			
		};


	}

}
