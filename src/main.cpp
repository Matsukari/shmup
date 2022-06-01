
#include "../lib/include/logger.h"
#include "../lib/include/sdlut.h"
#include "../lib/include/timer.h"
#include "../lib/include/texture.h"
#include "../lib/include/ttf.h"
#include "../lib/include/window.h"
#include "../lib/include/collisions.h"
#include "shmup.h"

// g++ logger.o color.o timer.o shapes.o window.o texture.o ttf.o collisions.o sdlut.o main.o -std=c++17 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -o exe; ./exe


void SDL_SetRenderDrawColor(SDL_Renderer* p_renderer, SDL_Color p_color, Uint8 p_alpha = 255)
{
	SDL_SetRenderDrawColor(p_renderer, p_color.r, p_color.g, p_color.b, p_alpha);
}


class App : public Window
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
	Shmup::ShmupGame* shmup;
	SDL_Event event;
	TTF font;

	Timer timer;

	bool has_quit = false;
	float dt = 0;
	float fps = 0;
	int rendered_frames = 0;
};

App::App() :
	Window(
		"Ex", Rect{
		SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED, 
		720, 
		550}, 
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE, 
		SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED),
	font(renderer, "assets/fonts/sans.ttf", 24)
{
	has_quit = false;
	shmup = new Shmup::ShmupGame(renderer, GetRect());

}
App::~App()
{
	delete shmup;
	logger("Ended App");
}

void App::Exe()
{
	logger("App starting to run");
	while(!has_quit)
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
		if(event.type == SDL_QUIT ||
		  (event.type == SDL_WINDOWEVENT && event.window.event   == SDL_WINDOWEVENT_CLOSE) ||
		  (event.type == SDL_KEYDOWN     && event.key.keysym.sym == SDLK_ESCAPE))
		{
			has_quit = true;
			
			logger(" Quitt");
		}

		shmup->HandleEvents(event);
	}
}
void App::On_Update()
{
	SDL_RenderPresent(renderer);
	dt = timer.SinceLastPeek() / 1000.0f;
	fps = rendered_frames / (timer.Peek() / 1000.0f);

	shmup->Update(dt);


}
void App::On_Render()
{
	SDL_SetRenderDrawColor(renderer, BLACK);
	SDL_RenderClear(renderer);

	shmup->Render();

	Rect fontrect(0, 0, 15, 15);
	font.Render(std::to_string(fps), fontrect);
	
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