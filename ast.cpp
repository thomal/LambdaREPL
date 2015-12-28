#include "ast.hpp"
#include <string>
#include <stdlib.h> //exit(1)
#include <string.h> //strdup
#include <iostream>
#include <sstream>  //ostringstream
#include <stdint.h> //uint64_t
using namespace std;

//////// ID GENERATION /////////////////////////////////////////////////////////
typedef uint64_t vid_t;
vid_t globallyUniqueID = 0; //Maximum number of variables

std::string newID () {
    id_t oldid = globallyUniqueID;
    id_t newid = ++globallyUniqueID;
    cout << "newID(): " << oldid << "->" << newid << "\n";
    if (newid > oldid) {
        std::ostringstream oss;
        oss << newid;
        return oss.str();
    } else {
        cout << "Variable limit of 2^64 bound variables exceeded.\n";
        exit(1); //Overflow, 2^64 variable limit exceeded
    }
}
////////////////////////////////////////////////////////////////////////////////

//////// VARIABLE //////////////////////////////////////////////////////////////
Variable::~Variable () {
    free(name);
}

std::string Variable::toString() {
    return std::string(name);
}

void Variable::alphaReduce(std::string oldname, std::string newname) {
    if (strcmp(name, oldname.c_str())==0) {
        free(name);
        name = strdup(newname.c_str());
        hasUniqueName = true;
    }
}

void Variable::betaReduce() {
    //TODO
}

bool Variable::isClosed() {
    return hasUniqueName;
}
////////////////////////////////////////////////////////////////////////////////

//////// ABSTRACTION ///////////////////////////////////////////////////////////
Abstraction::~Abstraction () {
    delete v;
    delete n;
}

std::string Abstraction::toString() {
    return "(λ" + v->toString() + "." + n->toString() + ")";
}

void Abstraction::alphaReduce(std::string oldname, std::string newname) {
    //If we aren't shadowing oldname alpha-reduce rhs
    if (strcmp(v->name,oldname.c_str())!=0)
        n->alphaReduce(oldname, newname);
    
    //Now alpha reduce the variable we bind to a unique name.
    //If we've already done so in a previous call then skip this.
    if (!v->hasUniqueName) {
        string newNameForOurArg = newID();
        string oldNameForOurArg = std::string(v->name);
        v->alphaReduce(oldNameForOurArg, newNameForOurArg);
        n->alphaReduce(oldNameForOurArg, newNameForOurArg);
    }
}

void Abstraction::betaReduce() {
    //TODO
}

bool Abstraction::isClosed() {
    return n->isClosed();
}
////////////////////////////////////////////////////////////////////////////////

//////// APPLICATION ///////////////////////////////////////////////////////////
Application::~Application () {
    delete lhs;
    delete rhs;
}

std::string Application::toString() {
    return "(" + lhs->toString() + " " + rhs->toString() + ")";
}

void Application::alphaReduce(std::string oldname, std::string newname) {
    lhs->alphaReduce(oldname, newname);
    rhs->alphaReduce(oldname, newname);
}

void Application::betaReduce() {
    //TODO
}

bool Application::isClosed() {
    return lhs->isClosed() && rhs->isClosed();
}
////////////////////////////////////////////////////////////////////////////////
