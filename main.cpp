#include <iostream>
#include <string>
#include <map>
#include <SDL2/SDL.h>
#include "SDL2_image-2.8.2/include/SDL_image.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int SCENE_WIDTH = 800;
const int SCENE_HEIGHT = 600;

enum class Status {
    Idle,
    RunningLeft,
    RunningRight
};

class World {
public:
    World(SDL_Renderer* renderer) : renderer(renderer), currentSceneX(0), currentSceneY(0) {
        loadWorldMap();
    }

    ~World() {
        for (auto& texture : sceneTextures) {
            SDL_DestroyTexture(texture.second);
        }
    }

    void loadWorldMap() {
        worldMap[std::make_pair(0, 0)] = "assets/scenes/OfficeBuilding.png";
        worldMap[std::make_pair(1, 0)] = "assets/scenes/OfficeBuilding.png";
        worldMap[std::make_pair(0, 1)] = "assets/scenes/OfficeBuilding.png";
        worldMap[std::make_pair(1, 1)] = "assets/scenes/OfficeBuilding.png";
    }

    void loadScene(int x, int y) {
        std::pair<int, int> coord(x, y);
        if (sceneTextures.find(coord) == sceneTextures.end()) {
            std::string scenePath = worldMap[coord];
            SDL_Texture* newTexture = IMG_LoadTexture(renderer, scenePath.c_str());
            if (newTexture == nullptr) {
                std::cerr << "Failed to load texture: " << SDL_GetError() << std::endl;
                return;
            }
            sceneTextures[coord] = newTexture;
        }
    }

    void renderScene(int playerX, int playerY) {
        int sceneX = playerX / SCENE_WIDTH;
        int sceneY = playerY / SCENE_HEIGHT;

        if (sceneX != currentSceneX || sceneY != currentSceneY) {
            loadScene(sceneX, sceneY);
            loadScene(sceneX + 1, sceneY);
            loadScene(sceneX, sceneY + 1);
            loadScene(sceneX + 1, sceneY + 1);
            currentSceneX = sceneX;
            currentSceneY = sceneY;
        }

        SDL_RenderClear(renderer);

        // Render the current scene and adjacent scenes
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 2; ++j) {
                SDL_Rect destRect = { i * SCENE_WIDTH - (playerX % SCENE_WIDTH),
                                      j * SCENE_HEIGHT - (playerY % SCENE_HEIGHT), 
                                      SCENE_WIDTH, SCENE_HEIGHT };
                SDL_RenderCopy(renderer, sceneTextures[{sceneX + i, sceneY + j}], nullptr, &destRect);
            }
        }

        SDL_RenderPresent(renderer);
    }

private:
    SDL_Renderer* renderer;
    int currentSceneX, currentSceneY;
    std::map<std::pair<int, int>, std::string> worldMap;
    std::map<std::pair<int, int>, SDL_Texture*> sceneTextures;
};

class RPGCharacter {
public:
    RPGCharacter(const std::string& name, int health, int speed, SDL_Renderer* renderer)
        : name(name), health(health), speed(speed), status(Status::Idle), x(0), y(0), renderer(renderer), frame(0) {
        loadTextures();
    }

    ~RPGCharacter() {
        for (auto texture : idleTextures) {
            SDL_DestroyTexture(texture);
        }
        for (auto texture : runningLeftTextures) {
            SDL_DestroyTexture(texture);
        }
        for (auto texture : runningRightTextures) {
            SDL_DestroyTexture(texture);
        }
    }

    void idle();
    void runLeft();
    void runRight();
    void update();
    void takeDamage(int damage);
    void move(int deltaX, int deltaY);
    void displayStatus() const;
    void render();

private:
    std::string name;
    int health;
    int speed;
    Status status;
    int x, y;
    SDL_Renderer* renderer;
    int frame;

    SDL_Texture* idleTextures[4];
    SDL_Texture* runningLeftTextures[4];
    SDL_Texture* runningRightTextures[4];

