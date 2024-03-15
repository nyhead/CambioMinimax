#pragma once
#include "Hand.h"

class Deck : public Hand {
public:
	// ~Deck();
	Deck();
	// Deck()
	int deal(Hand& hand);
	void makeDeck();
	void shuffleDeck();

	// Deck copyDeck();
	// Deck& operator=(Deck&);
};
