Enter one term, press enter for it to be reduced.

#Syntax
- [a-z]+      is a λ-term called a variable
- (λx.y)      is a λ-term called an abstraction iif x is a variable and y is a λ-term
- (x y)       is a λ-term called an application iif x and y are λ-terms
    
L and \ may be used inplace of λ if one desires. One may mix these symbols for λ as one desires. Thus the following is a λ-term: (λx.(Ly.(\z.(y (x z)))))

#Sugar
|Name |  Usage               |   Actual Term               |
| --- | -------------------- | --------------------------- |
|cons |  ((cons a) b)        |   (λx.(λy.(λf.((f x) y))))  |
|car  |  (car ((cons a) b))  |   (λl.(l (λx.(λy.x))))      |
|cdr  |  (cdr ((cons a) b))  |   (λl.(l (λx.(λy.y))))      |

#REPL Commands
- :quit       This has the effect of quitting the REPL.
- :debug      This toggles debug mode. Debug mode prints reduction information

#Examples
- (((λx.(λy.(x (x y)))) (λx.y)) z) ⇒ y
- ((λx.(x y)) (λx.x)) ⇒ y
- ((λf.f) (λf.(f f))) ⇒ (λf.(f f))
- (((λx.(λy.(λf.((f x) y)))) a) b) ⇒ (λ4.((4 a) b))
- ((λx.(x (λy.(λz.y)))) (((λx.(λy.(λf.((f x) y)))) a) b)) ⇒ a
- ((λx.(x (λy.(λz.z)))) (((λx.(λy.(λf.((f x) y)))) a) b)) ⇒ b
- (car ((cons a) b)) ⇒ a
- (cdr ((cons a) b)) ⇒ b

#Present features implemented
- α-Reduction
- β-Reduction
- Debug mode
- Sugar for cons/car/cdr

#Planned features
- true/false
- if
- nil?
- Y-combinator
- recursive function to reverse a list
- pretty-print lists
- pretty-print true and false
- think about how to let the user define functions with names, consider let
- map, fold
- No stack-depth limit, rather the limit is the available memory.
  - See Application::betaReduce recursively β-reducing the lhs and rhs
  - Walk the AST without recursion
- Line editing and history, maybe via GNU readline
- Sugar currying to accept (λx y.(y x))
- Church numerals
- successor/predessessor
- +/-
- *// (integer division only at first)

#Limits
- 2^64 variables IDs, aborts on exceeding this limit
- Recursion depth is limited by available memory, due to this trying to β-reduce Ω is a very bad idea.
- Variables cannot have the following names:
  - cons
  - car
  - cdr
