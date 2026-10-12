#include "TurnEngine.h"
#include "CounterSpell.h"
#include "GameAction.h"
#include "StandardAttack.h"
#include <algorithm>
#include <iostream>
#include <memory>
#include <stdexcept>

TurnEngine::~TurnEngine() {
    clear();
}

void TurnEngine::enqueue(GameAction* action) {
    if (action == nullptr) {
        throw std::invalid_argument("Cannot enqueue a null action.");
    }
    if (std::find(resolutionLine.begin(), resolutionLine.end(), action) != resolutionLine.end()) {
        throw std::invalid_argument("This action is already owned by the TurnEngine.");
    }
    std::unique_ptr<GameAction> owner(action);
    resolutionLine.push_back(action);
    if (dynamic_cast<StandardAttack*>(action) != nullptr) {
        try {
            attackIds.emplace(action, nextAttackId);
        } catch (...) {
            resolutionLine.pop_back();
            throw;
        }
        ++nextAttackId;
    }
    owner.release();
}

void TurnEngine::interrupt(GameAction* action) {
    if (action == nullptr) {
        throw std::invalid_argument("Cannot enqueue a null interrupt.");
    }
    if (std::find(resolutionLine.begin(), resolutionLine.end(), action) != resolutionLine.end()) {
        throw std::invalid_argument("This action is already owned by the TurnEngine.");
    }
    std::unique_ptr<GameAction> owner(action);
    const auto* counter = dynamic_cast<const CounterSpell*>(action);
    if (counter == nullptr || !isPendingAttack(counter->getTargetAttackId())) {
        throw std::invalid_argument("CounterSpell must target a pending StandardAttack.");
    }
    resolutionLine.push_front(action);
    owner.release();
}

bool TurnEngine::hasPendingAttack() const {
    for (GameAction* action : resolutionLine) {
        if (dynamic_cast<const StandardAttack*>(action) != nullptr) {
            return true;
        }
    }
    return false;
}

bool TurnEngine::isPendingAttack(std::size_t attackId) const {
    for (const auto& entry : attackIds) {
        if (entry.second == attackId &&
            dynamic_cast<const StandardAttack*>(entry.first) != nullptr) {
            return true;
        }
    }
    return false;
}

std::vector<std::size_t> TurnEngine::pendingAttackIds() const {
    std::vector<std::size_t> ids;
    for (const GameAction* action : resolutionLine) {
        if (dynamic_cast<const StandardAttack*>(action) != nullptr) {
            const auto found = attackIds.find(action);
            if (found != attackIds.end()) {
                ids.push_back(found->second);
            }
        }
    }
    return ids;
}

bool TurnEngine::cancelAttack(std::size_t attackId) {
    for (auto position = resolutionLine.begin(); position != resolutionLine.end(); ++position) {
        GameAction* action = *position;
        const auto found = attackIds.find(action);
        if (found != attackIds.end() && found->second == attackId &&
            dynamic_cast<StandardAttack*>(action) != nullptr) {
            resolutionLine.erase(position);
            attackIds.erase(found);
            delete action;
            return true;
        }
    }
    return false;
}

bool TurnEngine::resolveNext() {
    if (resolutionLine.empty()) {
        return false;
    }

    // Hold the removed pointer immediately so it is deleted even if execute throws
    std::unique_ptr<GameAction> action(resolutionLine.front());
    resolutionLine.pop_front();
    if (dynamic_cast<StandardAttack*>(action.get()) != nullptr) {
        attackIds.erase(action.get());
    }
    const auto* counter = dynamic_cast<const CounterSpell*>(action.get());
    const std::size_t targetId = counter == nullptr ? 0 : counter->getTargetAttackId();
    action->execute();
    if (counter != nullptr) {
        cancelAttack(targetId);
    }
    return true;
}

void TurnEngine::resolveAll() {
    while (resolveNext()) {
    }
}

void TurnEngine::displayQueue() const {
    if (resolutionLine.empty()) {
        std::cout << "Resolution line is empty.\n";
        return;
    }

    std::cout << "Resolution line (front resolves first):\n";
    std::size_t position = 1;
    for (GameAction* action : resolutionLine) {
        std::cout << "  " << position++ << ". ";
        const auto attack = attackIds.find(action);
        if (attack != attackIds.end()) {
            std::cout << "[Attack ID " << attack->second << "] ";
        }
        action->displayDetails();
        std::cout << '\n';
    }
}

bool TurnEngine::empty() const noexcept {
    return resolutionLine.empty();
}

std::size_t TurnEngine::size() const noexcept {
    return resolutionLine.size();
}

void TurnEngine::clear() noexcept {
    for (GameAction* action : resolutionLine) {
        delete action;
    }
    resolutionLine.clear();
    attackIds.clear();
}
