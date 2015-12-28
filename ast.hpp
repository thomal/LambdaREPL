#ifndef AST_HPP
#define AST_HPP
#include <string>

class ASTNode;

void setNodesAOPPTTNIfApplicable (ASTNode* n, ASTNode** aoppttn);

class ASTNode {
  public:
    virtual ~ASTNode () {};
    virtual std::string toString() = 0;
    virtual void alphaReduce(std::string oldname = "", std::string newname = "") = 0;
    //NB: This function cannot be invoked until setParentPointers has been
    //      invoked
    virtual void betaReduce() = 0;
    //NB: This function cannot be invoked until alphaReduce has been invoked
    virtual bool isClosed() = 0;
    //NB: This function must be called before β-reducing
    virtual void setParentPointers() = 0;
    //NB: This function cannot be invoked until alphaReduce has been invoked
    virtual void performSubstitution(std::string vid, ASTNode* nodeToCopy) = 0;
    //Performs α-reduction on a newly allocated deep-copy which it returns.
    //  Must be freed by the caller.
    virtual ASTNode* copyForSubstitution(ASTNode** aoppttn) = 0;
};

//WARNING: Variables must always be dynamically allocated because they invoke
//          "delete this" during β-reduction substitution.
class Variable : public ASTNode {
  public:
    Variable  (char* _name) : name(_name), hasUniqueName(false) {}
    virtual ~Variable ();
    virtual std::string toString();
    virtual void alphaReduce(std::string oldname = "", std::string newname = "");
    virtual void betaReduce();
    virtual bool isClosed();
    virtual void setParentPointers();
    virtual void performSubstitution(std::string vid, ASTNode* nodeToCopy);
    virtual ASTNode* copyForSubstitution(ASTNode** aoppttn);
    
    char* name;
    bool hasUniqueName;
    //Used during β-reduction substitution to replace the current Variable with
    // the term its being bound to. Set by the method setParentPointers since
    // a node has no parent at construction-time.
    ASTNode** addressOfParentsPointerToThisNode;
};

class Abstraction : public ASTNode {
  public:
    Abstraction (Variable* _v, ASTNode* _n) :
        v(_v), n(_n), preserveRHS(false) {}
    virtual ~Abstraction ();
    virtual std::string toString();
    virtual void alphaReduce(std::string oldname = "", std::string newname = "");
    virtual void betaReduce();
    virtual bool isClosed();
    virtual void setParentPointers();
    virtual void performSubstitution(std::string vid, ASTNode* nodeToCopy);
    virtual ASTNode* copyForSubstitution(ASTNode** aoppttn);
    
    Variable* v;
    ASTNode* n;
    bool preserveRHS; //If set to true the destructor doesn't delete n
};

//WARNING: Applications must always be dynamically allocated because they
//          invoke "delete this" during β-reduction.
class Application : public ASTNode {
  public:
    Application (ASTNode* _lhs, ASTNode* _rhs) :
        lhs(_lhs), rhs(_rhs) {}
    virtual ~Application ();
    virtual std::string toString();
    virtual void alphaReduce(std::string oldname = "", std::string newname = "");
    virtual void betaReduce();
    virtual bool isClosed();
    virtual void setParentPointers();
    virtual void performSubstitution(std::string vid, ASTNode* nodeToCopy);
    virtual ASTNode* copyForSubstitution(ASTNode** aoppttn);
    
    ASTNode* lhs;
    ASTNode* rhs;
    //Used during β-reduction to replace the current application with the term
    // resulting from the β-reduction. Set by the method setParentPointers since
    // a node has no parent at construction-time.
    ASTNode** addressOfParentsPointerToThisNode;
};
#endif
