#pragma once

#include <string>
#include <vector>
#include "token.h"

class Function;
class Statement;
class Expression;

class Program {
public:
    std::vector<Function*> functions;
};

class Statement {
public:
    virtual ~Statement() = default;
};

class Expression {
public:
    virtual ~Expression() = default;
};

class Function {
public:
    std::string name;
    std::vector<Statement*> body;
};

class ReturnStatement : public Statement {
public:
    Expression* value;
};

class IntegerLiteral : public Expression {
public:
    int value;
};

class BinaryExpression : public Expression {
public:
    Expression* left;
    Token op; 
    Expression* right;
};

class VariableDeclaration : public Statement {
public:
    std::string name;
    Expression* initializer;
};

class VariableExpression : public Expression {
public:
    std::string name;
};

class AssignmentStatement : public Statement {
public:
    std::string name;
    Expression* value;
};

class IfStatement : public Statement {
public:
    Expression* condition;
    std::vector<Statement*> body;
};

class WhileStatement : public Statement {
public:
    Expression* condition;
    std::vector<Statement*> body;
};