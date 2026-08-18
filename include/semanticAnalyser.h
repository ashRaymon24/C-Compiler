#pragma once
#include "astnodes.h"
#include <unordered_set>

class SemanticAnalyser{
    public:
        void analyse(Program& program);
    private:
        std::unordered_set<std::string> declaredVariables;
        void analyseExpression(Expression* expr);
        void analyseStatement(Statement* stmt);
};
