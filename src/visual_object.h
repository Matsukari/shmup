#pragma once
#define visual_object_h

#include "../lib/include/texture.h"
#include "../lib/include/event.h"
#include "settings.h"


namespace Shmup
{
	// inherit me
	class VisualObject
	{
	public:
		VisualObject(const Rect* p_screen, Texture_ptr p_texture, const FRect& p_rect);
		virtual ~VisualObject();

		Texture_ptr Get_Texture() const noexcept { return texture; }
		//const RectArray& GetFrames() const noexcept { return frames; }
		const FRect& Get_Rect() const noexcept { return rect; }

		void Set_Rect(const FRect& p_rect) noexcept { rect = p_rect; }

		virtual void HandleEvents(const Event* p_event) {}
		virtual void Update(float p_dt);
		virtual void Render();

	protected:
		const Rect* screen;
		Texture_ptr texture;
		FRect rect;

		int id;

	private:
		static int instances;
	};


}