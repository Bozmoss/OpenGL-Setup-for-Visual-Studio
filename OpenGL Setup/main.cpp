#include "engine.hpp"

int main() {
	Engine engine(1920, 1080, "OpenGL");

	if (!engine.init()) {
		std::cout << std::endl << "Press any key to close program..." << std::endl;
		std::cin.get();
	}
}