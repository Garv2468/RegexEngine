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
    start->edges.push_back(e);
    res.end = end;
    res.start = start;
    return res;
}

Fragment NFA::buildStar(StarNode* s){//
    State* end = makeState();
    Edge e2; Edge e3; Edge e4;
    Fragment child = build(s->op.get());
    Fragment res;

    e2.to = end;
    e2.epsilon = true;

    child.start->edges.push_back(e2); 

    e3.to = end;
    e3.epsilon = true;

    child.end->edges.push_back(e3);

    e4.to = child.start;
    e4.epsilon = true;

    child.end->edges.push_back(e4);

    res.end = end;
    res.start = child.start;
    return res;
}

Fragment NFA::buildConcat(ConcatNode* c){
    Fragment res;
    res = build(c->parts[0].get());
    for(int i = 1; i < int(c->parts.size()); i++){
        Fragment child = build(c->parts[i].get());
        Edge e;
        e.to = child.start;
        e.epsilon = true;
        res.end->edges.push_back(e);
        res.end = child.end;
    }
    return res;
}

Fragment NFA::buildBackref(BackrefNode* bf){
    State* start = makeState(); State* end = makeState();
    Edge e;
    Fragment res;
    e.to = end;
    e.backrefGroup = bf->num;
    start->edges.push_back(e);
    res.end = end;
    res.start = start;
    return res;
}

Fragment NFA::buildGroup(GroupNode* g){
    Fragment inner = build(g->inner.get());
    groupBoundaries[g->groupnum] = {inner.start, inner.end};
    return inner;
}

Fragment NFA::buildBracket(SquarebrackNode* s){//
    State* start = makeState(); State* end = makeState();
    Edge e;
    Fragment res;
    e.to = end;
    for(auto i : s->str) e.charset.insert(i);
    e.negate = s->neg;
    start->edges.push_back(e);
    res.end = end;
    res.start = start;
    return res;
}