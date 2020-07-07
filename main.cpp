#include "application.h"
#include "game-of-life/gameOfLife.h"
//#include "predator-and-prey/predatorAndPrey.h"
#include "native/unix.h"
#include <fstream>

#ifdef _WIN32
#include <windows.h>
#endif

namespace {
    void setConsolePosition() {
        #ifdef _WIN32
                HWND consoleWindow = GetConsoleWindow();
                SetWindowPos(consoleWindow, 0, 0, 0, 500, 500, SWP_NOSIZE | SWP_NOZORDER);
        #endif
    }

    constexpr int NUM_OPTIONS = 3;
    Config config;

    void loadConfig() {
        std::ifstream inFile("config.txt");
        if (!inFile.is_open()) {
            std::cout << "Unable to load config, making default settings\n";
            config = {{ 1280, 720 }, 8 };
            return;
        }
        std::string line;
        while (std::getline(inFile, line)) {
            if (line == "fps") inFile >> config.fps;
            else if (line == "winx") inFile >> config.windowSize.x;
            else if (line == "winy") inFile >> config.windowSize.y;
            else if (line == "cellsize") inFile >> config.cellSize;
        }
        config.init();
    }

    template<typename T>
    void run() {
        std::cout << "\nTo choose another Cellular Automaton, simply close the window\n";
        std::cout << TextColor::green << "Controls: \n" << TextColor::Default;
        std::cout << "Up - Zoom in\nDown - Zoom out\nWASD - Move Around\n";
        std::cout << "R - Reset the view\n";
        std::cin.ignore();
        Application app(config);
        app.init<T>();
        app.run();
        std::cout << '\n';
    }

    bool isValidChoice(int option) {
        return option > 0 && option <= NUM_OPTIONS;
    }
}

int main() {
    loadConfig();
    bool exit = false;
    while (!exit) {
        int option = 0;
        std::cout << "Which Cellular Automaton would you like to see?\n"
            << "1. Game of Life\n"
            << "2. Predator and Prey\n"
            << NUM_OPTIONS << ". Exit\n";
        while (!isValidChoice(option)) {
            std::cin >> option;
            if (!isValidChoice(option)) {
                std::cout << "Invalid option, please pick a number between 1 and " << NUM_OPTIONS << "\n";
                std::cin.ignore();
            }
        }

        switch (option) {
            case 1:
                run<GameOfLife>();
                break;
            case 2:
                //run<PredatorAndPrey>();
                std::cout << "Predator and Prey :D" << std::endl;
                break;
            case NUM_OPTIONS:
                exit = true;
                break;
            default:
                break;
        }
    }
}