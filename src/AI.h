#include "Deck.h"

enum Type {Uninitialized, Replace, PeekOwn, PeekOther, BlindSwitch, PeekSwitch, Call};

struct Play {
    Type type = Uninitialized;
	Card throwable;

	int replaceableID;
	Card replaceable;
	
	int peekableID;
	Card peekable;

	int enemySwitchID;
	Card enemyCard;

	int ownSwitchID;
	Card ownCard;
};


class Tracking {
	public:
	std::vector<Card> hands[2];
	//int countCards[14];
	void update(int turn, int idCard,Card* card);
	void initialize(std::vector<Card> player, std::vector<Card> ai);
};

class ShallowGame {
	public:
		int turn;
		int playerCount = 2;
		std::vector<Card> deck;
		Tracking tracking;
		bool isDeterminized = false;
		void throwPlay(Play&);
		// void replaceBehaviour(Card* card);
		int evaluateFeatures();
		void move(Play&);
		void unMove(Play&);
		std::vector<Play> possibleMoves(int seed);
};

// inline 
