#include "ast.hpp"
#include <string>
#include <stdlib.h> //exit(1)
#include <string.h> //strdup
#include <iostream>
#include <sstream>  //ostringstream
#include <stdint.h> //uint64_t
using namespace std;
#define IFDEBUG(X)

//////// ID GENERATION /////////////////////////////////////////////////////////
typedef uint64_t vid_t; //Maximum number of IDs is defined by this
vid_t globallyUniqueID = 0;

std::string newID () {
    id_t oldid = globallyUniqueID;
    id_t newid = ++globallyUniqueID;
    if (newid > oldid) {
        std::ostringstream oss;
        oss << newid;
        return oss.str();
    } else { //Overflow
        cout << "Limit of 2^64 unique IDs exceeded.\n";
        exit(1);
    }
}
////////////////////////////////////////////////////////////////////////////////

//////// RTTI STUFF THAT OUGHTN'T BE IN THE BASE CLASS /////////////////////////
void setNodesAOPPTTNIfApplicable (ASTNode* n, ASTNode** aoppttn) {
    Application* a = dynamic_cast<Application*>(n);
    if (a) {
        a->addressOfParentsPointerToThisNode = aoppttn;
    }
    Variable* v = dynamic_cast<Variable*>(n);
    if (v) {
        v->addressOfParentsPointerToThisNode = aoppttn;
    }
}
////////////////////////////////////////////////////////////////////////////////

//////// VARIABLE //////////////////////////////////////////////////////////////
Variable::~Variable() {
    free(name);
}

std::string Variable::toString() {
    return std::string(name);
}

void Variable::alphaReduce(std::string oldname, std::string newname) {
    IFDEBUG(string callID = newID();)
    IFDEBUG(cout << "START var alpha reduce " << callID << "\t" << toString() << "\n";)
    
    if (strcmp(name, oldname.c_str())==0) {
        free(name);
        name = strdup(newname.c_str());
        hasUniqueName = true;
    }
    
    IFDEBUG(cout << "END   var alpha reduce " << callID << "\t" << toString() << "\n";)
}

void Variable::betaReduce() {
    IFDEBUG(string callID = newID();)
    IFDEBUG(cout << "NOP   var beta  reduce " << callID << "\t" << toString() << "\n";)
    
    //nop
}

bool Variable::isClosed() {
    return hasUniqueName;
}

void Variable::setParentPointers() {
    //nop, a variable has no children
}

void Variable::performSubstitution(std::string vid, ASTNode* nodeToCopy) {
    if (strcmp(name, vid.c_str())==0) {
        //Deep copy the thing we're being replaced with
        ASTNode* n = nodeToCopy->copyForSubstitution (
                                             addressOfParentsPointerToThisNode);
        //There's no need to n->alphaReduce() because it is already alphaReduced
        //  with respect to itself.
        //And replace ourselves with it
        *addressOfParentsPointerToThisNode = n;
        //Then delete ourselves since we no longer have a parent to do so
        delete this;
    }
}

ASTNode* Variable::copyForSubstitution(ASTNode** aoppttn) {
    Variable* v = new Variable(strdup(name));
    v->addressOfParentsPointerToThisNode = aoppttn;
    v->setParentPointers();
    v->alphaReduce();
    return v;
}
////////////////////////////////////////////////////////////////////////////////

//////// ABSTRACTION ///////////////////////////////////////////////////////////
Abstraction::~Abstraction() {
    delete v;
    if (!preserveRHS)
        delete n;
}

std::string Abstraction::toString() {
    return "(λ" + v->toString() + "." + n->toString() + ")";
}

void Abstraction::alphaReduce(std::string oldname, std::string newname) {
    IFDEBUG(string callID = newID();)
    IFDEBUG(cout << "START abs alpha reduce " << callID << "\t" << toString() << "\n";)
    
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
    
    IFDEBUG(cout << "END   abs alpha reduce " << callID << "\t" << toString() << "\n";)
}

void Abstraction::betaReduce() {
    IFDEBUG(string callID = newID();)
    IFDEBUG(cout << "START abs beta  reduce " << callID << "\t" << toString() << "\n";)

    //No need to β-reduce a variable. It's not and doesn't contain an
    //  Application.
    n->betaReduce();
    
    IFDEBUG(cout << "END   abs beta  reduce " << callID << "\t" << toString() << "\n";)
}

bool Abstraction::isClosed() {
    return n->isClosed();
}

void Abstraction::setParentPointers() {
    setNodesAOPPTTNIfApplicable(v, (ASTNode**)&v);
    setNodesAOPPTTNIfApplicable(n, &n);
    
    v->setParentPointers();
    n->setParentPointers();
}

void Abstraction::performSubstitution(std::string vid, ASTNode* nodeToCopy) {
    v->performSubstitution(vid, nodeToCopy);
    n->performSubstitution(vid, nodeToCopy);
    alphaReduce();
}

