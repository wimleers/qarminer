#ifndef FPNODE_H
#define FPNODE_H

#include <QHash>
#include <QDebug>
#include <QMetaType>
#include <QString>

#define ROOT_ITEM -1

class FPNode {
protected:
    FPNode * parent;
    QHash<int, FPNode*> children;
    int count;
    int item;

public:
    FPNode(int item = ROOT_ITEM, FPNode* parent = NULL);
    ~FPNode();

    bool isRoot() const { return this->item == ROOT_ITEM; }
    bool isLeaf() const { return this->children.size() == 0; }

    int getItem() const { return this->item; }
    int getCount() const { return this->count; }
    FPNode* getParent() const { return this->parent; }
    FPNode* getChild(int item) const;
    QHash<int, FPNode*> getChildren() const;
    bool hasChild(int item) const;
    int numChildren() const { return this->children.size(); }

    void addChild(FPNode * child);
    void removeChild(FPNode * child);
    void setitem(int item) { this->item = item; };
    void setParent(FPNode * parent) { this->parent = parent; }

    void increment() { this->count++; }
    void decrement() { this->count--; }
};

Q_DECLARE_METATYPE(FPNode);

QDebug operator<<(QDebug dbg, const FPNode &node);

#endif // FPNODE_H
