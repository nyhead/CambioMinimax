#include "Hand.h"
#include "Card.h"

Hand::Hand(){
    _vCards = std::vector<Card*>();
}

Hand::~Hand() {
    _vCards.clear();
}


void Hand::setCard(int myIndex, Card* card) {
    _vCards[myIndex] = card;
}

void Hand::switchCards(Hand& to, int index, int myIndex) {
    if (index >= 0 && myIndex >= 0) {
        Card mCard = *getCard(myIndex);
        Card card = *to.getCard(index);
        
        *to._vCards[index] = mCard;
        *_vCards[myIndex] = card;
    }
}


int Hand::size(){
  return _vCards.size();
}

int Hand::getScore() {
    int score = 0;
    for (int i = 0; i < _vCards.size(); i++) {
        score += _vCards[i]->getCambioValue();
    }

    return score;
}

Card* Hand::getCard(int index){
  return _vCards[index];
}


Card * Hand::operator[](int index) {
  return getCard(index);
}

void Hand::addCard(Card * card) {
  _vCards.push_back(card);
}

void Hand::addCard(int index, Card* card) {
  _vCards[index] = card;
}

Card* Hand::popCard(){
  Card* topCard = _vCards.back();
  _vCards.pop_back();
  return topCard;
}

std::vector<Card> Hand::copyCards()
{
  std::vector<Card> cards;
  for (int i = 0; i < this->size(); i++) {
    cards.push_back(*_vCards[i]);
  }
  return cards;
}

//Hand Hand::returnSorted() {
//  auto copy = this->_vCards;
//  std::sort(copy.begin(), copy.end(), [](Card* a, Card* b){ return a->getValue() > b->getValue();});
//}