# SpellStack: Turn & Interrupt Resolver

SpellStack is a C++ turn-resolution program. Standard actions are added to
the back of a `std::deque`, while interrupt actions are added to the front.
This means a counter-spell resolves before the attack it targets.

## Files

- `GameAction.h` - Abstract base class for all actions.
- `Player.h` - Player name, health, damage, and healing behavior.
- `StandardAttack.h` - Action that deals damage to a player.
- `CounterSpell.h` - Interrupt action that targets an attack ID.
- `TurnEngine.h` / `TurnEngine.cpp` - Owns and resolves the action deque.
- `main.cpp` - Interactive command-line program.

## Requirements

- A C++17-compatible compiler
- On Windows, [MinGW-w64](https://www.mingw-w64.org/) with `g++`, or another
  compiler that supports C++17

## Build and Run on Windows

Open PowerShell in the `SpellStack` folder and compile the program:

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp TurnEngine.cpp -o SpellStack.exe
```

Run it with:

```powershell
.\SpellStack.exe
```

The project can also be compiled without creating a named executable:

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp TurnEngine.cpp
.\a.exe
```

## How to Play

When the program starts, choose an option from the menu:

```text
1. Queue a standard attack
2. Play a counter-spell interrupt
3. Resolve the next action
4. Resolve all actions
5. Show the resolution line
6. View player information
7. Set player information
0. Quit
```

### View or set player information

Choose `6` to display the player's name, current health, and defeated status.
Choose `7` to enter a new name and starting/current health value. Names may
contain spaces, and health must be zero or greater.

### Queue an attack

Choose `1`. A 10-damage attack targeting the player is added to the back of the
resolution line and receives a unique attack ID. If the attack resolves, the
player's health decreases by 10.

### Counter an attack

Choose `2`, then enter the ID of one of the pending attacks. The counter-spell
is placed at the front of the resolution line, so it resolves before the
targeted attack. When it resolves, that attack is removed and does not deal
damage.

If the entered ID does not belong to a pending attack, the counter-spell is not
queued.

### Resolve actions

- Choose `3` to resolve only the action at the front of the line.
- Choose `4` to resolve actions repeatedly until the line is empty.
- Choose `5` to inspect the current order. The front entry resolves first.

### Example

```text
1. Queue a standard attack
2. Play a counter-spell interrupt
5. Show the resolution line
```

After queueing an attack and countering attack ID `1`, the line will look like:

```text
1. CounterSpell (targets attack ID 1)
2. [Attack ID 1] StandardAttack (10 damage)
```

Selecting `4` resolves the counter-spell first. It cancels attack `1`, so the
attack is deleted without dealing damage.

## Empty Queue and Invalid Input

- Resolving an empty queue prints a message and does nothing.
- Attempting to counter when no attacks are pending is rejected.
- Non-numeric menu input is rejected and the menu is shown again.
- Invalid attack IDs do not create counter-spells.

## Cleanup

The `TurnEngine` owns every action accepted by `enqueue` or `interrupt`.
Actions are deleted when they resolve, are cancelled, or when the engine is
destroyed.
