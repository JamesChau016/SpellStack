#ifndef SPELLSTACK_STANDARD_ATTACK_H
#define SPELLSTACK_STANDARD_ATTACK_H

#include "GameAction.h"
#include "Player.h"

#include <iostream>
#include <stdexcept>

class StandardAttack : public GameAction {
public:
    explicit StandardAttack(Player* target = nullptr, int damage = 10)
        : target(target), damage(damage) {
        if (damage < 0) {
            throw std::invalid_argument("Attack damage cannot be negative.");
        }
    }

    StandardAttack(Player& target, int damage = 10)
        : StandardAttack(&target, damage) {
    }

    void execute() override {
        if (target != nullptr) {
            target->takeDamage(damage);
        }
    }

    void displayDetails() const override {
        std::cout << "StandardAttack (" << damage << " damage";
        if (target != nullptr) {
            std::cout << " to " << target->getName();
        }
        std::cout << ')';
    }

    int getDamage() const noexcept {
        return damage;
    }

    Player* getTarget() const noexcept {
        return target;
    }

private:
    Player* target;
    int damage;
};

#endif
