#include "repl.hpp"
#include <iostream>

void REPLCmd (std::string cmd) {
    if (cmd == "quit")
        REPLrunning = false;
    else
        std::cout << "ERROR: Unknown command" << std::endl;
}
