#include "Game.h"
#include <algorithm>
#include <thread>
#include <chrono>
#include <random>
#include <utility>
#include <future>

Game::Game(ViewInput& f){
  _isActive = true;
  _playerTurn = 0;
  _gameTick = 0;
  _Players = 2;
  _front = &f;
}

void Game::createGame() {
    createPlayers();
    makeDeck();
    _deck.shuffleDeck();
    dealCards();

}


bool Game::isActive() {
  return _isActive;
}

void Game::setActive(bool active) {
  _isActive = active;
}


Deck& Game::getDeck () {
    return _deck;
}

int minimax(ShallowGame g, int depth, Play& bestPlay, int seed) {

    if (depth == 0 ) {
        return -g.evaluateFeatures();
    }

    int bestVal = g.turn == 1 ? INT32_MIN : INT32_MAX;
    for (auto p : g.possibleMoves(seed)) {
        Play dummy;
        g.move(p);
        int v = minimax(g, depth - 1, dummy, seed);
        g.unMove(p);
        if (g.turn ==1 ? v > bestVal : v < bestVal) {
            bestVal = v;
            bestPlay = p;
        }
    }
    
    return bestVal;
}
bool customComparator(const std::pair<Play, int>& a, const std::pair<Play, int>& b) {
    return a.second < b.second;  // Compare by the second key
}
Play Game::findActualMove(std::vector<std::pair<Play, int>> &moves, Card *d) {
    Play p;
    for (auto m : moves) {
        if (p.type != Uninitialized) break;
        Play temp = m.first;
        if (temp.throwable.getRank() != d->getRank() || temp.throwable.getSuite() != d->getSuite()) {
            continue;
        }
        switch (temp.type)
        {
        case Replace:
        {
            int id = temp.replaceableID;
            int rank = temp.replaceable.getRank();
            if (_vPlayers[1]->getHand()[id]->getRank() == rank) {
                p = temp;
                break;
            }

        }
        break;
        case PeekOwn:
        {

            int id = temp.peekableID;
            int rank = temp.peekable.getRank();
            if (_vPlayers[1]->getHand()[id]->getRank() == rank) {
                p = temp;
                break;
            }
        }
        break;
        case PeekOther: {

            int id = temp.peekableID;
            int rank = temp.peekable.getRank();
            if (_vPlayers[0]->getHand()[id]->getRank() == rank) {
                p = temp;
                break;
            }
        }
                      break;
        case BlindSwitch: {

            int idOwn = temp.ownSwitchID;
            int idEnemy = temp.enemySwitchID;
            int rankOwn = temp.ownCard.getRank();
            int rankEnemy = temp.enemyCard.getRank();
            if (_vPlayers[0]->getHand()[idEnemy]->getRank() == rankEnemy && _vPlayers[1]->getHand()[idOwn]->getRank() == rankOwn) {
                p = temp;
                break;
            }
        }
                        break;
        case PeekSwitch: {
            int idOwn = temp.ownSwitchID;
            int idEnemy = temp.enemySwitchID;
            int rankOwn = temp.ownCard.getRank();
            int rankEnemy = temp.enemyCard.getRank();
            if (_vPlayers[0]->getHand()[idEnemy]->getRank() == rankEnemy && _vPlayers[1]->getHand()[idOwn]->getRank() == rankOwn) {
                p = temp;
            }
        }
                       break;
        default:
            break;
        }
    }
    return p;
}


int Game::gameLoop() {

      while (isActive() && _deck.size() > 1) {
        std::cout << "Deck size: " <<_deck.size() << std::endl;
            _playerTurn = _gameTick % _vPlayers.size();
          std::cout << "Game's tick: " << _gameTick << std::endl;
            if (_vPlayers[_playerTurn]->isHuman) {
                playerAction(*_vPlayers[_playerTurn]);
            }
            else {
                auto g = this->copy();
      
                std::vector<std::pair<Play, int>> moves = runManySims(g);
                Card *d = _deck.popCard();
                Play p = findActualMove(moves, d); //match the move that could actually be made
                if (p.type == Uninitialized) {
                    std::cout << "aiAction enabled";
                    p = aiAction(d); // if not found, discard the card and act a certain strategy based on its power
                } 
                else
                    std::cout << "minimax enabled";
                this->move(p, d);

                _front->agentView(_tracker.hands);
            }     
            _gameTick++;
        }
    
        

    int w = this->deduceWinner();
    std::cout << "Cambio called, winner is " << w+1 << std::endl;
    for (int i = 0; i < this->getNumberOfPlayers(); ++i) {
        for (int j = 0; j < 4; j++) 
            _vPlayers[i]->getHand()[j]->setHidden(false);
    }
    _front->update(_vPlayers);

    return 0;
}

