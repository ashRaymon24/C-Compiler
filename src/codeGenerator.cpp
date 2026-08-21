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
    int variableCount = countVariables(function);
    int byteCount = variableCount * 4; 
    int stackSize = ((byteCount + 15) / 16) * 16; // Round up to the nearest multiple of 16
    emit(function->name + ":");
    emit("push rbp");
    emit("mov rbp, rsp");
    emit("sub rsp, " + std::to_string(stackSize)); // Allocate space for local variables
    nextOffset = 4;
    variableOffsets.clear(); // Clear previous variable offsets for the new function
    for (Statement* stmt : function->body) {
        generateStatement(stmt);
    }
    emit(".Lreturn_" + function->name + ":");
    emit("mov rsp, rbp");
    emit("pop rbp");
    emit("ret");
}
void CodeGenerator::generateStatement(Statement* stmt) {
    if (auto* retStmt = dynamic_cast<ReturnStatement*>(stmt)) {
        generateExpression(retStmt->value);
        emit("jmp .Lreturn_" + function->name);
    }
    else if (auto* varDecl = dynamic_cast<VariableDeclaration*>(stmt)) {
        generateExpression(varDecl->initializer);
        variableOffsets[varDecl->name] = nextOffset; // Store variable at offset -8 from rbp
        nextOffset += 4; // Increment offset for next variable
        int offset = variableOffsets[varDecl->name];
        emit("mov DWORD PTR [rbp-" + std::to_string(offset) + "], eax");
    }
}
void CodeGenerator::generateExpression(Expression* expr) {
    if (auto* intLiteral = dynamic_cast<IntegerLiteral*>(expr)) {
        emit("mov rax, " + std::to_string(intLiteral->value));
    } 
    else if (auto* binaryExpr = dynamic_cast<BinaryExpression*>(expr)) {
        generateExpression(binaryExpr->left);
        emit("push rax");
        generateExpression(binaryExpr->right);
        emit("pop rbx");

        if (binaryExpr->op.type == TokenType::PLUS) {
            emit("add rax, rbx");
        } else if (binaryExpr->op.type == TokenType::MINUS) {
            emit("sub rbx, rax");
            emit("mov rax, rbx");
        } else if (binaryExpr->op.type == TokenType::STAR) {
            emit("imul rax, rbx");
        } else if (binaryExpr->op.type == TokenType::SLASH) {
            emit("mov rcx, rax"); // Move dividend to rcx
            emit("mov rax, rbx"); // Move divisor to rax
            emit("cqo");          // Sign-extend rax into rdx:rax
            emit("idiv rcx");     // Divide rcx by rax, result in rax
        }
    }
    else if (auto* varExpr = dynamic_cast<VariableExpression*>(expr)) {
        auto it = variableOffsets.find(varExpr->name);
        if (it != variableOffsets.end()) {
            emit("mov eax, DWORD PTR [rbp-" + std::to_string(it->second) + "]");
        }
    }
}

int CodeGenerator::countVariables(Function* function) {
    int count = 0;

    for (Statement* stmt : function->body) {
        if (dynamic_cast<VariableDeclaration*>(stmt)) {
            count++;
        }
    }

    return count;
}