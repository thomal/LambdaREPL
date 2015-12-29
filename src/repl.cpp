#include "repl.hpp"
#include "CPPcommon.hpp"
#include <iostream>

void REPLCmd (std::string cmd) {
    if (cmd == "quit")
        REPLrunning = false;
    else if (cmd == "debug")
        debugMode = !debugMode;
    else
        std::cout << "ERROR: Unknown command" << std::endl;
}
