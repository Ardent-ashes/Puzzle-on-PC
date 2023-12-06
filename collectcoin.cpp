#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int COIN_SIZE = 10;
const int PLAYER_SIZE = 50;
const int ENEMY_SIZE = 30;
const int MAX_COINS = 15;
int remainingCoins = MAX_COINS;  
const float MOVE_DISTANCE = 5.0f;  // Distance to move on each key press
const float RANDOM_MOVE_INTERVAL = 5;
const float RANDOM_DIRECTION_CHANGE_INTERVAL = .005; 

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* backgroundTexture = nullptr;
SDL_Texture* blockTexture = nullptr;

struct Coin {
    int x, y;
};

struct Player {
    int x, y;
};

struct Obstacle {
    int x, y, width, height;
};


std::vector<Obstacle>obstacles;
std::vector<Coin> coins;
Player player;
Player randomp;

bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("Coin Collector", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}
bool loadBackgroundImage() {
    SDL_Surface* backgroundSurface = IMG_Load("prac.jpg");
    if (backgroundSurface == nullptr) {
        printf("Failed to load background image! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);

    if (backgroundTexture == nullptr) {
        printf("Failed to create background texture! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    return true;
}

void close() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void spawnCoins() {
    for (int i = 0; i < MAX_COINS; i++) {
        Coin coin;
        coin.x = rand() % (SCREEN_WIDTH - COIN_SIZE);
        coin.y = rand() % (SCREEN_HEIGHT - COIN_SIZE);
        coins.push_back(coin);
    }
}
const int STEP_SIZE = 5;


bool checkCollision(int x, int y) {
    for (const auto& obstacle : obstacles) {
        if (x < obstacle.x + obstacle.width &&
            x + PLAYER_SIZE > obstacle.x &&
            y < obstacle.y + obstacle.height &&
            y + PLAYER_SIZE > obstacle.y) {
            return true;  // Collision detected
        }
    }
    return false;  // No collision
}
void handleEvents(bool& quit) {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            quit = true;
        }
        if (e.type == SDL_KEYDOWN) {
                int prevBlockX = player.x;
                int prevBlockY = player.y;
            switch (e.key.keysym.sym) {
                
                case SDLK_UP:
                    if (player.y > 0) {
                        player.y -= STEP_SIZE;
                    }
                    break;
                case SDLK_DOWN:
                    if (player.y < SCREEN_HEIGHT - PLAYER_SIZE) {
                        player.y += STEP_SIZE;
                    }
                    break;
                case SDLK_LEFT:
                    if (player.x > 0) {
                        player.x -= STEP_SIZE;
                    }
                    break;
                case SDLK_RIGHT:
                    if (player.x < SCREEN_WIDTH - PLAYER_SIZE) {
                        player.x += STEP_SIZE;
                    }
                    break;
            }

            if (checkCollision(player.x, player.y)) {
                player.x = prevBlockX;
                 player.y = prevBlockY;
               // collisionOccurred = true;  // Set collision flag
            }
        }
    }
}

bool checkCoinCollision(const Coin& coin) {
    if (player.x < coin.x + COIN_SIZE &&
        player.x + PLAYER_SIZE > coin.x &&
        player.y < coin.y + COIN_SIZE &&
        player.y + PLAYER_SIZE > coin.y) {
        remainingCoins--;  
        return true;
    }
    return false;
}
void drawObstacles(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);  // Gray color for obstacles
    for (const auto& obstacle : obstacles) {
        SDL_Rect obstacleRect = { obstacle.x, obstacle.y, obstacle.width, obstacle.height };
        SDL_RenderFillRect(renderer, &obstacleRect);
    }
}
void moveRandomBlock(int& blockX, int& blockY, Uint32& lastMoveTime, Uint32& lastDirectionChangeTime) {
    static int randomDirection = rand() % 4;  // Initial random direction
    Uint32 currentTime = SDL_GetTicks();

    // Check if it's time to change direction
    if (currentTime - lastDirectionChangeTime >= RANDOM_DIRECTION_CHANGE_INTERVAL) {
        int newDirection = rand() % 4;

        // Change direction only if the new direction is not towards a structure
        switch (newDirection) {
        case 0:  // Move up
            if (blockY > 0 && !checkCollision(blockX, blockY - MOVE_DISTANCE)) {
                blockY -= static_cast<int>(MOVE_DISTANCE);
                randomDirection = newDirection;
            }
            break;
        case 1:  // Move down
            if (blockY < SCREEN_HEIGHT - PLAYER_SIZE && !checkCollision(blockX, blockY + MOVE_DISTANCE)) {
                blockY += static_cast<int>(MOVE_DISTANCE);
                randomDirection = newDirection;
            }
            break;
        case 2:  // Move left
            if (blockX > 0 && !checkCollision(blockX - MOVE_DISTANCE, blockY)) {
                blockX -= static_cast<int>(MOVE_DISTANCE);
                randomDirection = newDirection;
            }
            break;
        case 3:  // Move right
            if (blockX < SCREEN_WIDTH - PLAYER_SIZE && !checkCollision(blockX + MOVE_DISTANCE, blockY)) {
                blockX += static_cast<int>(MOVE_DISTANCE);
                randomDirection = newDirection;
            }
            break;
        }

        lastDirectionChangeTime = currentTime;
    }

    // Check if it's time to move the block
    if (currentTime - lastMoveTime >= RANDOM_MOVE_INTERVAL) {
        // Move in the current direction if not colliding with a structure
        switch (randomDirection) {
        case 0:  // Move up
            if (blockY > 0 && !checkCollision(blockX, blockY - MOVE_DISTANCE)) {
                blockY -= static_cast<int>(MOVE_DISTANCE);
            }
            break;
        case 1:  // Move down
            if (blockY < SCREEN_HEIGHT - PLAYER_SIZE && !checkCollision(blockX, blockY + MOVE_DISTANCE)) {
                blockY += static_cast<int>(MOVE_DISTANCE);
            }
            break;
        case 2:  // Move left
            if (blockX > 0 && !checkCollision(blockX - MOVE_DISTANCE, blockY)) {
                blockX -= static_cast<int>(MOVE_DISTANCE);
            }
            break;
        case 3:  // Move right
            if (blockX < SCREEN_WIDTH - PLAYER_SIZE && !checkCollision(blockX + MOVE_DISTANCE, blockY)) {
                blockX += static_cast<int>(MOVE_DISTANCE);
            }
            break;
        }

        lastMoveTime = currentTime;
    }
}

void game()
{

     if(!loadBackgroundImage()) {
        printf("Failed to load images!\n");
     }
    randomp.x = 500; //(SCREEN_WIDTH - PLAYER_SIZE) / 4;  // Initial position for the random block
    randomp.y = 500; //(SCREEN_HEIGHT - PLAYER_SIZE) / 4;

    Uint32 lastRandomMoveTime = SDL_GetTicks();
    Uint32 lastRandomDirectionChangeTime = SDL_GetTicks();
    int randomDirection = rand() % 4;  // Initial random direction

     obstacles.push_back({ 0, 70, 150, 20 }); //horizontal
     obstacles.push_back({ 220, 70, 150, 20 });
     obstacles.push_back({590 , 70, 20, 200 }); //vertical
       obstacles.push_back({450 , 0, 20, 80 });
     obstacles.push_back({ 500, 160, 20, 200 });
     obstacles.push_back({ 65, 160, 350, 20 });
     obstacles.push_back({ 400, 250, 20, 65 });
     obstacles.push_back({ 300, 160, 20, 100 });
     obstacles.push_back({ 300, 245, 135, 20 });
     obstacles.push_back({ 80, 250, 20, 100 });
     obstacles.push_back({ 500, 250, 200, 20 });
     obstacles.push_back({ 50, 250, 180, 20 });
     obstacles.push_back({ 0, 420, 200, 20 });
     obstacles.push_back({ 200, 400, 20, 80 });
     obstacles.push_back({ 700, 0, 20, 50 });
     obstacles.push_back({ 700, 120, 20, 100 });
     obstacles.push_back({ 700, 320, 100, 20 });
     obstacles.push_back({ 100, 420, 20, 100 });
     obstacles.push_back({300 , 500, 20, 200 });
     obstacles.push_back({290 , 380, 100, 20 });
     obstacles.push_back({310 , 350, 20, 30 });
     obstacles.push_back({310 , 350, 20, 30 });
     obstacles.push_back({600 , 370, 20, 70 });
     obstacles.push_back({600 , 440, 70, 20 });
     obstacles.push_back({670 , 440, 20, 70 });
     obstacles.push_back({670 , 500, 50, 20 });
     obstacles.push_back({400 , 470, 100, 20 });
     obstacles.push_back({400 , 470, 20, 40 });
     obstacles.push_back({550 , 570, 20, 45 });


    
    spawnCoins();
    bool quit = false;

    srand(time(NULL));

    while (!quit) {
        handleEvents(quit);
      moveRandomBlock(randomp.x, randomp.y, lastRandomMoveTime, lastRandomDirectionChangeTime);
        if (randomp.x < player.x + PLAYER_SIZE &&
            randomp.x + ENEMY_SIZE > player.x &&
            randomp.y < player.y + PLAYER_SIZE &&
            randomp.y + ENEMY_SIZE > player.y) {
            printf("You lost\n");
            //close();
            return ;
        }
        
        SDL_RenderClear(renderer);
      SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
      drawObstacles(renderer);
        

       
        for (auto it = coins.begin(); it != coins.end();) {
            if (checkCoinCollision(*it)) {
                it = coins.erase(it);
            }
            else {
                SDL_Rect coinRect = { it->x, it->y, COIN_SIZE, COIN_SIZE };
                SDL_SetRenderDrawColor(renderer, 255, 223, 186, 255); // Color of the coins
                SDL_RenderFillRect(renderer, &coinRect);
                it++;
            }
        }

        if (remainingCoins == 0) {
       printf("You win\n");
       return;
        }
         SDL_Rect playerRect = { player.x, player.y, PLAYER_SIZE, PLAYER_SIZE };
        SDL_SetRenderDrawColor(renderer, 0, 0, 153, 255); // Color of the player
        SDL_RenderFillRect(renderer, &playerRect);
         
        SDL_Rect rRect = { randomp.x, randomp.y, ENEMY_SIZE, ENEMY_SIZE };
        SDL_SetRenderDrawColor(renderer, 153,0, 0, 255); 
        SDL_RenderFillRect(renderer, &rRect);

        
        SDL_RenderPresent(renderer);
         }
}

