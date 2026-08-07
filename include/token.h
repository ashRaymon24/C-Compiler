#pragma once
#include <string>

enum class TokenType {
    INT,
    RETURN,

    IF,
    ELSE,
    WHILE,
    FOR,

    IDENTIFIER,
    INTEGER,

    LEFT_PAREN,
    RIGHT_PAREN,

    LEFT_BRACE,
    RIGHT_BRACE,

    COMMA,
    SEMICOLON,

    PLUS,
    MINUS,
    STAR,
    SLASH,
    ASSIGN,
    NOT,

    NOTEQUALTO,
    EQUALTO,
    GREATERTHAN,
    LESSTHAN,
    GREATEREQUALTO,
    LESSEQUALTO,

    END_OF_FILE,

    UNKNOWN
};


struct Token{
    TokenType type;
    std::string lexeme;
    int line;

    Token(TokenType type, const std::string& lexeme, int line)
        : type(type), lexeme(lexeme), line(line) {}
};