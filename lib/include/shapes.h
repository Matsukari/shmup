#pragma once
#define shapes_h

#include <SDL2/SDL.h>



struct Rect;

struct FRect : public SDL_FRect
{
	FRect(float px, float py, float pw, float ph) noexcept {x = px; y = py; w = pw; h = ph;}
	FRect() noexcept {x = 0; y = 0; w = 0; h = 0;}
	FRect(SDL_Rect prect) noexcept; // polymorhp Rect
	~FRect() = default;

	float Top()    const noexcept { return y;     };
	float Bottom() const noexcept { return y + h; };
	float Left()   const noexcept { return x;     };
	float Right()  const noexcept { return x + w; };

	float CenterToX() const noexcept { return x - (w / 2); }
	float CenterToY() const noexcept { return y - (h / 2); }

	// the center of rect
	float RectCenterX() const noexcept { return x + (w / 2); }
	float RectCenterY() const noexcept{ return y + (h / 2); }

	void Set_RightToRight(FRect rect) noexcept
	{
		x = rect.Right() - w;
	}
	void Set_BotToBot(FRect rect) noexcept
	{
		y = rect.Bottom() - h;
	}

	void Set_TopRightToTopRight(FRect rect) noexcept
	{
		Set_RightToRight(rect);
		y = rect.y;
	}

	// this center will be centered in RECT's center
	void Set_CenterToCenter(FRect rect) noexcept
	{
		// x is stil in topleft from prect center
		x = rect.RectCenterX();
		y = rect.RectCenterY();

		x -= static_cast<int>(w/2);
		y -= static_cast<int>(h/2);

	}

}; 


struct Rect : public SDL_Rect
{
	Rect(int px, int py, int pw, int ph) noexcept {x = px; y = py; w = pw; h = ph;}
	Rect() noexcept {x = 0; y = 0; w = 0; h = 0;}
	Rect(SDL_FRect prect) noexcept; // polymorph FRect
	~Rect() =  default;


	int Top()    const noexcept  { return y;     };
	int Bottom() const noexcept  { return y + h; };
	int Left() 	 const noexcept  { return x;     };
	int Right()  const noexcept  { return x + w; };

	// the x mentioned is in topleft; the rect's center will be centered in this x
	float CenterToX() const noexcept { return x - (w / 2); }
	float CenterToY() const noexcept { return y - (h / 2); }

	// the center of rect
	float RectCenterX() const noexcept { return x + (w / 2); }
	float RectCenterY() const noexcept { return y + (h / 2); }


	void Set_RightToRight(Rect rect) noexcept
	{
		x = rect.Right() - w;
	}
	void Set_BotToBot(Rect rect) noexcept
	{
		y = rect.Bottom() - h;
	}

	void Set_TopRightToTopRight(Rect rect) noexcept
	{
		Set_RightToRight(rect);
		y = rect.y;
	}

	// this center will be centered in RECT's center
	void Set_CenterToCenter(Rect rect) noexcept
	{
		// x is stil in topleft from prect center
		x = rect.RectCenterX();
		y = rect.RectCenterY();

		x -= static_cast<int>(w/2);
		y -= static_cast<int>(h/2);

	}

}; 


struct FCircle;

struct Circle : public SDL_Point
{
	Circle(int px, int py, int pr) noexcept { x = px; y = py; r = pr; }
	Circle(FCircle pcircle) noexcept;
	Circle() noexcept { x = 0; y = 0; r = 0; }
	~Circle()=default;

	int r;

	int TopLeft_X() const noexcept { return x - r; }
	int TopLeft_Y() const noexcept { return y - r; }

	int Diameter() const noexcept { return (r + r ); }


};

struct FCircle : public SDL_FPoint
{
	FCircle(float px, float py, float pr) noexcept { x = px; y = py; r = pr; }
	FCircle(Circle pcircle) noexcept;
	FCircle() noexcept { x = 0.0f; y = 0.0f; r = 0.0f; }
	~FCircle()=default;
	float r;

	float TopLeftX() const noexcept { return x - r; }
	float TopLeftY() const noexcept { return y - r; }

	float Diameter() const noexcept { return (r + r ); }

};
