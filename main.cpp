#include <iostream>
#include "tokenizer.cpp"
#include "parser.cpp"

int main(){
    Tokenizer t("^a\\(b$\\)c");
    std::vector<Token> tokens = t.tokenize();
    for(auto x : tokens) std::cout << tokTypeStr(x.type) << "\n";
    Parser p(tokens);
    ParseResult res = p.parse();
    std:: cout << res.anchorEnd << " " << res.anchorStart;
}