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

std::vector<Token> tokenize(const std::string& pattern);