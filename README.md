- [1. Description](#1-description)
- [2. Rules](#2-rules)
- [3. Usage](#3-usage)
- [4. Manual](#4-manual)
  - [4.1. Start](#41-start)
  - [4.2. Game Loop](#42-game-loop)
  - [4.3. Calling Cambio / Finishing the Game](#43-calling-cambio--finishing-the-game)
- [5. Implementation](#5-implementation)


# 1. Description

This individual software project aims to implement an Artificial Intelligence (AI) opponent for a two-player Cambio card game, applied with a Minimax algorithm. The goal of this project is to challenge the human player by providing a competent AI opponent, simulating strategic decision-making based on the potential outcomes of each game state.

Cambio is a card game with a strong element of strategy and memory, where players aim to achieve the least points possible by the end of the game, using a mix of known and unknown information. Players need to keep track of their own cards and remember as much as possible about their opponent's cards to make optimal moves.

The main development task within this project is creating a Minimax algorithm to serve as the AI opponent's decision-making mechanism. The Minimax algorithm is a recursive algorithm used for decision-making in game theory and artificial intelligence for minimising the possible loss in a worst-case scenario.

# 2. Rules
Warning: parts of the game such as "Sticking a card", "Penalties" are not available since the game
runs in the terminal.

[Cambio rules](https://cambiocardgame.com/)

# 3. Usage
Please, make sure you have cmake installed before running the script.
```
python3 run.py
```
To reveal the cards or activate agent view, simply pass `reveal`, `agentv` arguments
```
python3 run.py reveal agentv
```
# 4. Manual

## 4.1. Start
- The game begins by dealing four cards to each player, arranged in a 2x2 grid.
- The human player can see their two bottom cards, while the AI (agent) knows its own two bottom cards.

## 4.2. Game Loop
- **Drawing a Card:** At the start of each turn, the game automatically draws a card from the deck for the current player.
- **Making a Decision:**
  - To discard the drawn card without using it, the player presses the **Enter** key.
  - To swap the drawn card with one of their own, the player types the position number (1-4) of the card they want to replace in their hand.
- **Special Cards:** If a discarded card is special, its ability is activated. The player may need to make additional choices based on the card's power:
  - Choose one of their own cards.
  - Choose an opponent's card.
  - Sequentially choose one of their own cards and then an opponent's card, depending on the specific ability triggered.

## 4.3. Calling Cambio / Finishing the Game
- **Calling Cambio:** At the start of their turn, a player can call "Cambio" to indicate they believe they have the lowest points and wish to end the game. To do this, the player types **5**.
- After Cambio is called, the game would proceed to reveal all cards and calculate the scores to determine the winner.

# 5. Implementation

in **Game.cpp**, and **AI.cpp**

```cpp

gameLoop() {
    while (game.isActive()) {
        if (turn == 0) {
            humanAction()
        } else if (turn == 1) {
            var shallowCopy = game.copy();
            List<Play> listPlays = game.runSimulations(shallowCopy) // runs n simulations of determinized mininimax
            Play p = findRealPlay(listPlays);
            if (p == Uninitialized) // if the simulation could not generate a "real" move 
            {
                p = followAiStrategy(tracking); // discard the card, and follow the strategy 
            }
            game.move(p); // applies the play
        }
    }
}
determinize() {
    generate hidden cards with uniform distribution
}

runSimulations(shallowCopy) {
    for i = 0 to N {
        (ShallowWorld determinized, int seed) = determinize(shallowCopy)
        (Play p, int score) = minimax(determinized, depth = 10, seed)
        detList.add(determinized)
    }
    return sort(detList) // ascending
}
findRealPlay() {
    for m :moves {
        if determized drawn card != real drawn card from deck
            continue
        
        based on play type : Replace, PeekOwn, PeekOther, BlindSwitch, PeekSwitch
        check if the determinized card matches the real one
    }
}
```