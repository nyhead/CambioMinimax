#include "ViewInput.h"
#include <cstdlib>
#include <chrono> // Include the chrono library for time measurement
#include <thread> // Include the thread library for sleeping
void ViewInput::printCard(std::string c, std::string ending) {

	std::cout << c << ending;
}





void ViewInput::update(std::vector<Player*>& players, int length) {
    // Define the duration for which you want to keep printing
    std::chrono::milliseconds duration(length); // For example, 5000 milliseconds (5 seconds)

    // Get the start time

    
        // Your existing code to print the cards
        Hand* h1 = &players[0]->getHand();
        Hand* h2 = &players[1]->getHand();
        for (int i = 0; i < 3; i+=2) {
            printCard(h1->getCard(i)->toString(reveal), " ");
            printCard(h1->getCard(i+1)->toString(reveal), "		");

            printCard(h2->getCard(i)->toString(reveal), " ");
            printCard(h2->getCard(i+1)->toString(reveal),"\n");
        }

        if (h1->size() > 4) {
            printCard(h1->getCard(4)->toString(reveal), "");
            std::cout << " " << "		";
        }
        if (h2->size() > 4) {
            printCard(h2->getCard(4)->toString(reveal), "\n");
        }

        std::cout << std::endl << std::endl;
		std::cout << std::flush;
		if (length > 0) {
			std::this_thread::sleep_for(std::chrono::milliseconds(length));

			// Clear the screen after waiting
			clearScreen();
		}
 //   auto start = std::chrono::high_resolution_clock::now();
	//while (true) {
	//	if (length == 0) break;

	//	
 //       // Check if the elapsed time has exceeded the desired duration
 //       auto now = std::chrono::high_resolution_clock::now();
 //       auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
 //       if (elapsed >= duration) {
	//		clearScreen();
 //           break; // Exit the loop if the duration has been reached
 //       }
 //   }
}

void ViewInput::agentView(std::vector<Card>(&hands)[2])
{
	if (aiView) {

	std::cout << "Agent's perception: " << std::endl;
	std::vector<Card> h1 = hands[0];
	std::vector<Card> h2 = hands[1];

	for (int i = 0; i < 3; i += 2) {
		//if (h1[i].getHidden())
		//	printCard("X", " ");
		//else 
			printCard(h1[i].toString(reveal), " ");

		//if (h1[i+1].getHidden())
		//	printCard("X", "		");
		//else
			printCard(h1[i+1].toString(reveal), "		");


		//if (h2[i].getHidden())
		//	printCard("X", " ");
		//else
			printCard(h2[i].toString(reveal), " ");

		//if (h2[i + 1].getHidden())
		//	printCard("X", "\n");
		//else
			printCard(h2[i + 1].toString(reveal), "\n");

	}

	std::cout << std::endl << std::endl;
	}
}

void ViewInput::clearScreen()
{

#if defined _WIN32
	system("cls");
	//clrscr(); // including header file : conio.h
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
	system("clear");
	//std::cout<< u8"\033[2J\033[1;1H"; //Using ANSI Escape Sequences 
#elif defined (__APPLE__)
	system("clear");
#endif
}

bool ViewInput::InputDecideCallCambio() {
	//sf::Event event;
	//bool lock_click = false;
	//while (!lock_click) {
	//	_w->pollEvent(event);
	//	if (event.mouseButton.button == sf::Mouse::Left) {
	//		sf::Vector2f mouse = _w->mapPixelToCoords(sf::Mouse::getPosition(*_w));
	//		sf::FloatRect bounds = callCambioText.getGlobalBounds();
	//		lock_click = true;
	//		if (bounds.contains(mouse)) {
	//			return true;
	//		}
	//	}
	//}
	return false;
}

int ViewInput::InputChooseCard(int turn, std::string overrideMsg) {

	//ignore_identity is for deciding whether we want to choose 
	//one of the opponent's cards or only our cards when ignore_identity set to false

	if (overrideMsg.empty())
		std::cout << "Choose " <<  turn+1 << " player's card:" << std::endl;

	std::cout << overrideMsg << std::endl;
	std::string input;
	std::getline(std::cin, input); // Read the entire line of input

	std::stringstream ss(input);
	int res = -1;
	ss >> res;
	if (res < 1) res = -1;
	if (res >= 0 && res <= 4) res -= 1;
	return res;
	//bool lock_click = false;
	//sf::Event event;
	//while (!lock_click) {
	//	_w->pollEvent(event);
	//	if (event.type == sf::Event::Closed)
	//		_w->close();
	//	if (event.type == sf::Event::MouseButtonPressed) //Mouse button Pressed
	//	{
	//		if (event.mouseButton.button == sf::Mouse::Left) //specifies
	//		{
	//			sf::Vector2f mouse = _w->mapPixelToCoords(sf::Mouse::getPosition(*_w));


	//			lock_click = true;

	//			// hit test
	//			for (int i = 0; i < positions.size(); i++) {
	//				if (!ignoreIdentity and i != currentPlayer) continue;
	//				if (ignoreIdentity and i == currentPlayer) continue;
	//				for (int j = 0; j < positions[i].size(); j++) {

	//					sf::FloatRect bounds = positions[i][j].getGlobalBounds();


	//					if (bounds.contains(mouse))
	//					{
	//						// mouse is on sprite!
	//						
	//						std::cout << "pressed " << i << std::endl;
	//						return std::pair<int,int>(i,j);
	//					}

	//				}
	//			}
	//		}
	//	}
	//}
	//return std::pair<int, int>(-1, -1);
}