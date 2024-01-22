#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

#define SPRITE_WIDTH 50
#define SPRITE_HEIGHT 50
#define GAME_DURATION 30000

typedef struct {
    int x, y;
    SDL_Texture* texture;
} Digit;

typedef struct {
    char player_name[50];
    int score;
} ScoreEntry;

typedef struct {
    int x, y;
    SDL_Texture* texture;
    int score;
    Digit digits[3]; // Pour un score allant jusqu'à 999
} Sprite;

typedef struct {
    int window_width;
    int window_height;
    int sound;
} Config;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
Sprite duck; // Un seul canard
int score = 0;

bool initializeSDL(Config config) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow("Cliquez sur le canard!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, config.window_width, config.window_height, SDL_WINDOW_SHOWN);
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

void loadDigits(Digit digits[10]) {
    for (int i = 0; i < 10; ++i) {
        char filename[20];
        sprintf(filename, "assets/%d.bmp", i);
        digits[i].texture = loadTexture(filename);
    }
}

void loadDuck(Config *config) {
    duck.x = rand() % (config->window_width - SPRITE_WIDTH);
    duck.y = rand() % (config->window_height - SPRITE_HEIGHT);
    duck.texture = loadTexture("assets/duck.bmp");
    duck.score = 0;
    loadDigits(duck.digits);
}

void cleanupSDL() {
    SDL_DestroyTexture(duck.texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    FILE* scoreboardFile = fopen("scores.txt", "a");
    if (scoreboardFile != NULL) {
        ScoreEntry entry;
        printf("Entrez votre nom : ");
        scanf("%s", entry.player_name);
        entry.score = duck.score;
        fprintf(scoreboardFile, "%s %d\n", entry.player_name, entry.score);
        fclose(scoreboardFile);
    } else {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier des scores.\n");
    }
}

void handleInput(SDL_Event event, Config config) {
    if (event.type == SDL_QUIT) {
        exit(0);
    } else if (event.type == SDL_MOUSEBUTTONDOWN) {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        if (mouseX >= duck.x && mouseX <= duck.x + SPRITE_WIDTH &&
            mouseY >= duck.y && mouseY <= duck.y + SPRITE_HEIGHT) {
            // Clique sur le canard, incrémenter le score et déplacer le canard
            duck.score++;

            duck.x = rand() % (config.window_width - SPRITE_WIDTH);
            duck.y = rand() % (config.window_height - SPRITE_HEIGHT);
        }
    }
}

void render(Config *config) {
    for (int i = 0; i < config->window_width; ++i) {
        // Calculer la composante rouge et bleue en fonction de la position sur l'axe x
        int red = (i * 255) / config->window_width;
        int blue = 255 - red;

        SDL_SetRenderDrawColor(renderer, red, 0, blue, 255);
        SDL_RenderDrawLine(renderer, i, 0, i, config->window_height);
    }

    // Afficher le canard
    SDL_Rect destRect = { duck.x, duck.y, SPRITE_WIDTH, SPRITE_HEIGHT };
    SDL_RenderCopy(renderer, duck.texture, NULL, &destRect);

    // Afficher le score avec les chiffres
    int score = duck.score;
    int digitWidth = 50;
    int digitHeight = 50;
    int x = config->window_width - digitWidth - 10; // Marge à droite de la fenêtre

    if (score == 0) {
        SDL_Rect digitRect = { x, 10, digitWidth, digitHeight };
        SDL_RenderCopy(renderer, duck.digits[0].texture, NULL, &digitRect);
    } else {
        int digitCount = 0;
        while (score > 0 && digitCount < 3) {
            int digit = score % 10;
            SDL_Rect digitRect = { x, 10, digitWidth, digitHeight };
            SDL_RenderCopy(renderer, duck.digits[digit].texture, NULL, &digitRect);
            x -= digitWidth;
            score /= 10;
            digitCount++;
        }
    }

    SDL_RenderPresent(renderer);
}


Config readConfig(const char* filename) {
    Config config;
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier de configuration.\n");
        exit(EXIT_FAILURE);
    }

    // Lecture du fichier de configuration
    while (fscanf(file, "%*[^:]:%d\n", &config.window_width) != 1);
    while (fscanf(file, "%*[^:]:%d\n", &config.window_height) != 1);
    while (fscanf(file, "%*[^:]:%d\n", &config.sound) != 1);

    fclose(file);

    return config;
}

void displayTopScores() {
    printf("Meilleurs scores :\n");

    FILE* scoreboardFile = fopen("scores.txt", "r");
    if (scoreboardFile != NULL) {
        ScoreEntry scores[5];
        for (int i = 0; i < 5; ++i) {
            if (fscanf(scoreboardFile, "%s %d", scores[i].player_name, &scores[i].score) != 2) {
                break; // Arrête la lecture si le fichier n'a plus de scores
            }
        }

        fclose(scoreboardFile);

        // Tri des scores
        for (int i = 0; i < 5; ++i) {
            for (int j = i + 1; j < 5; ++j) {
                if (scores[j].score > scores[i].score) {
                    ScoreEntry temp = scores[i];
                    scores[i] = scores[j];
                    scores[j] = temp;
                }
            }
        }

        // Affichage des scores
        for (int i = 0; i < 5; ++i) {
            printf("%d. %s - %d\n", i + 1, scores[i].player_name, scores[i].score);
        }
    } else {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier des scores.\n");
    }
}

int main() {
    Config config = readConfig("conf/.conf");
    if (!config.window_height)
        return EXIT_FAILURE;
    if (!initializeSDL(config))
        return EXIT_FAILURE;

    loadDuck(&config);

    Uint32 startTime = SDL_GetTicks();
    SDL_Event event;
    bool quit = false;

    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            handleInput(event, config);
        }

        render(&config);

        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - startTime >= GAME_DURATION) {
            printf("Temps écoulé! Score final : %d\n", duck.score);
            quit = true;
        } else {
            SDL_Delay(16); // Cap the frame rate to approximately 60 frames per second
        }
    }

    cleanupSDL();
    displayTopScores();
    return EXIT_SUCCESS;
}
