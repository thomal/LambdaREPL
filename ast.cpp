#include <string>
#include <stdlib.h>
#include "ast.hpp"

#include <iostream>
using namespace std;

Variable::~Variable () {
    cout << "Variable::Destructor\n";
    free(name);
}

std::string Variable::toString() {
    cout << "Variable::toString()\n";
    return std::string(name);
}

Lambda::~Lambda () {
    cout << "Lambda::Destructor\n";
    delete v;
    delete n;
}

std::string Lambda::toString() {
    cout << "Lambda::toString()\n";
    return "(L" + v->toString() + "." + n->toString() + ")";
}

Application::~Application () {
    cout << "Application::Destructor\n";
    delete lhs;
    delete rhs;
}

std::string Application::toString() {
    cout << "Application::toString()\n";
    return "(" + lhs->toString() + " " + rhs->toString() + ")";
}
