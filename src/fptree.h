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
    QList<Item> getItems() const { return this->itemPaths.keys(); }
    FPNodeList getItemPath(Item item) const;
    bool itemPathContains(Item item, FPNode* node) const;
    ItemNameHash* getItemNames() const { return this->itemNames; }
    ItemCount getItemSupport(Item item) const;
    QList<ItemList> calculatePrefixPaths(Item item) const;

    // Modifiers.
    void addTransaction(Transaction transaction);
    FPTree* getConditionalFPTreeFor(Item item);
    void setItemNames(ItemNameHash* itemNames) { this->itemNames = itemNames; }

    // Static (class) methods.
    static ItemCountHash calculateSupportCountsForPrefixPaths(QList<ItemList> prefixPaths);
};

Q_DECLARE_METATYPE(FPTree);

QDebug operator<<(QDebug dbg, const FPTree &tree);
QString dumpHelper(const FPNode &node, QString prefix = "");

#endif // FPTREE_H