Play Game::aiAction(Card *c) {
    Play p;
    switch (c->getRank()) {
    case 7:
    case 8:
    {
        int ci = 0;
        int maxv = INT32_MIN;
        int maxc = 0;
        for (int i = 0; i < 4; i++) {
            Card c = _tracker.hands[turn()][i];
            int cv = c.getValue();

            if (c.getHidden()) { ci = i; break; }

        }
        p.type = PeekOwn;
        p.peekableID = ci;
        p.peekable = _tracker.hands[turn()][p.peekableID];


    }
    break;
    case 9:
    case 10:
    {
        int ci = 0;
        int minv = INT32_MAX;
        int minc = 0;
        for (int i = 0; i < 4; i++) {
            Card c = _tracker.hands[(3 - turn()) % 2][i];
            int cv = c.getValue();
            if (c.getHidden()) ci = i;
        }
        p.type = PeekOther;
        p.peekableID = ci;
        p.peekable = _tracker.hands[(3 - turn()) % 2][p.peekableID];
    }
    break;
    case 11:
    case 12:
    {
        int maxv = INT32_MIN;
        int maxc = 0;
        for (int i = 0; i < 4; i++) {
            Card c = _tracker.hands[turn()][i];
            if (c.getHidden()) continue;
            int cv = c.getValue();
        }

        int minv = INT32_MAX;
        int minc = 0;
        for (int i = 0; i < 4; i++) {
            Card c = _tracker.hands[(3 - turn()) % 2][i];
            if (c.getHidden()) continue;
            int cv = c.getValue();
            if (minv > cv) {
                minv = cv;
                minc = i;
            }
        }

        p.type = BlindSwitch;
        p.ownSwitchID = maxc;
        p.ownCard = _tracker.hands[turn()][maxc];

        p.enemySwitchID = minc;
        p.enemyCard = _tracker.hands[(3 - turn()) % 2][minc];

    }

    case 13: {
        if (p.throwable.getColor() == BLACK) {
            int ci = 0;
            int minv = INT32_MAX;
            int minc = 0;
            for (int i = 0; i < 4; i++) {
                Card c = _tracker.hands[(3 - turn()) % 2][i];
                int cv = c.getValue();
                if (c.getHidden()) ci = i;
            }
            p.type = PeekSwitch;
            p.peekableID = ci;
            p.peekable = _tracker.hands[(3 - turn()) % 2][p.peekableID];

            p.enemySwitchID = ci;
            p.enemyCard = _tracker.hands[(3 - turn()) % 2][p.peekableID];

            int maxv = INT32_MIN;
            int maxc = 0;
            for (int i = 0; i < 4; i++) {
                Card c = _tracker.hands[turn()][i];
                if (c.getHidden()) continue;
                int cv = c.getValue();
                if (maxv < cv) {
                    maxv = cv;
                    maxc = i;
                }
            }
            p.ownSwitchID = maxc;
            p.ownCard = _tracker.hands[turn()][maxc];

        }
    }
           break;

    }
    return p;
}

