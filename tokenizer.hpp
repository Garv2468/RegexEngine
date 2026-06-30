#pragma once
#include <string>
#include <vector>
#include <stdexcept>

enum TokType {
    LITERAL,        // any char
    DOT,            // . 
    STAR,           // *  
    ANCHOR_START,   // ^  
    ANCHOR_END,     // $ 
    LBRACK,         // \( 
    RBRACK,         // \) 
    LCURLY,         // \{ 
    RCURLY,         // \}  
    SQUARE_BRACK,   // [...]
    BACKREF,        // \1–\9 
    END             // EOF
};

struct Token {
    TokType type;
    char ch = '\0'; 
    std::string bracket;        
    int pos = 0;   
};

class Tokenizer{
    private:
    bool atEnd();
    char peek(int offset);
    char consume();
    bool dollarIsAnchor();
    Token handleEscape(int startPos);
    std::string BracketExpr(int startPos);
    std::string pattern;
    int pos = 0;

    public:
    Tokenizer(const std :: string& pattern) : pattern(pattern){}
    std::vector<Token> tokenize();
};