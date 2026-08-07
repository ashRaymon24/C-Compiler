#pragma once
#include <string>
#include <vector>
#include "token.h"

class Lexer {
    public:
        explicit Lexer(const std::string& source);

        std::vector<Token> tokenize();
    private:
        const std::string& source_;
        size_t current_ = 0;
        int line = 1;

        char advance();
        char peek() const;
        char peekNext() const;
        bool isAtEnd() const;

        void skipWhitespace();
        Token makeToken(TokenType type);
        Token identifier();
        Token number();
        Token scanToken();
    
};