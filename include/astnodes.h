#pragma once

#include <string>
#include <vector>

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