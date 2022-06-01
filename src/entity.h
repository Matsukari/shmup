#pragma once
#define entity_h

#include "../lib/include/logger.h"
#include "../lib/include/texture.h"
#include "../lib/include/shapes.h"
#include <memory>
#include <vector>
#include <string>

namespace Shmup
{
	using Vec2 = SDL_FPoint;
	using Vecf2 = SDL_FPoint;
	using Texture_ptr = std::shared_ptr<Texture>;
	using RectArray = std::vector<Rect>;
	struct EntityProp
	{

		Texture_ptr texture;
		FRect rect;

		 RectArray frames;
		float fspeed; // frame dureation; how many ms needed to advance in a second
	};
	class Entity : public EntityProp
	{
	public:
		Entity(const EntityProp& p_prop);
		virtual ~Entity();

		Texture_ptr GetTexture() const noexcept { return texture; }
		const RectArray& GetFrames() const noexcept { return frames; }
		const FRect& GetRect() const noexcept { return rect; }

		void SetRect(const FRect& p_rect) noexcept { rect = p_rect; }

		virtual void HandleEvents(const SDL_Event& p_event) {}
		virtual void Update(float p_dt);
		virtual void Render();

	protected:
		int curframe; // convert to int when indexing. It is so that it can accumulate vaalues without spilling anything( dt is a float )
		int id;

	private:
		static int instances;
	};
	int Entity::instances = 0;

	Entity::Entity(const EntityProp& p_prop) :
		EntityProp(p_prop)
	{
		logger("Initializing <Entity><", id, ">...");

		instances++;
		id = instances;

		curframe = 0;

	}
	Entity::~Entity()
	{
		logger("Destructing <Entity><", id, ">...");
	}

	void Entity::Update(float p_dt)
	{
		curframe += (fspeed * p_dt) / 60.0f;
	}


	void Entity::Render()
	{
		texture->Render(Rect{rect}, &frames.at(curframe));
	}

}