void Game::playerAction(Player& player) {
    //bool isCambio = _front->InputChooseCard(turn(), "Enter to discard/your card's id to be replaced:");
    //if (isCambio) {
    //    int w = this->deduceWinner();
    //    std::cout << "Cambio called, winner is " << w << std::endl;
    //    this->setActive(false);
    //    return;
    //}


    _deck.deal(player.getHand());
    _front->update(_vPlayers);
    int cardId = _front->InputChooseCard(turn(), "Discard(enter)/write 5 for calling/write your card's id to be replaced:");
    auto c = player.getHand().popCard();
    if (cardId == -1) {

        switch (c->getRank()) {
            case 7:
            case 8:
            case 9:
            case 10:{
                
                int playerIndex = c->getRank() < 9 ? 0: 1;
                std::cout << "Peek " << playerIndex +1<< " player card" << std::endl;
                int cardIndex = _front->InputChooseCard(playerIndex);
                std::cout << cardIndex << std::endl;
                _vPlayers[playerIndex]->getHand()[cardIndex]->setHidden(false);

                Card* tr = (player.getHand()[cardIndex]);
                
                _front->update(_vPlayers,1000);

                _vPlayers[playerIndex]->getHand()[cardIndex]->setHidden(true);

            }
                break;
            case 11:
            case 12: {
                std::cout << "Blind switch your card with enemy's" << std::endl;
                auto yourCardIndex = _front->InputChooseCard(0);
                int playerIndex = 1;
                int cardIndex = _front->InputChooseCard(1);
                _vPlayers[playerIndex]->getHand().switchCards(player.getHand(), yourCardIndex, cardIndex);
                Card yourCardTr = _tracker.hands[turn()][yourCardIndex];
                _tracker.hands[turn()][yourCardIndex] = _tracker.hands[playerIndex][cardIndex];
                _tracker.hands[playerIndex][cardIndex] = yourCardTr;
            }
                break;
            case 13: {
                if (c->getColor() == BLACK) {
                    std::cout << "Peek anyone card's and switch your card with enemy's" << std::endl;
                    int playerIndex = 1;
                    playerIndex = _front->InputChooseCard(playerIndex, "Choose one of the 2 players(1-2)");
                    std::cout << "Peek " << playerIndex << " player card" << std::endl;
                    int cardIndex = _front->InputChooseCard(playerIndex);
                    std::cout << cardIndex << std::endl;
                    _vPlayers[playerIndex]->getHand()[cardIndex]->setHidden(false);

                    Card* tr = (player.getHand()[cardIndex]);

                    _front->update(_vPlayers,1000);
                    _front->update(_vPlayers);
                    player.getHand()[cardIndex]->setHidden(true);

                    std::cout << "Switch your card with enemy's" << std::endl;
                    auto yourCardIndex = _front->InputChooseCard(0);
                    playerIndex = 1;
                    cardIndex = _front->InputChooseCard(1);
                    _vPlayers[playerIndex]->getHand().switchCards(player.getHand(), yourCardIndex, cardIndex);
                    Card yourCardTr = _tracker.hands[turn()][yourCardIndex];
                    _tracker.hands[turn()][yourCardIndex] = _tracker.hands[playerIndex][cardIndex];
                    _tracker.hands[playerIndex][cardIndex] = yourCardTr;
                }
            }
                break;
        }
    }
    else if (cardId == 5) {
    this->setActive(false);
    //return;
    }
    else {
        player.getHand().setCard(cardId, c);
    }
    
}


int Game::deduceWinner() {
    std::vector<Player*>::iterator result = std::min_element(_vPlayers.begin(), _vPlayers.end(),
        [](Player*  lhs, Player* rhs) {
            return lhs->getHand().getScore() < rhs->getHand().getScore();
        });
     return std::distance(_vPlayers.begin(), result);
}


void Game::setNumberOfPlayers(int num){
  _Players = num;
}

int Game::getNumberOfPlayers(){
  return _Players;
}

void Game::makeDeck() {
    _deck = Deck();
    _deck.makeDeck();
}

void Game::dealCards() {
    for (int i = 0; i < this->getNumberOfPlayers(); ++i) {
        _vPlayers[i] = new Player(true);
        for (int j = 0; j < 4; ++j) {
            _deck.deal(_vPlayers[i]->getHand());
        }
    }
    _vPlayers[1]->getHand()[0]->setHidden(true);
    _vPlayers[1]->getHand()[1]->setHidden(true);
    _vPlayers[1]->getHand()[2]->setHidden(false);
    _vPlayers[1]->getHand()[3]->setHidden(false);
    _vPlayers[1]->isHuman = false;

    _tracker.initialize(_vPlayers[0]->getHand().copyCards(),
        _vPlayers[1]->getHand().copyCards());

    _vPlayers[1]->getHand()[2]->setHidden(true);
    _vPlayers[1]->getHand()[3]->setHidden(true);


    _vPlayers[0]->getHand()[0]->setHidden(true);
    _vPlayers[0]->getHand()[1]->setHidden(true);
    _vPlayers[0]->getHand()[2]->setHidden(false);
    _vPlayers[0]->getHand()[3]->setHidden(false);
    _front->update(_vPlayers,2000);

    for (int i = 0; i < this->getNumberOfPlayers(); ++i) {
        _vPlayers[i]->getHand()[2]->setHidden(true);
        _vPlayers[i]->getHand()[3]->setHidden(true);
    }
}

void Game::createPlayers() {
    for (int i = 0; i < this->getNumberOfPlayers(); ++i) {
        _vPlayers.push_back(nullptr);
    }
}

