#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

SDL_Window *window;
SDL_Renderer *renderer;
int done;

void DrawChessBoard(SDL_Renderer *renderer)
{
    SDL_FRect rect;
    SDL_Rect darea;

    /* Get the Size of drawing surface */
    SDL_GetRenderViewport(renderer, &darea);

    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);

    for (int row = 0; row < 8; row++) {
        int column = row % 2;
        int x = column;
        for (; column < 4 + (row % 2); column++) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);

            rect.w = static_cast<float>(darea.w) / 8;
            rect.h = static_cast<float>(darea.h) / 8;
            rect.x = rect.w * static_cast<float>(x);
            rect.y = rect.h * static_cast<float>(row);
            x = x + 2;
            SDL_RenderFillRect(renderer, &rect);
        }
    }
    SDL_RenderPresent(renderer);
}

void loop()
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

    DrawChessBoard(renderer);

    /* Got everything on rendering surface,
       now Update the drawing image on window screen */
    SDL_UpdateWindowSurface(window);
}

int main(int argc, char *argv[])
{
    SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_Init fail : %s\n", SDL_GetError());
        return 1;
    }

    /* Create window and renderer for given surface */
    window = SDL_CreateWindow("Chess Board", 640, 480, 0);
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

    /* Draw the Image on rendering surface */
    done = 0;

    while (!done) {
        loop();
    }

    SDL_Quit();
    return 0;
}