#include "CounterSpell.h"
#include "Player.h"
#include "StandardAttack.h"
#include "TurnEngine.h"

#include <iostream>
#include <limits>
#include <stdexcept>

namespace {
void showMenu() {
    std::cout << "\nSpellStack: Turn & Interrupt Resolver\n"
              << "1. Queue a standard attack\n"
              << "2. Play a counter-spell interrupt\n"
              << "3. Resolve the next action\n"
              << "4. Resolve all actions\n"
              << "5. Show the resolution line\n"
              << "6. View player information\n"
              << "7. Set player information\n"
              << "0. Quit\n"
              << "Choice: ";
}
}

int main() {
    TurnEngine engine;
    Player player;
    bool running = true;

    while (running) {
        showMenu();
        int choice = 0;
        if (!(std::cin >> choice)) {
            if (std::cin.eof()) {
                break;
            }
            std::cout << "Please enter a number from 0 to 7.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
        case 1:
            engine.enqueue(new StandardAttack(player));
            std::cout << "Standard attack added to the back of the line.\n";
            break;
        case 2:
            if (!engine.hasPendingAttack()) {
                std::cout << "No pending attack is available to counter.\n";
                break;
            }
            std::cout << "Pending attacks (enter an ID to counter): ";
            for (const std::size_t attackId : engine.pendingAttackIds()) {
                std::cout << attackId << ' ';
            }
            std::cout << "\nAttack ID: ";
            {
                std::size_t targetId = 0;
                if (!(std::cin >> targetId)) {
                    std::cout << "Invalid attack ID. Counter-spell was not queued.\n";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    break;
                }
                if (!engine.isPendingAttack(targetId)) {
                    std::cout << "That ID does not belong to a pending attack. Counter-spell was not queued.\n";
                    break;
                }
                try {
                    engine.interrupt(new CounterSpell(targetId));
                    std::cout << "Counter-spell added to the front, targeting attack "
                              << targetId << ".\n";
                } catch (const std::invalid_argument& error) {
                    std::cout << error.what() << " Counter-spell was not queued.\n";
                }
            }
            break;
        case 3:
            if (!engine.resolveNext()) {
                std::cout << "There are no actions to resolve.\n";
            }
            break;
        case 4:
            if (engine.empty()) {
                std::cout << "There are no actions to resolve.\n";
            } else {
                engine.resolveAll();
            }
            break;
        case 5:
            engine.displayQueue();
            break;
        case 6:
            std::cout << "Player: " << player.getName()
                      << " | Health: " << player.getHealth();
            if (player.isDefeated()) {
                std::cout << " (defeated)";
            }
            std::cout << '\n';
            break;
        case 7:
            {
                std::string name;
                int health = 0;
                std::cout << "Player name: ";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::getline(std::cin, name);
                std::cout << "Player health: ";
                if (!(std::cin >> health)) {
                    std::cout << "Invalid health. Player information was not changed.\n";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    break;
                }
                try {
                    if (name.empty()) {
                        throw std::invalid_argument("Player name cannot be empty.");
                    }
                    if (health < 0) {
                        throw std::invalid_argument("Player health cannot be negative.");
                    }
                    player.setName(name);
                    player.setHealth(health);
                    std::cout << "Player information updated.\n";
                } catch (const std::invalid_argument& error) {
                    std::cout << error.what()
                              << " Player information was not changed.\n";
                }
            }
            break;
        case 0:
            running = false;
            break;
        default:
            std::cout << "Please enter a number from 0 to 7.\n";
            break;
        }
    }

    std::cout << "SpellStack closed.\n";
    return 0;
}
