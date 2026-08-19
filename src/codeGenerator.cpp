#include "codeGenerator.h" 

void CodeGenerator::emit(const std::string& line) {
    output << line << std::endl;
}

void CodeGenerator::generate(Program* program) {
    output.open("output.s");
    emit(".intel_syntax noprefix");
    emit(".global main");
    emit("");
    for (Function* function : program->functions) {
        generateFunction(function);
    }
    output.close();
}
void CodeGenerator::generateFunction(Function* function) {
    emit(function->name + ":");
    for (Statement* stmt : function->body) {
        generateStatement(stmt);
    }
}
void CodeGenerator::generateStatement(Statement* stmt) {
    if (auto* retStmt = dynamic_cast<ReturnStatement*>(stmt)) {
        generateExpression(retStmt->value);
        emit("ret");
    }
}
void CodeGenerator::generateExpression(Expression* expr) {
    if (auto* intLiteral = dynamic_cast<IntegerLiteral*>(expr)) {
        emit("mov eax, " + std::to_string(intLiteral->value));
    } 
}