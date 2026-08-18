#pragma once
#include "token.h"
#include "lexer.h"
#include "astnodes.h"
#include <vector>
#include <string>

class Parser {
    public:
        explicit Parser(const std::vector<Token>& tokens) : tokens(tokens) {};

        Program parse();

    private:

        const std::vector<Token>& tokens;
        size_t current = 0;

        Function* parseFunction();
        Statement* parseStatement();
        Expression* parseExpression();
        Expression* parsePrimary();
        Expression* parseTerm();
        Expression* parseFactor();
        Expression* parseComparison();

        Token peek() const;
        Token advance();
        Token consume(TokenType type);
        Token previous() const;

        bool check(TokenType type) const;
        bool match(TokenType type);
        bool isAtEnd() const;
};