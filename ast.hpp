#ifndef AST_HPP
#define AST_HPP
#include <string>

class ASTNode {
  public:
    virtual ~ASTNode () {};
    virtual std::string toString() = 0;
};

class Variable : public ASTNode {
  public:
    Variable  (char* _name) : name(_name) {}
    virtual ~Variable ();
    virtual std::string toString();
    
    char* name;
};

class Abstraction : public ASTNode {
  public:
    Abstraction (Variable* _v, ASTNode* _n) : v(_v), n(_n) {}
    virtual ~Abstraction ();
    virtual std::string toString();
    
    Variable* v;
    ASTNode* n;
};

class Application : public ASTNode {
  public:
    Application (ASTNode* _lhs, ASTNode* _rhs) : lhs(_lhs), rhs(_rhs) {}
    virtual ~Application ();
    virtual std::string toString();
    
    ASTNode* lhs;
    ASTNode* rhs;
};
#endif
