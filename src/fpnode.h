#ifndef FPNODE_H
#define FPNODE_H

#include <QHash>
#include <QDebug>
#include <QMetaType>
#include <QString>

#include "typedefs.h"

class FPNode {
#ifdef DEBUG
private:
    static unsigned int lastNodeID;
    static unsigned int nextNodeID() { return FPNode::lastNodeID++; }
#endif

protected:
    FPNode* parent;
    ItemIDFPNodeHash children;
    SupportCount count;
    ItemID itemID;
#ifdef DEBUG
    unsigned int nodeID;
    ItemNQHash* itemNQs;
#endif

public:
    FPNode(Item = Item(), FPNode* parent = NULL);
    ~FPNode();

    // Accessors.
    bool isRoot() const { return this->itemID == ROOT_ITEMID; }
    bool isLeaf() const { return this->children.size() == 0; }
    Item getItem() const { Item item; item.id = this->itemID; item.supportCount = this->count; return item; }
    ItemID getItemID() const { return this->itemID; }
    SupportCount getCount() const { return this->count; }
    FPNode* getParent() const { return this->parent; }
    FPNode* getChild(ItemID itemID) const;
    ItemIDFPNodeHash getChildren() const;
    bool hasChild(ItemID itemID) const;
    unsigned int numChildren() const { return this->children.size(); }

    // Modifiers.
    void addChild(FPNode* child);
    void removeChild(FPNode * child);
    void setItem(ItemID itemID) { this->itemID = itemID; };
    void setParent(FPNode* parent) { this->parent = parent; }
    void increment() { this->count++; }
    void decrement() { this->count--; }
    void increaseCount(SupportCount count) { this->count += count; }
    void decreaseCount(SupportCount count) { this->count -= count; }

#ifdef DEBUG
    // Debug output.
    unsigned int getID() const { return this->nodeID; }
    ItemNQHash* getItemNQs() const { return this->itemNQs; }
    void setItemNQs(ItemNQHash* itemNQs) { this->itemNQs = itemNQs; }
#endif
};

Q_DECLARE_METATYPE(FPNode);

#ifdef DEBUG
QDebug operator<<(QDebug dbg, const FPNode &node);
QDebug operator<<(QDebug dbg, const FPNodeList &itemPath);
#endif

#endif // FPNODE_H
