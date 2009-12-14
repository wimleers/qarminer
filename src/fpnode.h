#ifndef FPNODE_H
#define FPNODE_H

#include <QHash>

#define NULL_ATTRIBUTE -1

class FPNode {
protected:
    FPNode * parent;
    FPNode * sibling;
    QHash<int, FPNode *> children;
    int count;
    int attribute;

public:
    FPNode(int attribute, FPNode * parent = NULL);
    ~FPNode();

    bool isRoot();
    bool isLeaf() { return this->children.size() == 0; }

    int getAttribute() { return this->attribute; }
    FPNode * getParent() { return this->parent; }
    FPNode * getSibling() { return this->sibling; }
    FPNode * getChild(int attribute);
    bool hasChild(int attribute);

    void addChild(FPNode * child);
    void removeChild(FPNode * child);
    void setAttribute(int attribute) { this->attribute = attribute; };
    void setParent(FPNode * parent) { this->parent = parent; }
    void setSibling(FPNode * sibling) { this->sibling = sibling; }

    void increment() { this->count++; }
    void decrement() { this->count--; }
};

#endif // FPNODE_H
