#include <string>
#include <ctime>
#include <map>
#include "Hand.h"
#include "Deck.h"
#include <cstdlib>
#include <algorithm> // For std::swap
#include <random>
#include <ctime>
Deck::Deck() : Hand(){ }

int Deck::deal(Hand &hand){
    Card* card = popCard();
    hand.addCard(card);
    return card->getRank();
}

void Deck::makeDeck() {
  for(int suite=0; suite<= 3; suite++){
          for (int rank = 1; rank <= 13; rank++) {
              Suite s = (Suite)suite;
              Color color = RED;
              if (s == CLUBS || s == SPADES) color = BLACK;
              addCard(new Card(rank, s, color));
          }
      
  }
// TO-DO add jokers
}

//void Deck::shuffleDeck() {
//    // Engine for random number generation
//    std::default_random_engine generator(std::random_device{}());
//
//    int size = _vCards.size();
//    // Parameters for normal distribution: mean and standard deviation
//    double mean = size /5.0;
//    double stddev = size / 10.0; // Adjust this value to control spread
//
//    std::normal_distribution<double> distribution(mean, stddev);
//
//    std::vector<Card*> old = _vCards;
//    _vCards.clear();
//
//    std::vector<bool> picked(size, false); // Keep track of picked cards to avoid duplicates
//
//    while (_vCards.size() < size) {
//        int place = std::round(distribution(generator));
//
//        // Ensure the generated index is within bounds
//        if (place >= 0 && place < size && !picked[place]) {
//            _vCards.push_back(old[place]);
//            picked[place] = true; // Mark as picked
//        }
//    }
//}

void Deck::shuffleDeck() {
    std::srand(unsigned(std::time(NULL)));
    size_t size = _vCards.size();
    std::vector<Card*> old = _vCards;
    _vCards.clear();
    while (_vCards.size() < size) {
        int place = (std::rand() % size);
        Card* card = old[place];
        old[place] = card;
        _vCards.push_back(card);
    }
}

// Deck& Deck::operator=(Deck& other) {
//   if (this != &other) {
//     _vCards = other._vCards;
//   }
//   return *this;
// }