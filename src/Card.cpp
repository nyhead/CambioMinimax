#include "Card.h"
#include <sstream>

Card::Card() {
  _rank = -1;
  _suite = SPADES;
  _hidden = true;
  _color = BLACK;
  _value = -1;
}
Card& Card::operator=(const Card& other) {
    if (this != &other) {
        _rank = other.getRank();
        _suite = other.getSuite();
        _color = other.getColor();
        _hidden = other.getHidden();
    }
    return *this;
}
	Card& Card::operator=(Card& other) {
		if (this != &other) {
			_rank = other.getRank();
			_suite = other.getSuite();
			_color = other.getColor();
			_hidden = other.getHidden();
			_value = other.getValue();
		}
    return *this;
	}



Card::Card(int rank, Suite suite, Color color, bool hidden){
  _rank = rank;
  _suite = suite;
  _hidden = hidden;
  _color = color;
  _value = getValue();
}

bool Card::isMagic() {
    if (
      (this->_rank >= 7 && this->_rank <= 12) ||
      (this->_rank == 13 &&
        this->_color == BLACK)
    ) {
      return true;
    }
    return false;
  }

int Card::getRank() const {
  return _rank;
}

int Card::getValue() {
    if (_value == INT32_MIN && !_hidden) {
        switch (getRank())
        {
        case 1: //Ace
            return 1;
        case 13: //Red kinh
            return getColor() == RED ? -1 : 10;
        case -1: //Joker
            return 0;
        default:
            if (_rank >= 2 && _rank <= 10) return _rank;
            //change to return number value for numbered cards and 10 for face cards
            return 10;
            break;
        }
    }
    return INT32_MIN;
}
int Card::getCambioValue() {
    switch (getRank())
    {
    case 1: //Ace
        return 1;
    case 13: //Red kinh
        return getColor() == RED ? -1 : 10;
    case -1: //Joker
        return 0;
    default:
        if (_rank >= 2 && _rank <= 10) return _rank;
        //change to return number value for numbered cards and 10 for face cards
        return 10;
        break;
    }

}
Suite Card::getSuite() const {
  return _suite;
}

Color Card::getColor() const {
    return _color;
}

std::string Card::getSuiteString() {
    switch (_suite) {
    case HEARTS: return "H";
    case DIAMONDS: return "D";
    case SPADES: return "S";
    case CLUBS: return "C";
    default: return "Joker";
    }
}

std::string Card::getRankString() {
    std::string num = "JOKER";
    int number = _rank;
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

std::string Card::toString(bool reveal) {
    std::string res = getSuiteString() + getRankString();
    return reveal ? res : (_hidden ? "X" : res);
}

bool Card::getHidden() const {
  return _hidden;
}

void Card::setHidden(bool hidden) {
  _hidden = hidden;
}
