#include <iostream>
#include <string>
#include <SDL2/SDL.h>

enum class Status {
    Idle,
    RunningLeft,
    RunningRight
};

class RPGCharacter {
public:
    RPGCharacter(const std::string& name, int health, int speed)
        : name(name), health(health), speed(speed), status(Status::Idle), x(0), y(0) {}

    void idle();
    void runLeft();
    void runRight();
    void update();
    void takeDamage(int damage);
    void move(int deltaX, int deltaY);
    void displayStatus() const;

private:
    std::string name;
    int health;
    int speed;
    Status status;
    int x, y;

    void displayIdleAnimation();
    void displayRunningLeftAnimation();
    void displayRunningRightAnimation();
};

void RPGCharacter::idle() {
    status = Status::Idle;
    displayIdleAnimation();
}

void RPGCharacter::runLeft() {
    status = Status::RunningLeft;
    displayRunningLeftAnimation();
}

void RPGCharacter::runRight() {
    status = Status::RunningRight;
    displayRunningRightAnimation();
}

void RPGCharacter::update() {
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

SDL_Texture* RPGCharacter::loadTexture(const std::string &file) {
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

    RPGCharacter character("Elias", 100, 5);
    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_LEFT:
                        character.runLeft();
                        character.move(-1, 0);
                        break;
                    case SDLK_RIGHT:
                        character.runRight();
                        character.move(1, 0);
                        break;
                }
            } else if (e.type == SDL_KEYUP) {
                if (e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_RIGHT) {
                    character.idle();
                }
            }
        }
        character.update();
    }

    SDL_Quit();

    return 0;
}
