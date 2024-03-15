#pragma once

#ifndef VIEWINPUT_HPP_
#define VIEWINPUT_HPP_

#include <iostream>
#include <unordered_map>
#include "Player.h"

#include <sstream>
#include <utility>

class ViewInput {
public:

    ViewInput() {
    }
    ~ViewInput() {}

    //void endMenu(int);
    //bool startMenu(sf::Text);
	void printCard(std::string c, std::string ending);
	void update(std::vector<Player*>& players, int length = 0);
    void agentView(std::vector<Card> (&hands)[2]);
    void clearScreen();
	bool InputDecideCallCambio();
	
    //this function returns a pair whose first and second elements are player's index and its card index respectively
	int InputChooseCard(int turn, std::string overrideMsg = "");
    bool reveal = true, aiView = true;
};

inline std::string getSuiteString(Suite suite) {
    switch (suite) {
    case HEARTS: return "H";
    case DIAMONDS: return "D";
    case SPADES: return "S";
    case CLUBS: return "C";
    default: return "Joker";
    }
}

inline std::string getRankString(int rank) {
    std::string num = "JOKER";
    int number = rank;
    if (number == 1) {
        num = "A";
    }
    else if (number < 11) {
        std::stringstream ss;
        ss << number;
        num = ss.str();
    }
    else if (number == 11) {
        num = "J";
    }
    else if (number == 12) {
        num = "Q";
    }
    else if (number == 13) {
        num = "K";
    }
    return num;
}
#endif