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


Expression* Parser::parseExpression() {
    if (check(TokenType::INTEGER)) {
        Token token = advance();

        auto* literal = new IntegerLiteral();
        literal->value = std::stoi(token.lexeme);

        return literal;
    }

    throw std::runtime_error("Expected expression.");
}


Statement* Parser::parseStatement() {
    if (match(TokenType::RETURN)) {
        auto* stmt = new ReturnStatement();

        stmt->value = parseExpression();

        consume(TokenType::SEMICOLON);

        return stmt;
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