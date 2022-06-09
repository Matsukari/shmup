#pragma once
#define visual_object_h

#include "../lib/include/texture.h"
#include "../lib/include/event.h"
#include "settings.h"


namespace Shmup
{
	class Object
	{
	public:
		virtual ~Object() = 0;

		virtual void Update(float p_dt) = 0;
		virtual void Render() = 0;
		
	};
	Object::~Object() {}
	
	// inherit me
	class VisualObject : public Object
	{
	public:
		VisualObject(const Rect* p_screen, Texture_ptr p_texture, const FRect& p_rect);
		virtual ~VisualObject() override;

		Texture_ptr Get_Texture() const noexcept { return texture; }
		//const RectArray& GetFrames() const noexcept { return frames; }
		const FRect& Get_Rect() const noexcept { return rect; }

		void Set_Rect(const FRect& p_rect) noexcept { rect = p_rect; }

		virtual void Update(float p_dt) override;
		virtual void Render() override;

	protected:
		const Rect* screen;
		Texture_ptr texture;
		FRect rect;

		int id;

	private:
		static int instances;
	};


	// static
	int VisualObject::instances = 0;

	VisualObject::VisualObject(const Rect* p_screen, Texture_ptr p_texture, const FRect& p_rect) :
		screen(p_screen),
		texture(p_texture),
		rect(p_rect)
	{
		logger("Initializing <VisualObject><", id, ">...");

		instances++;
		id = instances;

		//curframe = 0;

	}
	VisualObject::~VisualObject()
	{
		logger("Destructing <VisualObject><", id, ">...\n");
		screen = nullptr;
	}

	void VisualObject::Update(float p_dt)
	{
	}

	void VisualObject::Render()
	{
		texture->Render(rect);
	}


}