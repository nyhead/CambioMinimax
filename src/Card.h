#pragma once
#include <string>
#include "stdint.h"
enum Suite { DIAMONDS, CLUBS, HEARTS, SPADES };
enum Color { BLACK, RED };

class Card {

public:
	Card();

	Card(int rank, Suite suite, Color color, bool hidden = true);
	int getRank() const;
	int getValue();
	int getCambioValue();
	Suite getSuite() const;
	Color getColor() const;
	std::string toString(bool reveal = false);
	bool isMagic();
	bool getHidden() const;
	void setHidden(bool hidden);
	static void swap(Card& a, Card& b) {
		Card temp = a; // or T temp(std::move(t1));
		a = b;
		b = temp;
	}
	Card& operator=(const Card& other);

	Card& operator=(Card& other);

protected:
	int _rank;
	int _value = INT32_MIN;
	Suite _suite;
	Color _color;
	bool _hidden;
	std::string getRankString();
	std::string getSuiteString();

};