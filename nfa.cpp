#include "nfa.hpp"

State* NFA::makeState() {
    auto newState = std::make_unique<State>();

    State* rawPtr = newState.get();
    pool.push_back(std::move(newState));

    return rawPtr;
}


Fragment NFA::build(Node* n){
    switch(n->kind){
        case Nodekind::LITERAL_NODE: return buildLiteral(static_cast<LiteralNode*>(n));
        case Nodekind::STAR_NODE: return buildStar(static_cast<StarNode*>(n));
        case Nodekind::SQUARE_BRACK_NODE: return buildBracket(static_cast<SquarebrackNode*>(n));
        case Nodekind::DOT_NODE: return buildDot(static_cast<DotNode*>(n));
        case Nodekind::BACKREF_NODE: return buildBackref(static_cast<BackrefNode*>(n));
        case Nodekind::GROUP_NODE: return buildGroup(static_cast<GroupNode*>(n));
        case Nodekind::CONCAT_NODE: return buildConcat(static_cast<ConcatNode*>(n));
        default:
            return Fragment{nullptr, nullptr};
    }
}

Fragment NFA::buildLiteral(LiteralNode* l){
    State* start = makeState(); State* end = makeState();
    Edge e;
    Fragment res;
    e.label = l->ch;
    e.to = end;
    start->edges.push_back(e);
    res.end = end;
    res.start = start;
    return res;
}

Fragment NFA::buildDot(DotNode* d){//
    State* start = makeState(); State* end = makeState();
    Edge e;
    Fragment res;
    e.to = end;
    e.any = true;
    e.epsilon = true;
    start->edges.push_back(e);
    res.end = end;
    res.start = start;
    return res;
}

Fragment NFA::buildStar(StarNode* s){//
    State* start = makeState();
    Edge e;
    Fragment res;
    e.to = start;
    //e.label = s->op;
    start->edges.push_back(e);
    res.end = start;
    res.start = start;
    return res;
}