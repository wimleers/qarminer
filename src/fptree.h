#ifndef FPTREE_H
#define FPTREE_H

#include <QHash>
#include <QPair>
#include <QDebug>
#include <QMetaType>
#include <QString>

#include "typedefs.h"
#include "fpnode.h"

class FPTree {
protected:
    FPNode* root;
    QHash<Item, FPNodeList> itemPaths;
    ItemNameHash * itemNames;

    void addNodeToItemPath(FPNode* node);
    void removeNodeFromItemPath(FPNode* node);

public:
    FPTree();
    ~FPTree();

    // Accessors.
    FPNode* getRoot() const { return this->root; }
    bool hasItemPath(Item item) const;
    FPNodeList getItemPath(Item item) const;
    bool itemPathContains(Item item, FPNode* node) const;
    ItemNameHash* getItemNames() const { return this->itemNames; }

    // Modifiers.
    void addTransaction(Transaction transaction);
    FPTree* getConditionalFPTreeFor(Item item);
    void setItemNames(ItemNameHash* itemNames) { this->itemNames = itemNames; }
};

Q_DECLARE_METATYPE(FPTree);

QDebug operator<<(QDebug dbg, const FPTree &tree);
QString dumpHelper(const FPNode &node, QString prefix = "");
QDebug operator<<(QDebug dbg, const FPNodeList &itempath);
QDebug operator<<(QDebug dbg, const Transaction &transaction);

#endif // FPTREE_H
