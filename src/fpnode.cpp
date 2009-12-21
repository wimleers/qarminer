#include "fpnode.h"

// Initialize static members.
unsigned int FPNode::lastNodeID = 0;

FPNode::FPNode(Item item, FPNode* parent) {
    this->nodeID = FPNode::nextNodeID();
    this->itemID = item.id;
    this->count = item.supportCount;
    this->parent = parent;
    this->itemNQs = NULL;

    // Also let the parent know it has a new child, when it is a valid parent.
    if (this->parent != NULL)
        this->parent->addChild(this);
}

FPNode::~FPNode() {
    // Delete all child nodes.
    foreach (FPNode* child, this->children)
        delete child;
    this->children.clear();
}

bool FPNode::hasChild(ItemID itemID) const {
    return this->children.contains(itemID);
}

FPNode * FPNode::getChild(ItemID itemID) const {
    if (this->children.contains(itemID))
        return this->children.value(itemID);
    else
        return NULL;
}

ItemIDFPNodeHash FPNode::getChildren() const {
    return this->children;
}

void FPNode::addChild(FPNode* child) {
    this->children.insert(child->getItemID(), child);
}

void FPNode::removeChild(FPNode* child) {
    if (this->children.contains(itemID))
        this->children.remove(child->getItemID());
}



#ifdef DEBUG
QDebug operator<<(QDebug dbg, const FPNode &node) {
    if (node.getItemID() == ROOT_ITEMID)
        dbg.nospace() << "(NULL)";
    else {
        QString item, count, ID;
        ItemNQHash* itemNQs = node.getItemNQs();
        if (itemNQs != NULL)
            item.sprintf("%  5s:% 3d", (*itemNQs)[node.getItemID()].name.toStdString().c_str(), (*itemNQs)[node.getItemID()].quantity);
        else
            item.sprintf("%  5d", node.getItemID());
        count.sprintf("#%03d", node.getCount());
        ID.sprintf("0x%04d", node.getID());
        dbg.nospace() << item.toStdString().c_str() << " (" << count.toStdString().c_str() << "," << ID.toStdString().c_str() <<  ")";
    }

    return dbg.nospace();
}

QDebug operator<<(QDebug dbg, const FPNodeList &itemPath) {
    dbg.nospace() << "[size=" << itemPath.size() << "]";

    for (int i = 0; i < itemPath.size(); i++) {
        if (i > 0)
            dbg.nospace() << "->";
        dbg.nospace() << *(itemPath[i]);
    }

    return dbg.nospace();
}
#endif
