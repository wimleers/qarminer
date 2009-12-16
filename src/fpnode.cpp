#include "fpnode.h"

// Initialize static members.
unsigned int FPNode::lastNodeID = 0;

FPNode::FPNode(int item, FPNode* parent) {
    this->ID = FPNode::nextNodeID();
    this->item = item;
    this->parent = parent;
    this->count = (item != ROOT_ITEM) ? 1 : 0; // When it's the root item, initialize the count to 0.

    // Also let the parent know it has a new child, when it is a valid parent.
    if (this->parent != NULL)
        this->parent->addChild(this);
}

FPNode::~FPNode() {
    // Delete all child nodes.
    foreach (FPNode * child, this->children)
        delete child;
    this->children.clear();
}

bool FPNode::hasChild(int item) const {
    return this->children.contains(item);
}

FPNode * FPNode::getChild(int item) const {
    if (this->children.contains(item))
        return this->children.value(item);
    else
        return NULL;
}

QHash<int, FPNode*> FPNode::getChildren() const {
    return this->children;
}

void FPNode::addChild(FPNode * child) {
    this->children.insert(child->getItem(), child);
}

void FPNode::removeChild(FPNode * child) {
    if (this->children.contains(item))
        this->children.remove(child->getItem());
}




QDebug operator<<(QDebug dbg, const FPNode &node) {
    if (node.getItem() == ROOT_ITEM)
        dbg.nospace() << "(NULL)";
    else {
        QString item, count, ID;
        item.sprintf("%  5d", node.getItem());
        count.sprintf("#%03d", node.getCount());
        ID.sprintf("0x%04d", node.getID());
        dbg.nospace() << item.toStdString().c_str() << " (" << count.toStdString().c_str() << "," << ID.toStdString().c_str() <<  ")";
    }

    return dbg.nospace();
}
