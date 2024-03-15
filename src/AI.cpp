#include "AI.h"
#include <random>

int ShallowGame::evaluateFeatures() {    
    int scores[2] = {0};
    int hiddenCards[2] = { 0 };
    for (int i = 0; i < this->playerCount; i++) {
        for (int j = 0; j < 4; j++) {

            Card c = this->tracking.hands[i][j];
            if (c.getHidden()) hiddenCards[i]++;
            if (isDeterminized) {
                c.setHidden(false);
            }
            if (!c.getHidden()) {
                //scores[i] += c.isMagic() ? c.getValue() + 10 : c.getValue();
                scores[i] += c.getValue();
            }
        }
    }

    if (isDeterminized) {
        scores[0] *= (hiddenCards[0]);
        scores[1] *= (hiddenCards[1]);
    }
    int gameScore = scores[1] - scores[0];

    return gameScore;
}
void ShallowGame::throwPlay(Play& p) {


    switch(p.throwable.getRank()) {
        case 7:
        case 8:
        {
            //if our world was determinized then find the card with maximum value
            //otherwise find the first hidden value
            int ci = 0;
            int maxv = INT32_MIN;
            int maxc = 0;
            for (int i = 0; i < 4; i++) {
                Card c = tracking.hands[turn][i];
                // i'm revealing the card in determinized mode manually here just in case we need to differentiate
                // between actual cards and determinized cards
   
                if (isDeterminized) {
                    c.setHidden(false);
                }
                int cv = c.getValue();

                if (c.getHidden()) { ci = i; break; }
                if (isDeterminized && maxv < cv) {
                    maxv = cv;
                    maxc = i;
                }
  
            }
            p.type = PeekOwn;
            p.peekableID = isDeterminized ? maxc : ci;
            p.peekable = tracking.hands[turn][p.peekableID];

             
        }
            break;
        case 9:
        case 10:
        {
            int ci = 0;
            int minv = INT32_MAX;
            int minc = 0;
            for (int i = 0; i < 4; i++) {
                Card c = tracking.hands[(3 - turn) % 2][i];
                if (isDeterminized)
                    c.setHidden(false);
                int cv = c.getValue();
                if (c.getHidden()) ci = i;
                if (isDeterminized && minv > cv) {
                    minv = cv;
                    minc = i;
                }
            }
            p.type = PeekOther;
            p.peekableID = isDeterminized ? minc : ci;
            p.peekable = tracking.hands[(3 - turn) % 2][p.peekableID];
        }
            break;
        case 11:
        case 12:
        {
            int maxv = INT32_MIN;
            int maxc = 0;
            for (int i = 0; i < 4; i++) {
                Card c = tracking.hands[turn][i];
                if (isDeterminized)
                    c.setHidden(false);
                if (c.getHidden()) continue;
                int cv = c.getValue();
                if (maxv < cv) {
                    maxv = cv;
                    maxc = i;
                }
            }

            int minv = INT32_MAX;
            int minc = 0;
            for (int i = 0; i < 4; i++) {
                Card c = tracking.hands[(3-turn)%2][i];
                if (isDeterminized)
                    c.setHidden(false);
                if (c.getHidden()) continue;
                int cv = c.getValue();
                if (minv > cv) {
                    minv = cv;
                    minc = i;
                }
            }
            
            p.type = BlindSwitch;
            p.ownSwitchID = maxc;
            p.ownCard = tracking.hands[turn][maxc];

            p.enemySwitchID = minc;
            p.enemyCard = tracking.hands[(3 - turn) % 2][minc];

        }

        case 13: {
            if (p.throwable.getColor() == BLACK) {
                int ci = 0;
                int minv = INT32_MAX;
                int minc = 0;
                for (int i = 0; i < 4; i++) {
                    Card c = tracking.hands[(3 - turn) % 2][i];
                    if (isDeterminized)
                        c.setHidden(false);
                    int cv = c.getValue();
                    if (c.getHidden()) ci = i;
                    if (isDeterminized && minv > cv) {
                        minv = cv;
                        minc = i;
                    }
                }
                p.type = PeekSwitch;
                p.peekableID = isDeterminized ? minc : ci;
                p.peekable = tracking.hands[(3 - turn) % 2][p.peekableID];

                p.enemySwitchID = isDeterminized ? minc : ci;
                p.enemyCard = tracking.hands[(3 - turn) % 2][p.peekableID];

                int maxv = INT32_MIN;
                int maxc = 0;
                for (int i = 0; i < 4; i++) {
                    Card c = tracking.hands[turn][i];
                    if (isDeterminized)
                        c.setHidden(false);
                    if (c.getHidden()) continue;
                    int cv = c.getValue();
                    if (maxv < cv) {
                        maxv = cv;
                        maxc = i;
                    }
                }
                p.ownSwitchID = maxc;
                p.ownCard = tracking.hands[turn][maxc];

            }
        }
        break;
        
    }
}

