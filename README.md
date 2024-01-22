Documentation du code de TrainAim

Ce document fournit une documentation pour le code en C, qui implémente un jeu simple où le joueur doit cliquer sur un canard pour marquer des points.

Structures
    
    Digit

La structure Digit représente un chiffre (0-9) dans le jeu. Chaque chiffre a une position (x, y) et une texture SDL associée.

c

typedef struct {
    int x, y;
    SDL_Texture* texture;
} Digit;

    
    ScoreEntry

La structure ScoreEntry représente une entrée de score avec le nom du joueur et le score obtenu.

c

typedef struct {
    char player_name[50];
    int score;
} ScoreEntry;

    
    Sprite

La structure Sprite représente le canard dans le jeu. Elle contient sa position (x, y), une texture SDL, son score, et un tableau de chiffres (digits) pour afficher le score.

c

typedef struct {
    int x, y;
    SDL_Texture* texture;
    int score;
    Digit digits[3]; // Pour un score allant jusqu'à 999
} Sprite;


    Config

La structure Config stocke les paramètres de configuration du jeu, tels que la largeur de la fenêtre, la hauteur de la fenêtre et le son.

c

typedef struct {
    int window_width;
    int window_height;
    int sound;
} Config;


Fonctions Principales
    
    initializeSDL

Cette fonction initialise la bibliothèque SDL, crée une fenêtre et un rendu. Elle renvoie un booléen indiquant si l'initialisation a réussi.

    loadTexture

Cette fonction charge une texture à partir d'un fichier BMP.

    loadDigits

Cette fonction charge les textures des chiffres de 0 à 9 dans le tableau digits.

    loadDuck

Cette fonction initialise les propriétés du canard, y compris sa position, sa texture et son score.
    
    cleanupSDL

Cette fonction nettoie la mémoire utilisée par la SDL, détruit la texture du canard et enregistre le score du joueur dans un fichier.

    handleInput

Cette fonction gère les événements d'entrée, tels que la fermeture de la fenêtre et les clics de souris sur le canard.

    render

Cette fonction effectue le rendu du jeu. Elle affiche un dégradé de couleur de bleu à rouge en arrière-plan, le canard à sa position actuelle et le score du joueur.
    
    readConfig

Cette fonction lit les paramètres de configuration à partir d'un fichier et renvoie une structure Config.
    
    displayTopScores

Cette fonction affiche les cinq meilleurs scores en lisant à partir d'un fichier de scores.

    main

La fonction principale du programme. Elle initialise la SDL, charge le canard, gère les entrées, rend le jeu, et affiche les scores.


Utilisation

    Assurez-vous d'avoir la bibliothèque SDL installée sur votre système.
    Compilez le code avec la commande: gcc src/main.c -o TrainAim $(sdl2-config --cflags --libs)
    Exécutez le programme généré.
    Pour jouer, vous disposez de 30 secondes, cliquez sur le plus de cibles possibles dans les 30 secondes.

Le joueur doit cliquer sur le canard pour marquer des points. Le jeu se termine après une durée prédéfinie, et le score final est affiché avec la possibilité d'entrer le nom du joueur pour l'enregistrement du score. Les cinq meilleurs scores sont également affichés à la fin du jeu.