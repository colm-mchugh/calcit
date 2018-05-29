
#ifndef EXPR_H_
#define EXPR_H_

#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <cassert>

#include "list.h"

#define MAX_TOKEN_LEN 128

enum class expr_opcode { OP_PLUS, OP_MINUS, OP_MULT, OP_DIV, OP_MOD,
						OP_EQ, OP_NE, OP_GT, OP_LT, OP_GTE, OP_LTE,
						OP_AND, OP_OR, OP_NOT, OP_UNKNOWN
					};

enum class expr_type { INTEGER, DOUBLE, BOOLEAN, VOID, UNRESOLVED };

// expr - Base class for all expression nodes.
class expr {
public:
  virtual ~expr() = default;
};

class typed_expr :public expr {
protected:
	expr_type etype;
public:
	typed_expr(expr_type etype) : etype(etype) {}

	void setType(expr_type etype) { this->etype = etype; }
	expr_type getType() { return this->etype; }
};

/// FltLiteral - Expression class for numeric literals like "1.0".
class FltLiteral : public typed_expr {
  double Val;

public:
  FltLiteral(double Val) : typed_expr(expr_type::DOUBLE), Val(Val) {}
  FltLiteral(const char* Val) : typed_expr(expr_type::DOUBLE), Val(atof(Val)) {}
};

/// FltLiteral - Expression class for numeric literals like "1.0".
class IntLiteral : public typed_expr {
  int Val;

public:
  IntLiteral(int Val) : typed_expr(expr_type::INTEGER), Val(Val) {}
  IntLiteral(const char* Val) : typed_expr(expr_type::INTEGER), Val(atoi(Val)) {}
};

/// BoolLiteral - Expression class for boolean literals like True, False
class BoolLiteral : public typed_expr {
  bool Val;

public:
  BoolLiteral(bool Val) : typed_expr(expr_type::BOOLEAN), Val(Val) {}
};

/// VarRef - Expression class for referencing a variable, like "a".
class VarRef : public typed_expr {
  std::string Name;

public:
  VarRef(const std::string &Name) : typed_expr(expr_type::UNRESOLVED), Name(Name) {}
  VarRef(const char* Name) : typed_expr(expr_type::UNRESOLVED), Name(Name) {}

  const std::string &getName() const { return Name; }
};

/// UnaryOp - Expression class for a unary operator.
class UnaryOp : public typed_expr {
	expr_opcode Opcode;
  std::unique_ptr<expr> Operand;

public:
  UnaryOp(expr_opcode Opcode, std::unique_ptr<expr> Operand)
      : typed_expr(expr_type::UNRESOLVED), Opcode(Opcode), Operand(std::move(Operand)) {}

};

/// BinaryOp - Expression class for a binary operator.
class BinaryOp : public typed_expr {
public:
	expr_opcode Opcode;
  std::unique_ptr<expr> LHS, RHS;

public:
  BinaryOp(expr_opcode Opcode, std::unique_ptr<expr> LHS,
                std::unique_ptr<expr> RHS)
      : typed_expr(expr_type::UNRESOLVED), Opcode(Opcode), LHS(std::move(LHS)), RHS(std::move(RHS)) {}

};

/// call - Expression class for function calls.
class Call : public expr {
  std::string Callee;
  std::vector<std::unique_ptr<expr>> Args;

public:
  Call(const std::string &Callee,
              std::vector<std::unique_ptr<expr>> Args)
      : Callee(Callee), Args(std::move(Args)) {}

};

/// call - Expression class for function calls.
class Fcall : public expr {
  std::unique_ptr<expr> Callee;
  List* Args;

public:
  Fcall(std::unique_ptr<expr> Callee, List* Args)
      : Callee(std::move(Callee)), Args(Args) {}

};

/// if - Expression class for if/then/else.
class If : public expr {
  std::unique_ptr<expr> Cond, Then, Else;

public:
  If(std::unique_ptr<expr> Cond, std::unique_ptr<expr> Then,
            std::unique_ptr<expr> Else)
      : Cond(std::move(Cond)), Then(std::move(Then)), Else(std::move(Else)) {}

};

