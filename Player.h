#ifndef SPELLSTACK_PLAYER_H
#define SPELLSTACK_PLAYER_H

#include <stdexcept>
#include <string>
#include <utility>

class Player {
public:
    explicit Player(std::string name = "Player", int startingHealth = 100)
        : name(std::move(name)), health(startingHealth) {
        if (this->name.empty()) {
            throw std::invalid_argument("Player name cannot be empty.");
        }
        if (startingHealth < 0) {
            throw std::invalid_argument("Player health cannot be negative.");
        }
    }

    const std::string& getName() const noexcept {
        return name;
    }

    int getHealth() const noexcept {
        return health;
    }

    void setName(std::string newName) {
        if (newName.empty()) {
            throw std::invalid_argument("Player name cannot be empty.");
        }
        name = std::move(newName);
    }

    void setHealth(int newHealth) {
        if (newHealth < 0) {
            throw std::invalid_argument("Player health cannot be negative.");
        }
        health = newHealth;
    }

    bool isDefeated() const noexcept {
        return health == 0;
    }

    void takeDamage(int amount) {
        if (amount < 0) {
            throw std::invalid_argument("Damage cannot be negative.");
        }
        health = amount >= health ? 0 : health - amount;
    }

    void heal(int amount) {
        if (amount < 0) {
            throw std::invalid_argument("Healing cannot be negative.");
        }
        health += amount;
    }

private:
    std::string name;
    int health;
};

#endif
