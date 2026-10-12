#ifndef SPELLSTACK_TURN_ENGINE_H
#define SPELLSTACK_TURN_ENGINE_H

#include <cstddef>
#include <deque>
#include <map>
#include <vector>

class GameAction;

// Owns every non-null GameAction pointer accepted by enqueue/interrupt
class TurnEngine {
public:
    TurnEngine() = default;
    ~TurnEngine();

    TurnEngine(const TurnEngine&) = delete;
    TurnEngine& operator=(const TurnEngine&) = delete;

    // Standard actions join the end of the resolution line
    void enqueue(GameAction* action);

    // Interrupts join the front and therefore resolve next
    void interrupt(GameAction* action);

    // The engine assigns stable IDs to attacks as they are queued
    bool hasPendingAttack() const;
    bool isPendingAttack(std::size_t attackId) const;
    std::vector<std::size_t> pendingAttackIds() const;
    bool cancelAttack(std::size_t attackId);

    // Resolve and remove the next action; false means the line was empty
    bool resolveNext();
    void resolveAll();
    void displayQueue() const;
    bool empty() const noexcept;
    std::size_t size() const noexcept;

private:
    std::deque<GameAction*> resolutionLine;
    std::map<const GameAction*, std::size_t> attackIds;
    std::size_t nextAttackId = 1;
    void clear() noexcept;
};

#endif
