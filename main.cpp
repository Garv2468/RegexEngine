#include <iostream>
#include "parser.hpp"
#include <queue>
#include <unordered_map>
#include "nfa.hpp"

void printTree(Node* node, int indent = 0){
    std::string pad(indent, ' ');
    if(!node){ std::cout << pad << "null\n"; return; }
    
    switch(node->kind){
        case Nodekind::LITERAL_NODE:
            std::cout << pad << "Literal: " << static_cast<LiteralNode*>(node)->ch << "\n";
            break;
        case Nodekind::CONCAT_NODE:{
            std::cout << pad << "Concat\n";
            for(auto& child : static_cast<ConcatNode*>(node)->parts)
                printTree(child.get(), indent+2);
            break;
        }
        case Nodekind::GROUP_NODE:{
            auto* g = static_cast<GroupNode*>(node);
            std::cout << pad << "Group(" << g->groupnum << ")\n";
            printTree(g->inner.get(), indent+2);
            break;
        }
        case Nodekind::STAR_NODE:
            std::cout << pad << "Star\n";
            printTree(static_cast<StarNode*>(node)->op.get(), indent+2);
            break;
        case Nodekind::DOT_NODE:
            std::cout << pad << "Dot\n"; break;
        case Nodekind::BACKREF_NODE:
            std::cout << pad << "Backref: " << static_cast<BackrefNode*>(node)->num << "\n"; break;
        case Nodekind::SQUARE_BRACK_NODE:
            std::cout << pad << "Bracket\n"; break;
    }
}

void describeEdge(const Edge& e) {
    if (e.epsilon) { std::cout << "epsilon"; return; }
    if (e.any) { std::cout << "any (.)"; return; }
    if (e.backrefGroup != -1) { std::cout << "backref \\" << e.backrefGroup; return; }
    if (!e.charset.empty()) {
        std::cout << (e.negate ? "bracket [^" : "bracket [");
        for (char c : e.charset) std::cout << c;
        std::cout << "]";
        return;
    }
    std::cout << "literal '" << e.label << "'";
}

void printNFA(Fragment frag) {
    std::unordered_map<State*, int> ids;
    std::queue<State*> q;
    q.push(frag.start);
    ids[frag.start] = 0;
    int nextId = 1;

    std::vector<State*> order;
    while (!q.empty()) {
        State* cur = q.front(); q.pop();
        order.push_back(cur);
        for (auto& e : cur->edges) {
            if (!ids.count(e.to)) {
                ids[e.to] = nextId++;
                q.push(e.to);
            }
        }
    }

    for (State* s : order) {
        std::cout << "S" << ids[s] << (s == frag.end ? " (END)" : "") << ":\n";
        for (auto& e : s->edges) {
            std::cout << "  --[";
            describeEdge(e);
            std::cout << "]--> S" << ids[e.to] << "\n";
        }
    }
    std::cout << "Total states reachable: " << order.size() << "\n";
}

int main(){
    Tokenizer t("a\\(12\\)\\1");
    std::vector<Token> tokens = t.tokenize();
    //for(auto x : tokens) std::cout << tokTypeStr(x.type) << "\n";
    Parser p(tokens);
    ParseResult res = p.parse();
    printTree(res.root.get());
    NFA nfa;
    Fragment frag = nfa.build(res.root.get());
    printNFA(frag);

    if (!nfa.groupBoundaries.empty()) {
        std::cout << "Group boundaries recorded:\n";
        for (auto& [num, pair] : nfa.groupBoundaries)
            std::cout << "  group " << num << ": start/end states recorded\n";
    }
}