#ifndef AST_HPP
#define AST_HPP
#include <string>

class ASTNode {
  public:
    virtual ~ASTNode () {};
    virtual std::string toString() = 0;
    virtual void alphaReduce(std::string oldname = "", std::string newname = "") = 0;
    virtual void betaReduce() = 0;
    
    //NB: This function only works following an alpha-reduction
    virtual bool isClosed() = 0;
};

class Variable : public ASTNode {
  public:
    Variable  (char* _name) : name(_name), hasUniqueName(false) {}
    virtual ~Variable ();
    virtual std::string toString();
    virtual void alphaReduce(std::string oldname = "", std::string newname = "");
    virtual void betaReduce();
    virtual bool isClosed();
    
    char* name;
    bool hasUniqueName;
};

class Abstraction : public ASTNode {
  public:
    Abstraction (Variable* _v, ASTNode* _n) : v(_v), n(_n) {}
    virtual ~Abstraction ();
    virtual std::string toString();
    virtual void alphaReduce(std::string oldname = "", std::string newname = "");
    virtual void betaReduce();
    virtual bool isClosed();
    
    Variable* v;
    ASTNode* n;
};

class Application : public ASTNode {
  public:
    Application (ASTNode* _lhs, ASTNode* _rhs) : lhs(_lhs), rhs(_rhs) {}
    virtual ~Application ();
    virtual std::string toString();
    virtual void alphaReduce(std::string oldname = "", std::string newname = "");
    virtual void betaReduce();
    virtual bool isClosed();
    
    ASTNode* lhs;
    ASTNode* rhs;
};
#endif