std::pair<int, ShallowGame> Game::determinize(ShallowGame world) {
    int playerCount = 2; // Defined in the ShallowGame, but reiterating here for clarity.
    std::random_device rd;
    std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd().
    std::uniform_int_distribution<int> dis(0, 13);
    for (int i = 0; i < playerCount; ++i) {
        std::uniform_int_distribution<int> suiteDis(0, 4);
        for (int j = 0; j < 4; ++j) {
            if (world.tracking.hands[i][j].getHidden()) {
                Suite s = (Suite)suiteDis(gen);

                int cardId = dis(gen); // Get a random card index.
                Color color = RED;
                if (s == CLUBS || s == SPADES) color = BLACK;
                Card card = Card(cardId, (Suite)s, color, true);

                world.tracking.hands[i][j] = card;
            }
            
        }
    }

    int seed = gen(); 
    world.isDeterminized = true;
    return std::make_pair(seed, world);
}
std::pair<Play, int> Game::simulateGame(const ShallowGame& world) {
    std::pair<int, ShallowGame> seedadetState = determinize(world);
    Play p;
    int score = minimax(seedadetState.second, 10, p, seedadetState.first);
    return std::make_pair(p, score);
}
std::vector<std::pair<Play, int>> Game::runManySims(ShallowGame& world)
{
    //std::vector<std::pair<Play, int>> moves;
    //int nSimulations = 10000; 
    //for (int i = 0; i < nSimulations; i++) { 
    //    auto res = simulateGame(world);
    //    moves.push_back(std::make_pair(res.first, res.second));
    //}
    //sort(moves.begin(), moves.end(), customComparator);

    //return moves;
    std::vector<std::future<std::pair<Play, int>>> futures;
    int nSimulations = 10000;
    for (int i = 0; i < nSimulations; ++i) {
        futures.push_back(std::async(std::launch::async, &Game::simulateGame, this, world));
    }

    // Collect results
    std::vector<std::pair<Play, int>> moves;
    for (auto& future : futures) {
        moves.push_back(future.get());
    }

    sort(moves.begin(), moves.end(), customComparator);

    return moves;

}


int Game::turn() {
    return _playerTurn;
}


ShallowGame Game::copy() {
    ShallowGame cpy;
    cpy.deck = _deck.copyCards();
    cpy.tracking = _tracker;
    cpy.turn = this->turn();
    return cpy;
}

void Game::move(Play& p, Card *drawn) {
    switch(p.type) {
        case PeekOwn:
            _tracker.hands[turn()][p.peekableID].setHidden(false);
            //_vPlayers[turn()]->getHand().getCard(p.peekableID)->setHidden(false);
            std::cout << std::endl << "PeekOwn" << std::endl;
            std::cout << "AI card:" << p.peekableID +1<< "was revealed" << std::endl;
            break;
        case PeekOther:
            _tracker.hands[(3-turn())%2][p.peekableID].setHidden(false);
            //_vPlayers[(3 - turn()) % 2]->getHand().getCard(p.peekableID)->setHidden(false);
            std::cout << std::endl << "PeekOther" << std::endl;
            std::cout << "Human's card:" << p.peekableID +1<< "was revealed" << std::endl;
            break;
        case BlindSwitch: 
        {


            _vPlayers[turn()]->getHand().switchCards(_vPlayers[(3 - turn()) % 2]->getHand(), p.enemySwitchID,p.ownSwitchID);
            Card::swap(_tracker.hands[turn()][p.ownSwitchID], _tracker.hands[(3 - turn()) % 2][p.enemySwitchID]);

            std::cout << std::endl << "BlindSwitch" << std::endl;
            std::cout << "Agent's card:" << p.ownSwitchID +1<< "was switched with:" << p.enemySwitchID +1<< std::endl;
        }
            break;
        case PeekSwitch:
            _tracker.hands[(3 - turn()) % 2][p.peekableID].setHidden(false);            
            std::cout << std::endl << "PeekSwitch" << std::endl;
            std::cout << "Human's card:" << p.peekableID +1<< "was revealed" << std::endl;

            _vPlayers[turn()]->getHand().switchCards(_vPlayers[(3 - turn()) % 2]->getHand(), p.enemySwitchID, p.ownSwitchID);
            Card::swap(_tracker.hands[turn()][p.ownSwitchID], _tracker.hands[(3 - turn()) % 2][p.enemySwitchID]);
            std::cout << "Agent's card:" << p.ownSwitchID +1<< "was switched with:" << p.enemySwitchID +1<< std::endl;
            break;
        case Replace:
        {
            int id = p.replaceableID;
            //may cause some problems since we are not sharing the orginal card's adress (p->throwable)
            Card* tobeReplace = _vPlayers[turn()]->getHand().getCard(id);
            *tobeReplace = *drawn;
            //_vPlayers[turn()]->getHand().setCard(id, top);

            _tracker.hands[turn()][id] = p.throwable;
            std::cout << std::endl << "Replace" << std::endl;
            std::cout << "Agent's card:" << p.replaceableID +1<< "was replaced with:" << p.throwable.toString(_front->reveal) << std::endl;
        }
            break;
        case Call:
            break;
    }
      _front->update(_vPlayers);
}

Game::~Game() {
}