ASTNode* Abstraction::copyForSubstitution(ASTNode** aoppttn) {
    ASTNode* vcopy_ast = v->copyForSubstitution(0);
    //Variable::copyForSubstitution --always-- returns a Variable*
    Variable* vcopy = dynamic_cast<Variable*>(vcopy_ast);
    ASTNode* ncopy = n->copyForSubstitution(0);
    
    Abstraction* a = new Abstraction(vcopy, ncopy);
    
    setNodesAOPPTTNIfApplicable(a->v, (ASTNode**)&(a->v));
    setNodesAOPPTTNIfApplicable(a->n, &(a->n));
    
    vcopy->setParentPointers();
    vcopy->alphaReduce();
    
    ncopy->setParentPointers();
    ncopy->alphaReduce();
    
    return a;
}
////////////////////////////////////////////////////////////////////////////////

//////// APPLICATION ///////////////////////////////////////////////////////////
Application::~Application() {
    delete lhs;
    delete rhs;
}

std::string Application::toString() {
    return "(" + lhs->toString() + " " + rhs->toString() + ")";
}

void Application::alphaReduce(std::string oldname, std::string newname) {
    IFDEBUG(string callID = newID();)
    IFDEBUG(cout << "START app alpha reduce " << callID << "\t" << toString() << "\n";)
    
    lhs->alphaReduce(oldname, newname);
    rhs->alphaReduce(oldname, newname);
    
    IFDEBUG(cout << "END   app alpha reduce " << callID << "\t" << toString() << "\n";)
}

void Application::betaReduce() {
    std::string startStr = toString();
    IFDEBUG(string callID = newID();)
    IFDEBUG(cout << "START app beta  reduce " << callID << "\t" << toString() << "\n";)
    
    IFDEBUG(cout << "MID   app beta  reduce " << callID << "\t" << "lhs start\n";)
    lhs->betaReduce();
    IFDEBUG(cout << "MID   app beta  reduce " << callID << "\t" << "lhs end/rhs start\n";)
    rhs->betaReduce();
    IFDEBUG(cout << "MID   app beta  reduce " << callID << "\t" << "rhs end\n";)

    //If we have an abstraction on the left then we then replace all instances
    //  of the lhs's argument with copies of the β-reduced rhs, we then set our
    //  parents pointer to ourselves to be a pointer to the rhs of the lhs and
    //  delete the lhs (except its rhs) and delete ourselves and delete the rhs.
    Abstraction* a = dynamic_cast<Abstraction*>(lhs);
    if (a) {
        IFDEBUG(cout << "MID   app beta  reduce " << callID << "\t" << "substituting\n";)
        //Replace all instances of the lhs's argument with copies of the rhs
        std::string vid = std::string(a->v->name);
        a->n->performSubstitution(vid, rhs);
        IFDEBUG(cout << "MID   app beta  reduce " << callID << "\t" << a->n->toString() << "\n";)
        
        cout << startStr << " β⇒ " << a->n->toString() << "\n";
        
        //Set our parents pointer to ourselves to point to the rhs of the lhs
        *addressOfParentsPointerToThisNode = a->n;
        
        //Delete ourselves, and the lhs excluding its rhs.
        IFDEBUG(cout << "END   app beta  reduce " << callID << "\t" << "deleting self\n";)
        a->preserveRHS = true;
        delete this; //Our destructor takes care of destructing the lhs and rhs
        
        //β-reduce our replacment
        a->n->betaReduce();
    } else {
        IFDEBUG(cout << "END   app beta  reduce " << callID << "\t" << toString() << "\n";)
    }
}

bool Application::isClosed() {
    return lhs->isClosed() && rhs->isClosed();
}

void Application::setParentPointers() {
    setNodesAOPPTTNIfApplicable(lhs, &lhs);
    setNodesAOPPTTNIfApplicable(rhs, &rhs);
    
    lhs->setParentPointers();
    rhs->setParentPointers();
}

void Application::performSubstitution(std::string vid, ASTNode* nodeToCopy) {
    lhs->performSubstitution(vid, nodeToCopy);
    rhs->performSubstitution(vid, nodeToCopy);
    alphaReduce();
}

ASTNode* Application::copyForSubstitution(ASTNode** aoppttn) {
    ASTNode* lcopy = lhs->copyForSubstitution(0);
    ASTNode* rcopy = rhs->copyForSubstitution(0);
    
    Application* a = new Application(lcopy, rcopy);
    
    setNodesAOPPTTNIfApplicable(a->lhs, &(a->lhs));
    setNodesAOPPTTNIfApplicable(a->rhs, &(a->rhs));
    
    lcopy->setParentPointers();
    lcopy->alphaReduce();
    
    rcopy->setParentPointers();
    rcopy->alphaReduce();
    
    return a;
}
////////////////////////////////////////////////////////////////////////////////
