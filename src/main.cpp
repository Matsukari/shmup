
#include "../lib/include/logger.h"
#include "../lib/include/sdlut.h"
#include "../lib/include/timer.h"
#include "../lib/include/texture.h"
#include "../lib/include/ttf.h"
#include "../lib/include/window.h"
#include "../lib/include/collisions.h"
#include "../lib/include/event.h"
#include "../lib/include/resource_manager.h"
#include <vector>
#include <memory>

// g++ logger.o color.o timer.o shapes.o window.o texture.o ttf.o collisions.o sdlut.o main.o -std=c++17 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -o exe; ./exe


void SDL_SetRenderDrawColor(SDL_Renderer* p_renderer, SDL_Color p_color, Uint8 p_alpha = 255)
{
	SDL_SetRenderDrawColor(p_renderer, p_color.r, p_color.g, p_color.b, p_alpha);
}



#include <SDL2/SDL2_gfxPrimitives.h>
using Texture_ptr = std::shared_ptr<Texture>;

class App : public Window, public Event
{
public:
	App();
	~App();

	void Exe(); // run

protected:
	void On_Events();
	void On_Update();
	void On_Render();

private:
	TTF font;
	ResourceManager<Texture> txtmgr;
	Texture_ptr texture;

	Timer timer;

	float dt = 0;
	float fps = 0;
	int rendered_frames = 0;


	//FCircle a;
	//FCircle b;

	Rect a;

};

App::App() :
	Window(
		"Ex", Rect{
		SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED, 
		720, 
		550}, 
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE, 
		SDL_RENDERER_ACCELERATED),
	font(renderer, "assets/fonts/sans.ttf", 64)
{
	SDL_StartTextInput();
	//a = FCircle{100, 100, 60};
	//b = FCircle{200, 200, 60};

	a = Rect{100, 100, 200, 200};

	texture = txtmgr.Construct("1", this, "assets/images/ship.png");
	//texture->Lock();

}
App::~App()
{
	SDL_StopTextInput();

	//delete shmup;
	logger("Ended App");
}

void App::Exe()
{
	logger("App starting to run");
	while(!Event::Has_Quit())
	{
		On_Events();
		On_Update();
		On_Render();
	}
}

void App::On_Events()
{
	while(SDL_PollEvent(&event))
	{
		Event::HandleEvents();
	}
}
void App::On_Update()
{
	SDL_RenderPresent(renderer);
	dt = timer.SinceLastPeek() / 1000.0f;
	fps = rendered_frames / (timer.Peek() / 1000.0f);

	Event::Update();

	/*Circle temp = a;
	temp.x = Get_MousePos().x;
	temp.y = Get_MousePos().y;

	if (!Has_Collided(temp, b))
	{
		a = temp;
	}*/



}

template<class T>
std::string ts(const T& n)
{
	return std::move(std::to_string(n));
}

void App::On_Render()
{
	SDL_SetRenderDrawColor(renderer, BLACK);
	SDL_RenderClear(renderer);


	Rect fontrect(0, 0, 15, 15);
	font.Render(std::to_string(fps), fontrect);

	font.Render(Event::Get_TextInput(), Rect(300, 200,15, 15));


	//SDL_SetRenderDrawColor(renderer, WHITE);
	//filledCircleRGBA(renderer, b.x, b.y, b.r,  255, 255, 255, 255);

	//SDL_SetRenderDrawColor(renderer, GREEN);
	//filledCircleRGBA(renderer, a.x, a.y, a.r,  0, 255, 0, 255);

	//a.w = texture->Get_W();
	//a.h = texture->Get_H();

	texture->Render(a);



	rendered_frames++;
}









int main(int argc, char** argv)
{
	if (!SDLU_Init(SDL_INIT_EVERYTHING))
	{
		return -1;
	}
	App* app = new App();
	app->Exe();
	delete app;
	app = nullptr;

	logger("Closing Audio");
	Mix_CloseAudio();

	SDLU_Quit();

	return 0;
}