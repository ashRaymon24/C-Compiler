#pragma once
#include "astnodes.h"
#include <fstream>
#include <unordered_map>

class CodeGenerator {
public:
    void generate(Program* program);

private:
    std::ofstream output;
    std::unordered_map<std::string, int> variableOffsets;
    std::string functionName; // Store the current function name
    int nextOffset;
    void generateFunction(Function* function);
    void generateStatement(Statement* stmt);
    void generateExpression(Expression* expr);
    
    void emit(const std::string& line);
    int countVariables(Function* function);
};