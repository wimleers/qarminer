#include "fpnode.h"

FPNode::FPNode(int attribute, FPNode * parent) {
    this->attribute = attribute;
    this->parent    = parent;
    this->sibling   = NULL;
    this->count     = 1;
}

FPNode::~FPNode() {
    // Nothing to be done.
}

bool FPNode::hasChild(int attribute) {
    return this->children.contains(attribute);
}

FPNode * FPNode::getChild(int attribute) {
    if (this->hasChild(attribute))
        return this->children.value(attribute);
    else
        return NULL;
}

void FPNode::addChild(FPNode * child) {
    this->children.insert(child->getAttribute(), child);
}

void FPNode::removeChild(FPNode * child) {
    if (this->hasChild(attribute))
        this->children.remove(child->getAttribute());
}