    SDL_Texture* loadTexture(const std::string& file);
    void loadTextures();
    void displayIdleAnimation();
    void displayRunningLeftAnimation();
    void displayRunningRightAnimation();
};

void RPGCharacter::idle() {
    status = Status::Idle;
    frame = 0;
}

void RPGCharacter::runLeft() {
    status = Status::RunningLeft;
    frame = 0;
}

void RPGCharacter::runRight() {
    status = Status::RunningRight;
    frame = 0;
}

void RPGCharacter::update() {
    frame = (frame + 1) % 4;
    switch (status) {
        case Status::Idle:
            std::cout << name << " is standing still." << std::endl;
            break;
        case Status::RunningLeft:
            std::cout << name << " is running left at speed " << speed << "." << std::endl;
            break;
        case Status::RunningRight:
            std::cout << name << " is running right at speed " << speed << "." << std::endl;
            break;
    }
}

void RPGCharacter::takeDamage(int damage) {
    health -= damage;
    if (health < 0) health = 0;
    std::cout << name << " takes " << damage << " damage. Health is now " << health << "." << std::endl;
}

void RPGCharacter::move(int deltaX, int deltaY) {
    x += deltaX * speed;
    y += deltaY * speed;
    std::cout << name << " moves to position (" << x << ", " << y << ")." << std::endl;
}

void RPGCharacter::displayStatus() const {
    std::cout << "Name: " << name << std::endl;
    std::cout << "Health: " << health << std::endl;
    std::cout << "Speed: " << speed << std::endl;
    std::cout << "Position: (" << x << ", " << y << ")" << std::endl;
}

void RPGCharacter::render() {
    SDL_Rect destRect = { x, y, 64, 64 };
    SDL_Texture* currentTexture = nullptr;
    if (status == Status::Idle) {
        currentTexture = idleTextures[frame];
    } else if (status == Status::RunningLeft) {
        currentTexture = runningLeftTextures[frame];
    } else if (status == Status::RunningRight) {
        currentTexture = runningRightTextures[frame];
    }
    SDL_RenderCopy(renderer, currentTexture, nullptr, &destRect);
}

SDL_Texture* RPGCharacter::loadTexture(const std::string& file) {
    SDL_Texture* newTexture = nullptr;
    SDL_Surface* loadedSurface = IMG_Load(file.c_str());
    if (loadedSurface == nullptr) {
        std::cerr << "Unable to load image " << file << "! SDL_image Error: " << IMG_GetError() << std::endl;
    } else {
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (newTexture == nullptr) {
            std::cerr << "Unable to create texture from " << file << "! SDL Error: " << SDL_GetError() << std::endl;
        }
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}

void RPGCharacter::loadTextures() {
    std::string basePath = "assets/" + name + "/";

    idleTextures[0] = loadTexture(basePath + "idle_0.png");
    idleTextures[1] = loadTexture(basePath + "idle_1.png");
    idleTextures[2] = loadTexture(basePath + "idle_2.png");
    idleTextures[3] = loadTexture(basePath + "idle_3.png");

    runningLeftTextures[0] = loadTexture(basePath + "run_left_0.png");
    runningLeftTextures[1] = loadTexture(basePath + "run_left_1.png");
    runningLeftTextures[2] = loadTexture(basePath + "run_left_2.png");
    runningLeftTextures[3] = loadTexture(basePath + "run_left_3.png");

    runningRightTextures[0] = loadTexture(basePath + "run_right_0.png");
    runningRightTextures[1] = loadTexture(basePath + "run_right_1.png");
    runningRightTextures[2] = loadTexture(basePath + "run_right_2.png");
    runningRightTextures[3] = loadTexture(basePath + "run_right_3.png");
}

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("RPG Character", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        SDL_DestroyWindow(window);
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    RPGCharacter character("Elias", 100, 5, renderer);
    World world(renderer);

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        world.renderScene(character.getX(), character.getY());
        character.update();
        character.render();
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
