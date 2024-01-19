#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define SPRITE_WIDTH 50
#define SPRITE_HEIGHT 50

typedef struct {
    int x, y;
    SDL_Texture* texture;
} Sprite;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
Sprite sprites[5]; // Tableau de sprites
int score = 0;

bool initializeSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow("Cliquez sur les sprites!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        fprintf(stderr, "Erreur lors de la création de la fenêtre : %s\n", SDL_GetError());
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        fprintf(stderr, "Erreur lors de la création du rendu : %s\n", SDL_GetError());
        return false;
    }

    return true;
}

SDL_Texture* loadTexture(const char* path) {
    SDL_Surface* surface = SDL_LoadBMP(path);
    if (surface == NULL) {
        fprintf(stderr, "Impossible de charger l'image %s : %s\n", path, SDL_GetError());
        return NULL;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    return texture;
}

void loadSprites() {
    for (int i = 0; i < 5; ++i) {
        sprites[i].x = rand() % (SCREEN_WIDTH - SPRITE_WIDTH);
        sprites[i].y = rand() % (SCREEN_HEIGHT - SPRITE_HEIGHT);
        // Charger l'image du sprite. Assurez-vous d'avoir des fichiers BMP dans le même répertoire que votre exécutable.
        sprites[i].texture = loadTexture("duck.bmp");
    }
}

void cleanupSDL() {
    for (int i = 0; i < 5; ++i) {
        SDL_DestroyTexture(sprites[i].texture);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void handleInput(SDL_Event event) {
    if (event.type == SDL_QUIT) {
        exit(0);
    } else if (event.type == SDL_MOUSEBUTTONDOWN) {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        for (int i = 0; i < 5; ++i) {
            if (mouseX >= sprites[i].x && mouseX <= sprites[i].x + SPRITE_WIDTH &&
                mouseY >= sprites[i].y && mouseY <= sprites[i].y + SPRITE_HEIGHT) {
                // Clique sur le sprite, incrémenter le score et déplacer le sprite
                score++;
                printf("Score : %d\n", score);

                sprites[i].x = rand() % (SCREEN_WIDTH - SPRITE_WIDTH);
                sprites[i].y = rand() % (SCREEN_HEIGHT - SPRITE_HEIGHT);
            }
        }
    }
}

void render() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    for (int i = 0; i < 5; ++i) {
        SDL_Rect destRect = { sprites[i].x, sprites[i].y, SPRITE_WIDTH, SPRITE_HEIGHT };
        SDL_RenderCopy(renderer, sprites[i].texture, NULL, &destRect);
    }

    SDL_RenderPresent(renderer);
}

int main() {
    if (!initializeSDL()) {
        return EXIT_FAILURE;
    }

    loadSprites();

    SDL_Event event;
    bool quit = false;

    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            handleInput(event);
        }

        render();

        SDL_Delay(16); // Cap the frame rate to approximately 60 frames per second
    }

    cleanupSDL();
    return EXIT_SUCCESS;
}