class While : public expr {
	std::unique_ptr<expr> Cond, Body;
public:
	While(std::unique_ptr<expr> Cond, std::unique_ptr<expr>  Body)
	      : Cond(std::move(Cond)), Body(std::move(Body)) {}

};

/// Forexpr - Expression class for for/in.
class Forexpr : public expr {
  std::string VarName;
  std::unique_ptr<expr> Start, End, Step, Body;

public:
  Forexpr(const std::string &VarName, std::unique_ptr<expr> Start,
             std::unique_ptr<expr> End, std::unique_ptr<expr> Step,
             std::unique_ptr<expr> Body)
      : VarName(VarName), Start(std::move(Start)), End(std::move(End)),
        Step(std::move(Step)), Body(std::move(Body)) {}

};

/// Varexpr - Expression class for var/in
class Vardef : public expr {
  std::unique_ptr<expr> Name;
  std::unique_ptr<expr> Value;

public:
  Vardef(std::unique_ptr<expr> Name, std::unique_ptr<expr> Value)
      : Name(std::move(Name)), Value(std::move(Value)) {}

};

/// Varexpr - Expression class for var/in
class Varexpr : public expr {
  std::vector<std::pair<std::string, std::unique_ptr<expr>>> VarNames;
  std::unique_ptr<expr> Body;

public:
  Varexpr(
      std::vector<std::pair<std::string, std::unique_ptr<expr>>> VarNames,
      std::unique_ptr<expr> Body)
      : VarNames(std::move(VarNames)), Body(std::move(Body)) {}

};

/// Prototype - This class represents the "prototype" for a function,
/// which captures its name, and its argument names (thus implicitly the number
/// of arguments the function takes), as well as if it is an operator.
class Prototype {
  std::string Name;
  std::vector<std::string> Args;
  bool IsOperator;
  unsigned Precedence; // Precedence if a binary op.

public:
  Prototype(const std::string &Name, std::vector<std::string> Args,
               bool IsOperator = false, unsigned Prec = 0)
      : Name(Name), Args(std::move(Args)), IsOperator(IsOperator),
        Precedence(Prec) {}

  const std::string &getName() const { return Name; }

  bool isUnaryOp() const { return IsOperator && Args.size() == 1; }
  bool isBinaryOp() const { return IsOperator && Args.size() == 2; }

  char getOperatorName() const {
    assert(isUnaryOp() || isBinaryOp());
    return Name[Name.size() - 1];
  }

  unsigned getBinaryPrecedence() const { return Precedence; }
};

/// FuncDef - This class represents a function definition itself.
class FuncDef : public expr {
  std::unique_ptr<Prototype> Proto;
  std::unique_ptr<expr> Body;

public:
  FuncDef(std::unique_ptr<Prototype> Proto,
              std::unique_ptr<expr> Body)
      : Proto(std::move(Proto)), Body(std::move(Body)) {}

};

class Suite : public expr {
	std::vector<std::unique_ptr<expr>> stmts;
    List	*stmts_l;
public:
	Suite(std::vector<std::unique_ptr<expr>> stmts)
	      : stmts(std::move(stmts)), stmts_l(NULL) {}
	Suite(List* stmts_l)
		      : stmts(), stmts_l(stmts_l) {}
};

class Return : public expr {
	std::unique_ptr<expr> Operand;
public:
	Return(std::unique_ptr<expr> Operand)
	      : Operand(std::move(Operand)) {}
};

class Break : public expr {

};

class Continue : public expr {

};

class Fnctn : public expr {
	std::unique_ptr<VarRef> name;
	List*	params;
	std::unique_ptr<expr> Body;

public:
	Fnctn(std::unique_ptr<VarRef> name, List *params,
              std::unique_ptr<expr> Body)
      : name(std::move(name)), params(params), Body(std::move(Body)) {}

};

class SyntaxErrNode : public expr {
	std::string error_message;
public:
	SyntaxErrNode(const char* err) : error_message(err) {}
};


#endif /* EXPR_H_ */
