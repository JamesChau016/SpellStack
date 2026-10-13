#ifndef SPELLSTACK_COUNTER_SPELL_H
#define SPELLSTACK_COUNTER_SPELL_H

#include "GameAction.h"

#include <cstddef>
#include <iostream>

class CounterSpell : public GameAction {
public:
    explicit CounterSpell(std::size_t targetAttackId)
        : targetAttackId(targetAttackId) {
    }

    void execute() override {
        // TurnEngine removes the selected attack immediately after this action resolves.
    }

    void displayDetails() const override {
        std::cout << "CounterSpell (targets attack ID " << targetAttackId << ')';
    }

    std::size_t getTargetAttackId() const noexcept {
        return targetAttackId;
    }

private:
    std::size_t targetAttackId;
};

#endif
