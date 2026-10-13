#ifndef SPELLSTACK_GAME_ACTION_H
#define SPELLSTACK_GAME_ACTION_H

#include <iostream>

// Polymorphic base for every action that can appear in the resolution line.
class GameAction {
public:
    virtual ~GameAction() = default;

    virtual void execute() = 0;
    virtual void displayDetails() const = 0;
};

#endif