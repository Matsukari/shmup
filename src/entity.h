#pragma once
#define entity_h

#include "../lib/include/logger.h"
#include "../lib/include/texture.h"
#include "../lib/include/shapes.h"
#include <memory>
#include <vector>

namespace Shmup
{
	using Vec2 = SDL_Point;
	using Texture_ptr = std::shared_ptr<Texture>;
	using RectArray = std::vector<Rect>;
	struct EntityProp
	{
		Texture_ptr texture;
		FRect rect;

		RectArray frames;
		float fps; // how many ms needed to advance in a second
	};
	class Entity
	{
	public:
		Entity(const EntityProp& p_prop);
		~Entity();

		Texture_ptr GetTexture() const noexcept { return texture; }
		const RectArray* GetFrames() const noexcept { return frames; }
		const FRect& GetRect() const noexcept { return rect; }

		void SetRect(const FRect& p_rect) noexcept { rect = p_rect; }

		virtual void HandleEvents(const SDL_Event& p_event) {}
		virtual void Update(float p_dt) {}
		virtual void Render();

	protected:
		Texture_ptr texture;
		const RectArray* frames;
		float fps;
		float curframe; // convert to int when indexing. It is so that it can accumulate vaalues without spilling anything( dt is a float )
		FRect rect;

		int id;

	private:
		static int instances;
	};
	int Entity::instances = 0;

	Entity::Entity(const EntityProp& p_prop) :
		texture(p_prop.texture),
		frames(&p_prop.frames),
		rect(p_prop.rect),
		fps(p_prop.fps)
	{
		logger("Initializing <Entity><", id, ">...");

		instances++;
		id = instances;

		curframe = 0.0f;

	}
	Entity::~Entity()
	{
		logger("Destructing <Entity><", id, ">...");

		frames = nullptr;
	}

	void Entity::Render()
	{
		texture->Render(Rect{rect}, &frames->at(static_cast<int>(curframe)));
	}

}