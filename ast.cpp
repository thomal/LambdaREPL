#include <string>
#include <stdlib.h>
#include "ast.hpp"

#include <iostream>
using namespace std;

Variable::~Variable () {
    free(name);
}

std::string Variable::toString() {
    return std::string(name);
}

Abstraction::~Abstraction () {
    delete v;
    delete n;
}

std::string Abstraction::toString() {
    return "(λ" + v->toString() + "." + n->toString() + ")";
}

Application::~Application () {
    delete lhs;
    delete rhs;
}

std::string Application::toString() {
    return "(" + lhs->toString() + " " + rhs->toString() + ")";
}
