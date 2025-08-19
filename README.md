# Marble Madness

Marble Madness is a C++ game project developed for UCLA's CS32 (Winter
2024). The objective of the game is to navigate through robot-infested
mazes, collect all crystals, and reach the exit to progress through
levels. The project emphasizes object-oriented programming, inheritance,
and real-time game mechanics.

------------------------------------------------------------------------

## 🎮 Gameplay

-   The player begins with **3 lives** and controls an avatar (Indiana
    Jones-style) using the arrow keys (or WASD / numeric keypad).\
-   The maze consists of **walls, marbles, pits, robots, factories,
    crystals, goodies, and exits**.\
-   The player must collect all **blue crystals** to reveal the exit.\
-   Enemies include:
    -   **RageBots**: Horizontal/Vertical robots that patrol and fire
        peas when the player is in sight.\
    -   **ThiefBots**: Wander the maze, stealing goodies if possible.\
    -   **Mean ThiefBots**: Stronger variants that also attack the
        player.\
-   Goodies provide benefits such as:
    -   Extra Life (+1 life)
    -   Restore Health (full heal)
    -   Ammo (+20 peas)

------------------------------------------------------------------------

## 🛠️ Technical Details

-   Written in **C++** using an object-oriented design.\
-   Inherits from the provided `GameWorld` and `GraphObject` classes.\
-   Key classes implemented:
    -   `StudentWorld` → manages the game world, actors, and level
        logic.\
    -   `Player`, `RageBot`, `ThiefBot`, `Marble`, `Pit`, `Wall`,
        `Crystal`, `Exit`, and goodies.\
-   Game loop divided into **ticks (20 per second)** where every actor
    executes its `doSomething()` method.\
-   Uses **level data files** (`level00.txt`, `level01.txt`, etc.) to
    define each maze's layout.

------------------------------------------------------------------------

## 📂 Project Structure

-   `StudentWorld.cpp/h`: Manages all actors, initialization, and
    cleanup.\
-   `Actor` and subclasses: Define behavior of game entities.\
-   `Level.h`: Provided helper class for loading levels.\
-   `GameWorld.h` & `GraphObject.h`: Provided base framework.\
-   `GameConstants.h`: Constants (image IDs, sounds, etc.).

------------------------------------------------------------------------

## ▶️ How to Run

1.  Clone or download the project.\

2.  Ensure all **provided framework files** are included (`GameWorld`,
    `GraphObject`, etc.).\

3.  Place `levelXX.txt` files in the correct assets directory.\

4.  Compile with a C++17 compatible compiler. Example (macOS/Linux):

    ``` bash
    g++ -std=c++17 *.cpp -o MarbleMadness
    ./MarbleMadness
    ```
