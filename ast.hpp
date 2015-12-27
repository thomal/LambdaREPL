#include <string>

class ASTNode {
  public:
    virtual ~ASTNode () {}
    virtual std::string toString() = 0;
};

class Variable : ASTNode {
  public:
    Variable  (char* _name) : name(_name) {}
    virtual ~Variable ();
    virtual std::string toString();
    
    char* name;
};

class Lambda : ASTNode {
  public:
    Lambda (ASTNode* _v, ASTNode* _n) : v(_v), n(_n) {}
    virtual ~Lambda ();
    virtual std::string toString();
    
    ASTNode* v;
    ASTNode* n;
};

class Application : ASTNode {
  public:
    Application (ASTNode* _lhs, ASTNode* _rhs) : lhs(_lhs), rhs(_rhs) {}
    virtual ~Application ();
    virtual std::string toString();
    
    ASTNode* lhs;
    ASTNode* rhs;
};
