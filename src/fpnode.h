#ifndef FPNODE_H
#define FPNODE_H

#include <QHash>
#include <QDebug>
#include <QMetaType>
#include <QString>

#include "typedefs.h"

class FPNode {
private:
    static unsigned int lastNodeID;
    static unsigned int nextNodeID() { return FPNode::lastNodeID++; }

protected:
    unsigned int ID;
    FPNode* parent;
    ItemFPNodeHash children;
    int count;
    int item;
    ItemNameHash* itemNames;

public:
    FPNode(Item item = ROOT_ITEM, FPNode* parent = NULL);
    ~FPNode();

    // Accessors.
    bool isRoot() const { return this->item == ROOT_ITEM; }
    bool isLeaf() const { return this->children.size() == 0; }
    int getID() const { return this->ID; }
    int getItem() const { return this->item; }
    int getCount() const { return this->count; }
    FPNode* getParent() const { return this->parent; }
    FPNode* getChild(Item item) const;
    ItemFPNodeHash getChildren() const;
    bool hasChild(Item item) const;
    int numChildren() const { return this->children.size(); }
    ItemNameHash* getItemNames() const { return this->itemNames; }

    // Modifiers.
    void addChild(FPNode* child);
    void removeChild(FPNode * child);
    void setitem(Item item) { this->item = item; };
    void setParent(FPNode* parent) { this->parent = parent; }
    void increment() { this->count++; }
    void decrement() { this->count--; }
    void setItemNames(ItemNameHash* itemNames) { this->itemNames = itemNames; }
};

Q_DECLARE_METATYPE(FPNode);

QDebug operator<<(QDebug dbg, const FPNode &node);

#endif // FPNODE_H
