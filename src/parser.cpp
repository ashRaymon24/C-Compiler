#include "parser.h"
#include "astnodes.h"
#include "token.h"
#include <stdexcept>


Token Parser::peek() const {
    return tokens[current];
}

Token Parser::previous() const {
    return tokens[current - 1];
}

bool Parser::isAtEnd() const {
    return peek().type == TokenType::END_OF_FILE;
}

Token Parser::advance() {
    if (!isAtEnd()) {
        current++;
    }

    return previous();
}

bool Parser::check(TokenType type) const {
    if (isAtEnd()) {
        return false;
    }

    return peek().type == type;
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }

    return false;
}

Token Parser::consume(TokenType type) {
    if (check(type)) {
        return advance();
    }

    throw std::runtime_error("Unexpected token.");
}

Expression* Parser::parsePrimary() {

    if (match(TokenType::INTEGER)) {
        Token token = previous();

        auto* literal = new IntegerLiteral();
        literal->value = std::stoi(token.lexeme);

        return literal;
    }
    else if (match(TokenType::IDENTIFIER)) {
        Token token = previous();

        auto* variable = new VariableExpression();
        variable->name = token.lexeme;

        return variable;
    }

    throw std::runtime_error("Expected primary expression.");
}

Expression* Parser::parseExpression() {

    return parseComparison();
}
Expression* Parser::parseComparison() {
    Expression* left = parseTerm();
    while (
        match(TokenType::NOTEQUALTO) ||
        match(TokenType::EQUALTO) ||
        match(TokenType::GREATERTHAN) ||
        match(TokenType::LESSTHAN) ||
        match(TokenType::GREATEREQUALTO) ||
        match(TokenType::LESSEQUALTO)
    )
    {
        Token op = previous();

        Expression* right = parseTerm();

        auto* binary = new BinaryExpression();
        binary->left = left;
        binary->op = op;
        binary->right = right;

        left = binary;
    }

    return left;
}
Expression* Parser::parseTerm() {
    Expression* left = parseFactor();
    while (match(TokenType::PLUS) || match(TokenType::MINUS)) {
        Token op = previous();
        Expression* right = parseFactor();

        auto* binary = new BinaryExpression();
        binary->left = left;
        binary->op = op;
        binary->right = right;

        left = binary;
    }
    return left;
}
Expression* Parser::parseFactor() {
    Expression* left = parsePrimary();
    while (match(TokenType::STAR) || match(TokenType::SLASH)) {
        Token op = previous();
        Expression* right = parsePrimary();

        auto* binary = new BinaryExpression();
        binary->left = left;
        binary->op = op;
        binary->right = right;

        left = binary;
    }
    return left;
}



Statement* Parser::parseStatement() {
    if (match(TokenType::RETURN)) {
        auto* stmt = new ReturnStatement();

        stmt->value = parseExpression();

        consume(TokenType::SEMICOLON);

        return stmt;
    }
    else if (match(TokenType::INT)){
        Token id = consume(TokenType::IDENTIFIER);
        consume(TokenType::ASSIGN);
        Expression* exp = parseExpression();
        consume(TokenType::SEMICOLON);
        auto* decl = new VariableDeclaration();
        decl->name = id.lexeme;
        decl->initializer = exp;

        return decl;
    }
    else if (match(TokenType::IDENTIFIER)) {
        Token id = previous();
        consume(TokenType::ASSIGN);
        Expression* exp = parseExpression();
        consume(TokenType::SEMICOLON);
        auto* assign = new AssignmentStatement();
        assign->name = id.lexeme;
        assign->value = exp;

        return assign;
    }
    else if (match(TokenType::IF)) {
        consume(TokenType::LEFT_PAREN);
        Expression* condition = parseExpression();
        consume(TokenType::RIGHT_PAREN);
        consume(TokenType::LEFT_BRACE);

        auto* ifStmt = new IfStatement();
        ifStmt->condition = condition;

        while (!check(TokenType::RIGHT_BRACE)) {
            ifStmt->body.push_back(parseStatement());
        }

        consume(TokenType::RIGHT_BRACE);
        
        if (match(TokenType::ELSE))
        {
            consume(TokenType::LEFT_BRACE);

            while (!check(TokenType::RIGHT_BRACE))
            {
                ifStmt->elseBody.push_back(parseStatement());
            }

            consume(TokenType::RIGHT_BRACE);
        }
        return ifStmt;
    }
    else if (match(TokenType::WHILE)){
        consume(TokenType::LEFT_PAREN);
        Expression* condition = parseExpression();
        consume(TokenType::RIGHT_PAREN);

        consume(TokenType::LEFT_BRACE);

        auto* whileStmt = new WhileStatement();
        whileStmt->condition = condition;

        while (!check(TokenType::RIGHT_BRACE)) {
            whileStmt->body.push_back(parseStatement());
        }

        consume(TokenType::RIGHT_BRACE);

        return whileStmt;
    }

    throw std::runtime_error("Expected statement.");
}


Function* Parser::parseFunction() {
    consume(TokenType::INT);

    Token nameToken = consume(TokenType::IDENTIFIER);

    consume(TokenType::LEFT_PAREN);
    consume(TokenType::RIGHT_PAREN);

    consume(TokenType::LEFT_BRACE);

    auto* function = new Function();
    function->name = nameToken.lexeme;

    while (!check(TokenType::RIGHT_BRACE)) {
        function->body.push_back(parseStatement());
    }

    consume(TokenType::RIGHT_BRACE);

    return function;
}



Program Parser::parse() {
    Program program;

    while (!isAtEnd()) {
        program.functions.push_back(parseFunction());
    }
    return program;
}