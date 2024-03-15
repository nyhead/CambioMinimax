#pragma once
#include "Player.h"
#include "Deck.h"
#include "ViewInput.h"

#include "AI.h"
#include <memory>

class Game {
private:
	int _Players;
	int _playerTurn;
	int _gameTick;
	
	ViewInput* _front = nullptr;
	bool _isActive;
	Tracking _tracker;
public:
	Deck _deck;
	std::vector<Player*> _vPlayers;
	Game(ViewInput&);
	~Game();

	void createGame(); // template method
	void setNumberOfPlayers(int);
	
	int getNumberOfPlayers();

	void makeDeck();
	void setActive(bool);
	bool isActive();

	int gameLoop();
	Play findActualMove(std::vector<std::pair<Play, int>>& moves, Card* d);

	void playerAction(Player&);
	Play aiAction(Card *c);
	void dealCards();
	void createPlayers();
	std::pair<int, ShallowGame> determinize(ShallowGame world);
	std::pair<Play, int> simulateGame(const ShallowGame& world);
	std::vector<std::pair<Play, int>> runManySims(ShallowGame &world);
	
	ShallowGame copy();

	void move(Play& p, Card* drawn);
	int deduceWinner();
	int turn();

	Deck& getDeck();
};
