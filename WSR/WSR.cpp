#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

SDL_Window *window;
SDL_Renderer *renderer;
int done;

void DrawChessBoard()
{
    SDL_Rect darea;
    SDL_GetRenderViewport(renderer, &darea);

    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);

    for(auto i = 0; i < darea.w; i++)
    {
        for(auto j = 0; j < darea.h; j++)
        {
            auto x = static_cast<float>(i);
            auto y = static_cast<float>(j);

            auto r = static_cast<Uint8>(i * 255 / darea.w);
            auto g = static_cast<Uint8>(j * 255 / darea.h);
            Uint8 b = 0x88;

            SDL_SetRenderDrawColor(renderer, r, g, b, 0);
            SDL_RenderPoint(renderer, x, y);
        }
    }
    
    SDL_RenderPresent(renderer);
}

void ProcessEvents()
{
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_EVENT_QUIT) {
            done = 1;
            return;
        }

        if ((e.type == SDL_EVENT_KEY_DOWN) && (e.key.keysym.sym == SDLK_ESCAPE)) {
            done = 1;
            return;
        }
    }
}

void MainLoop()
{
    ProcessEvents();

    DrawChessBoard();
    
    SDL_UpdateWindowSurface(window);
}

int main(int argc, char *argv[])
{
    SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_Init fail : %s\n", SDL_GetError());
        return 1;
    }
    window = SDL_CreateWindow("WSR", 400, 400, 0);
    if (!window) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Window creation fail : %s\n", SDL_GetError());
        return 1;
    }
    SDL_Surface* surface = SDL_GetWindowSurface(window);
    renderer = SDL_CreateSoftwareRenderer(surface);
    if (!renderer) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Render creation for surface fail : %s\n", SDL_GetError());
        return 1;
    }

    done = 0;
    while (!done) {
        MainLoop();
    }

    SDL_Quit();
    return 0;
}