void ShallowGame::move(Play& p) {
    switch(p.type) {
        case PeekOwn: {
            if (isDeterminized) {
                tracking.hands[turn][p.peekableID].setHidden(false);
            }
            //otherwise do nothing
        }
            
            break;
        case PeekOther: {
            if (isDeterminized) {
                tracking.hands[(3 - turn) % 2][p.peekableID].setHidden(false);
            }
            //otherwise do nothing
        }
            
            break;
        case BlindSwitch: 
            tracking.hands[turn][p.ownSwitchID] = p.enemyCard;
            tracking.hands[(3 - turn) % 2][p.enemySwitchID] = p.ownCard;
            break;
        case PeekSwitch:
            if (isDeterminized) {
                tracking.hands[(3 - turn) % 2][p.peekableID].setHidden(false);
            }
            tracking.hands[turn][p.ownSwitchID] = p.enemyCard;
            tracking.hands[(3 - turn) % 2][p.enemySwitchID] = p.ownCard;
            break;
        case Replace:
        {
            int id = p.replaceableID;
            tracking.hands[turn][id] = p.throwable; 
        }
            break;
        case Call:
            break;
    }
    turn = (3 - turn) % 2;
}

void ShallowGame::unMove(Play& p) {
    //look at the current state of the game
    //based on the play that was performed
    //we know cards that have been affected it's all stored in Play object

    switch (p.type) {
        case Replace:
        {           
            int id = p.replaceableID;
            this->tracking.hands[turn][id] = p.replaceable;
        }            
        break;
        case PeekOwn:
        {
            if (isDeterminized) {
                tracking.hands[turn][p.peekableID].setHidden(false);
            }
        }
             break;
        case PeekOther:
        {
            if (isDeterminized) {
                tracking.hands[(3 - turn) % 2][p.peekableID].setHidden(false);
            }
        }
             break;
        case BlindSwitch:
            tracking.hands[turn][p.ownSwitchID] = p.ownCard;
            tracking.hands[(3 - turn) % 2][p.enemySwitchID] = p.enemyCard;
            break;
        case PeekSwitch:
            if (isDeterminized) {
                tracking.hands[(3 - turn) % 2][p.peekableID].setHidden(true);
            }
            tracking.hands[turn][p.ownSwitchID] = p.ownCard;
            tracking.hands[(3 - turn) % 2][p.enemySwitchID] = p.enemyCard;
            break;
    }
    turn = (3 - turn) % 2;
}

std::vector<Play> ShallowGame::possibleMoves(int seed) {
    //if card is magic then throw, otherwise replace

    std::vector<Play> plays;
    std::mt19937 gen(seed);
    std::uniform_int_distribution<int> dis(0, 13);
    std::uniform_int_distribution<int> suitedis(0, 4);
    Suite s = (Suite)suitedis(gen);

    int cardid = dis(gen); // get a random card index.
    Color color = RED;
    if (s == CLUBS || s == SPADES) color = BLACK;
    Card thr = Card(cardid, (Suite)s, color, true);

    
    //Card thr = deck.back(); 
    //deck.pop_back();

    Play dummy;
    dummy.throwable = thr;

    if(!thr.isMagic()) {
            int maxv = INT32_MIN;
            int maxc = 0;
            for (int i = 0; i < 4; i++) {
                Card c = tracking.hands[turn][i];
             

                if (isDeterminized)
                    c.setHidden(false);

                if (!c.getHidden()) {
                    int cv = c.getValue();
                    if (maxv < cv) {
                        maxv = cv;
                        maxc = i;
                    }
                }

         
            }
            dummy.type = Replace;
            dummy.replaceableID = maxc;
            dummy.replaceable = tracking.hands[turn][maxc];
    } else {
        this->throwPlay(dummy);
    }
    plays.push_back(dummy);

    return plays;
}


void Tracking::update(int turn, int idCard, Card* card) {
    hands[turn][idCard] = *card;
}

void Tracking::initialize(std::vector<Card> player, std::vector<Card> ai) {
    this->hands[0] = player;
    this->hands[1] = ai;
}