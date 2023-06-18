#include <chrono>
#include <time.h>
#include <SDL.h>
#include "Common.h"
#include "LevelMap.h"
#include <utils/Log.h>

struct log_stream :  private std::streambuf , public std::ostream
{
    log_stream() : std::ostream(this) {}

private:

    log_stream& operator << (const std::string& s) {
        SDL_Log("%s", s.c_str());
        return *this;
    }
};

log_stream g_log_stream;

LOG_POSTFIX("\n");
LOG_PREFIX("[arkanoid]: ");

float block_size = 20.0f;

LevelMapPtr level;

void
render(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    level->render(renderer);
    SDL_RenderPresent(renderer);
}

void handleMouseButtonDown(SDL_Event * event)
{
    level->onMouseButtonDown();
}

/* called from main event loop */
void handleMouseButtonUp(SDL_Event * event)
{
    //level->onMouseButtonUp();
}

void handleMouseMove(SDL_MouseMotionEvent* event)
{
    level->onMouseMove(event);
}

int main(int argc, char *argv[])
{
    LOG_STREAM([]() -> std::ostream& {return g_log_stream;})
    LOG("Start the game");
    SDL_Window *window;
    SDL_Renderer *renderer;
    int done;
    SDL_Event event;

    /* initialize SDL */
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Could not initialize SDL\n");
        return 1;
    }
    
    /* seed random number generator */
    srand(time(NULL));
    
    /* create window and renderer */
    window =
    SDL_CreateWindow(NULL, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
                     SDL_WINDOW_OPENGL 
                     | SDL_WINDOW_FULLSCREEN_DESKTOP
                     );
    if (!window) {
        printf("Could not initialize Window\n");
        return 1;
    }
    
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        printf("Could not create renderer\n");
        return 1;
    }
    
    SDL_DisplayMode mode;
    SDL_GetDisplayMode(0, 0, &mode);
    block_size = mode.h / 16;
    
    SDL_Log("block_size: %f", block_size);
    
	level = std::make_unique<LevelMap>();
    
    /* Enter render loop, waiting for user to quit */
    done = 0;
    std::chrono::time_point<std::chrono::steady_clock> before = std::chrono::steady_clock::now();
    std::chrono::time_point<std::chrono::steady_clock> after = std::chrono::steady_clock::now();
    while (!done) {
        float dt = std::chrono::duration_cast<std::chrono::milliseconds>(after - before).count() / 1000.0f;
        before = std::chrono::steady_clock::now();
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_MOUSEBUTTONDOWN:
                    handleMouseButtonDown(&event);
                    break;
                case SDL_MOUSEBUTTONUP:
                    handleMouseButtonUp(&event);
                    break;
                case SDL_MOUSEMOTION:
                    handleMouseMove((SDL_MouseMotionEvent*)&event);
                    break;
                case SDL_QUIT:
                    done = 1;
                    break;
            }
        }
		
		const Uint8* state = SDL_GetKeyboardState(NULL);
		if (state[SDL_SCANCODE_ESCAPE])
			return 0;
		
        level->update(dt);
        render(renderer);
        SDL_Delay(1);
        after = std::chrono::steady_clock::now();
    }

	level.reset();

    /* shutdown SDL */
    SDL_Quit();
    
    return 0;
}
