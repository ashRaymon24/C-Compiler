#pragma once
#include "astnodes.h"
#include <fstream>

class CodeGenerator {
public:
    void generate(Program* program);

private:
    std::ofstream output;
    void generateFunction(Function* function);
    void generateStatement(Statement* stmt);
    void generateExpression(Expression* expr);
    
    void emit(const std::string& line);
};