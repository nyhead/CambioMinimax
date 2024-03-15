#include <iostream>
#include "Game.h"


#include <unordered_map>

int main(int argc, char* argv[]) {
    bool reveal = false;
    bool agentv = false;
    // Parse arguments starting from the second element (skip program name)
    for (int i = 1; i < argc; ++i) {
        std::string arg(argv[i]);

        // Check for specific arguments
        if (arg == "reveal") {
            reveal = true;
        }
        else if (arg == "agentv") {
            agentv = true;
        }
        else {
            // Handle unknown arguments (optional)
            std::cerr << "Warning: Unknown argument: " << arg << std::endl;
        }
    }

    // Access parsed arguments

	ViewInput v;
    v.aiView = agentv;
    v.reveal = reveal;
	Game g(v);
	g.createGame();
    g.gameLoop();

	return 0;
}