#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
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

typedef struct {
    int window_width;
    int window_height;
    int sound;
} Config;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
TTF_Font* font = NULL;  // Ajout de la variable pour la police
Sprite sprites[5];
int score = 0;

bool initializeSDL(Config config) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow("Cliquez sur les sprites!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, config.window_width, config.window_height, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        fprintf(stderr, "Erreur lors de la création de la fenêtre : %s\n", SDL_GetError());
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        fprintf(stderr, "Erreur lors de la création du rendu : %s\n", SDL_GetError());
        return false;
    }

    // Initialisation de SDL_ttf
    if (TTF_Init() == -1) {
        fprintf(stderr, "Erreur d'initialisation de SDL_ttf : %s\n", TTF_GetError());
        return false;
    }

    // Chargement de la police par défaut
    font = TTF_OpenFont("arial.ttf", 24);
    if (font == NULL) {
        fprintf(stderr, "Erreur lors du chargement de la police : %s\n", TTF_GetError());
        return false;
    }

    return true;
}

SDL_Texture* loadTexture(const char* path) {
    // ... (inchangé)
}

void loadSprites(Config config) {
    // ... (inchangé)
}

void cleanupSDL() {
    // ... (inchangé)
    TTF_CloseFont(font);  // Libération de la police
    TTF_Quit();           // Fermeture de SDL_ttf
}

void handleInput(SDL_Event event) {
    // ... (inchangé)
}

void render() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // Affichage du score en haut au milieu
    SDL_Color textColor = {0, 0, 0, 255};
    char scoreText[20];
    snprintf(scoreText, sizeof(scoreText), "Score : %d", score);

    SDL_Surface* scoreSurface = TTF_RenderText_Solid(font, scoreText, textColor);
    SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);

    int scoreWidth, scoreHeight;
    SDL_QueryTexture(scoreTexture, NULL, NULL, &scoreWidth, &scoreHeight);
    SDL_Rect scoreRect = {(SCREEN_WIDTH - scoreWidth) / 2, 10, scoreWidth, scoreHeight};

    SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);

    SDL_FreeSurface(scoreSurface);
    SDL_DestroyTexture(scoreTexture);
    // Fin de l'affichage du score

    // Affichage des sprites
    for (int i = 0; i < 5; ++i) {
        SDL_Rect destRect = {sprites[i].x, sprites[i].y, SPRITE_WIDTH, SPRITE_HEIGHT};
        SDL_RenderCopy(renderer, sprites[i].texture, NULL, &destRect);
    }

    SDL_RenderPresent(renderer);
}

Config readConfig(const char* filename) {
    // ... (inchangé)
}

int main() {
    Config config = readConfig("conf/.conf");
    if(!config.window_height)
        return EXIT_FAILURE;
    if (!initializeSDL(config))
        return EXIT_FAILURE;

    loadSprites(config);

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
