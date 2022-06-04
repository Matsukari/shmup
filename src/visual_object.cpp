#include "visual_object.h"

// static

namespace Shmup
{
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
		logger("Destructing <VisualObject><", id, ">...");
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
