#include <iostream>
#include <string>

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
      std::cout << name << " is running left." << std::endl;
      break;
    case Status::RunningRight:
      std::cout << name << " is running right." << std::endl;
      break;
    }
}

void RPGCharacter::displayIdleAnimation() {
  //animate(charecter, status)
  std::cout << name << " is in idle animation." << std::endl;
}

void RPGCharacter::displayRunningLeftAnimation() {
  //animate(charecter, status)
  std::cout << name << " is in running left animation." << std::endl;
}

void RPGCharacter::displayRunningRightAnimation() {
  //animate(charecter, status)
  std::cout << name << " is in running right animation." << std::endl;
}

void RPGCharacter::takeDamage(int damage) {
    health -= damage;
    if (health < 0) health = 0;

    std::cout << name << " has been demoted " << damage << " socialpoints. Social status is now " << health << "." << std::endl;
}
