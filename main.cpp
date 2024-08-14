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

void RPGCharacter::displayIdleAnimation() {
    std::cout << name << " is in idle animation." << std::endl;
}

void RPGCharacter::displayRunningLeftAnimation() {
    std::cout << name << " is in running left animation." << std::endl;
}

void RPGCharacter::displayRunningRightAnimation() {
    std::cout << name << " is in running right animation." << std::endl;
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
